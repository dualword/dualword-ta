/*
 * 
 * Copyright 2016 Lucas Tsatiris <chartgeany@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "qtachart_core.h"

// draw/refresh the object
void
QTACObject::draw ()
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  QTACObject *child;
  QString timeframe = "";
  qint32 max;
  
  if (core->events_enabled == false)
    return;
  
  basex = core->chartleftmost;
  width = core->chartwidth;
  if (type != QTACHART_OBJ_SUBCHART) // object is not a subchart
  {
	if (parentObject == NULL) // object belongs to main chart
    {
	  if (core->reloaded)
        valueSet ();	
      basey = core->charttopmost;
      height = core->chartheight;
    }
    else           		// object belongs to a subchart
    {
      basey = parentObject->basey;
      height = parentObject->height;
    }
	  
	if (type == QTACHART_OBJ_LABEL)
    {
      drawLabel ();
      return;
    }
  
    if (type == QTACHART_OBJ_TEXT)
    {
      drawText ();
      return;
    }
  
    if (type == QTACHART_OBJ_HLINE || type == QTACHART_OBJ_VLINE)
    {
      drawHVLine ();
      return;
    }
  
    if (type == QTACHART_OBJ_LINE)
    {
	  drawTLine ();  
      return;
    }
  
    if (type == QTACHART_OBJ_FIBO)
    {
	  drawFibo ();  
      return;
    } 

    X.clear ();
    
    qint32 vsetsize = valueset->size ();     
    max = core->nbars_on_chart + *core->startbar;
    for (qint32 counter = *core->startbar, i = 0;
         counter < max && counter < vsetsize;
         counter ++, i ++)
    {
      qreal x;
      x = width - (core->framewidth * 1.5 * (i + 1)) + 1;
      x += (core->framewidth * 0.8) / 2;
      x += basex;
      X += x;
    }

    Y.clear ();
    Y.reserve (X.size ());
    
    for (qint32 counter = *core->startbar;
         counter < max && counter < vsetsize;
         counter ++)
    {
	 
	  if (valueset->at(counter) == 0 && parentObject == NULL)
	    Y += 0;
	  else
	  {  	
        qreal y;
        if (parentObject == NULL || parentObject->type == QTACHART_OBJ_CURVE)
          y = yOnPrice (core, valueset->at(counter));
        else
          y = subYonPrice (valueset->at(counter));
        Y += y;
      }  
    }
    
    if (type == QTACHART_OBJ_VBARS)
    {
      drawVBars ();
      return;
    }
    
    if (type == QTACHART_OBJ_CURVE || type == QTACHART_OBJ_DOT)
    {
	  if (filter == NULL && parentObject == NULL)
      {
		title->setParent (this);  
        filter = new QTACObjectEventFilter (title);  
	    title->installEventFilter (filter); 
        title->setAcceptHoverEvents (true);
      }
      
      title->setVisible (false);
	  if (*core->startbar == 0 && dataset->size () > period)
	  {
		if (parentObject == NULL)
		{
		  QRectF rectf;  
		  rectf = title->boundingRect();  	
		  if (rectf.width () + (X.at (0) + 5) < core->chartrightmost)
		  {	
		    title->setVisible (true);
		    title->setDefaultTextColor (forecolor);
		    title->setPos (X.at (0) + 5, Y.at (0) - 12); 
		  }  
		}  
      }	
      
      if (type == QTACHART_OBJ_CURVE) 
        drawCurve ();
      else
        drawDot ();
    }
  }
  else // object is a subchart
  {
    QPen pen;
    QTACObject *objcursor;
    quint32 chart_sequence = 0, nobjects;
    
    if (core->reloaded)
      valueSet ();
    
    // no tooltip for title
    title->setToolTip (""); 
    
    // determine subchart's height
    height = core->height -
             (core->chartheight + core->bottomline_height +
              core->title_height + (core->chartframe * 2));
    height /= core->nsubcharts;
    
    // initialize rangemin and range max
    if (rmin == QREAL_MIN)
      rangemin = QREAL_MAX;
    else
      rangemin = rmin;

    if (rmax == QREAL_MAX)
      rangemax = QREAL_MIN;
    else
      rangemax = rmax;
    
    foreach (child, children)
      child->minmax ();
    
    foreach (child, children)
    {
      if (child->type == QTACHART_OBJ_CURVE || 
          child->type == QTACHART_OBJ_VBARS)
      {    		
	    if (rangemax < child->rangemax)
	      rangemax = child->rangemax;
	    if (rangemin > child->rangemin)
	      rangemin = child->rangemin;
	  }    
    }	
    
    quantum = (qAbs (rangemax - rangemin) / (height - 35));
    if (quantum == 0)
        quantum = 1;
    
    foreach (child, children)
    {
      child->quantum = quantum;
      child->rangemin = rangemin;
      child->rangemax = rangemax;
    }  
    
    // bottom line's pen
    pen.setColor (Qt::darkYellow);
    pen.setStyle (Qt::SolidLine);
    pen.setWidth (2);
   
    // set title
    if (period > 0)
      title->setPlainText (titlestr + "(" + (period > 0?QString::number (period, 'f', 0):"") + "):" + datastr);  
    
    // hide edit button if there is no parameters' dialog
    if (paramDialog == NULL)
      editBtn->setVisible (false);
    
    // determine subchart's sequence
    nobjects = core->Object.size ();
    for (quint32 counter = 0; counter < nobjects &&
                              core->Object.at (counter) != this; 
                              counter ++)
    {
	  objcursor = core->Object.at (counter);
	  if (objcursor->type == QTACHART_OBJ_SUBCHART &&
	      objcursor->deleteit == false)
	    chart_sequence ++;   	
    }                          
    
    // determine subchart's y
    basey = core->chartbottomost + ( chart_sequence * height);

    title->setPos (basex , basey + 5);
    if (bottomline != NULL)
    {
      scene->removeItem (bottomline);
      delete bottomline;
      bottomline = NULL;
    }
    
    // add bottom line
    bottomline = scene->addLine (basex + 1, basey + height, core->width, basey + height, pen);

    // add close and edit buttons
    prxcloseBtn->setGeometry (QRectF (core->chartrightmost - 24, basey + 4, 22, 22));
    prxeditBtn->setGeometry (QRectF (core->chartrightmost - 64, basey + 7, 30, 16));
  }
  
  // draw children objects
  foreach (child, children)
    child->draw ();
}

// draw 
void
QTACObject::drawLabel ()
{
  QRectF rect;		
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qreal x, y, width, height;
  
  text->setVisible (false);
  title->setVisible (false);
  if (deleteit)
    return; 
  
  height = qAbs (core->chartbottomost - core->charttopmost);
  width =  qAbs (core->chartrightmost - core->chartleftmost);
  x = width * relx;
  y = height * rely;
  x += core->chartleftmost;
  y += core->charttopmost;
  
  rect = text->boundingRect ();	
  
  if (x < core->chartleftmost)
    x = core->chartleftmost;
  if (y < core->charttopmost)
    y = core->charttopmost;
  if (x + rect.width () > core->chartrightmost)
    x = core->chartrightmost - rect.width ();
  if (y + rect.height () > core->chartbottomost)
    y =  (core->chartbottomost - rect.height ());    
  
  text->setPos (x, y);
  text->setVisible (true);
  objx = x;
  objy = y;
}

// draw text
void
QTACObject::drawText ()
{
  QRectF rect;	
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qreal x = -1, y = 0;
  int found = -1;
  
  text->setVisible (false);
  title->setVisible (false);
  if (deleteit)
    return; 
  
  for (qint32 counter = 0; counter < core->nbars_on_chart; counter ++)
  {
    if ( counter + *core->startbar < core->HLOC->size ()) 	  
      if (trailerCandleText == (*core->HLOC).at (counter + *core->startbar).Text)
	    found = counter;    
  }	
  
  if (found == -1)
    return;
  
  y = yOnPrice (core, price);
  x =  core->chartwidth - (core->framewidth * 1.5 * (found + 1)) + 1;
  x += (core->framewidth * 0.8) / 2;
  x += core->chartleftmost;
  text->setPos (x, y);
  
  rect = text->boundingRect ();	
  
  if (core->chartrightmost - x < rect.width ())
    return;
  
  if (x < core->chartleftmost)
    x = core->chartleftmost;
  if (y < core->charttopmost)
    y = core->charttopmost;
  if (x + rect.width () > core->chartrightmost)
    x = core->chartrightmost - rect.width ();
  if (y + rect.height () > core->chartbottomost)
    y =  (core->chartbottomost - rect.height ()); 
  
  text->setVisible (true);
  objx = x;
  objy = y;
}

// draw vertical bars
void
QTACObject::drawVBars ()
{
  QPen pen;	
  QTCGraphicsItem *item;	
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qreal x, y, w, h, y0;	
  int type = RectItemType, icounter = 0;
  
  pen.setColor (forecolor);
  clearITEMS ();
   
  y0 = subYonPrice (0);
  for (qint32 counter = 0; counter < Y.size (); counter ++)
  {
    w = core->framewidth / 4;
    x = X.at (counter);
    
    if (y0 > Y.at (counter))
      y = Y.at (counter);
    else
      y = y0;
    
    pen.setWidthF (w);
    h = qAbs (Y.at (counter) - y0); 
    if (Q_UNLIKELY (icounter >= ITEMSsize))
    {
      item = new QTCGraphicsItem (type);
      item->setRect (QRectF (x, y, w, h));
      item->setPen (pen);
      ITEMS[icounter] = item; 
      item->init (scene);    
      item->setZValue (0.9); 
    }
    else
    {
	  item = ITEMS[icounter]; 
	  item->setRect (QRectF (x, y, w, h));
    }  
    
    item->setPen (pen);
    item->setVisible (true);
    icounter ++;	
  }
  
  visibleitems = icounter;
  if (icounter > ITEMSsize)
    ITEMSsize = icounter;
}

// draw curve
void
QTACObject::drawCurve ()
{
  QPen pen;
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  QTCGraphicsItem *item;	
  qreal x1, x2, y1, y2;	
  int type = LineItemType, icounter = 0;
  
  pen.setColor (forecolor);
  pen.setWidth (1);
  clearITEMS ();
  
  if (Q_UNLIKELY(Y.size () < 2))
    return;
  
  x1 = X.at (0);
  y1 = Y.at (0);
  
  if (Q_UNLIKELY(y1 == 0))
    return;
  
  for (qint32 counter = 1, maxcounter = Y.size ();
       counter < maxcounter; counter ++)
  {
	x2 = X.at (counter);
	y2 = Y.at (counter);
	if (Q_UNLIKELY(y2 == 0))
	  return;
	
    if (Q_LIKELY(x1 > core->chartleftmost && x2 > core->chartleftmost))
	if (Q_LIKELY(y1 > basey && y1 < (basey + height)))
	if (Q_LIKELY(y2 > basey && y2 < (basey + height)))
	{
	  if (Q_UNLIKELY (icounter >= ITEMSsize))
      {
        item = new QTCGraphicsItem (type);
        item->setLine (QLineF (x1, y1, x2, y2));
        item->setPen (pen);
        ITEMS[icounter] = item; 
        item->init (scene); 
        item->setZValue (0.9);    
      }
      else
      {
		item = ITEMS[icounter]; 
		item->setLine (QLineF (x1, y1, x2, y2));
      }  
      item->setPen (pen);
      item->setVisible (true);
      icounter ++;	
	}  
	x1 = x2;
	y1 = y2;
  }  
  visibleitems = icounter;
  if (icounter > ITEMSsize)
    ITEMSsize = icounter;
}

// draw dot
void
QTACObject::drawDot ()
{
  QPen pen;	
  QTCGraphicsItem *item;	
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qreal x1, y1;
  int type = EllipseItemType, icounter = 0;
  
  pen.setColor (forecolor);
  pen.setWidth (1); 
  clearITEMS ();
  
  if (Y.size () < 2)
    return;
  
  if (Y.at (0) == 0)
    return;

  for (qint32 counter = 0, maxcounter = Y.size ();
       counter < maxcounter; counter ++)
  {
	x1 = X.at (counter);
	y1 = Y.at (counter);
	if (y1 == 0)
	  return;
	
	if (x1 > core->chartleftmost && x1 < core->chartrightmost)
	if (y1 > core->charttopmost && y1 < core->chartbottomost)
	{  
      if (Q_UNLIKELY (icounter >= ITEMSsize))
      {
        item = new QTCGraphicsItem (type);
        item->setRect (QRectF (x1, y1, 3, 3));
        item->setPen (pen);
        if (item->brush ().style () != Qt::SolidPattern) 
          item->setBrush (QBrush(Qt::SolidPattern));
        ITEMS[icounter] = item; 
        item->init (scene);  
        item->setZValue (0.9);   
      }
      else
      {
		item = ITEMS[icounter]; 
		item->setRect (QRectF (x1, y1, 3, 3));
        if (item->brush ().style () != Qt::SolidPattern) 
          item->setBrush (QBrush(Qt::SolidPattern));
      }  
      item->setPen (pen);
      item->setVisible (true);
      icounter ++;	
	}
  }
  
  visibleitems = icounter;
  if (icounter > ITEMSsize)
    ITEMSsize = icounter;
}

// draw trend line
void 
QTACObject::drawTLine ()
{
  QRectF rectf;	
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  LineEdge *edge;
  qreal x1 = 0, x2 = 0, y1 = 0, y2 = 0;
  int found = -1;
  
  if (hvline->isVisible ())
  {
    hvline->setVisible (false);
    Edge[0]->pricetxt->setVisible (false);
    Edge[1]->pricetxt->setVisible (false);
  }  
    
  if (Q_UNLIKELY(deleteit))
    return; 
  
  if (Edge[0]->trailerCandleText.size () < 4)
    found = 0;
  else  
  for (qint32 counter = 0; counter < core->nbars_on_chart; counter ++)
  {
    if ( counter + *core->startbar < core->HLOC->size ()) 	  
      if (Edge[0]->trailerCandleText == (*core->HLOC).at (counter + *core->startbar).Text)
	    found = counter;    
  }	
 
  if (found == -1)
    return;  
  
  y1 = yOnPrice (core, Edge[0]->price);
  if (Edge[0]->trailerCandleText.size () > 3)
  {
	x1 =  core->chartwidth - (core->framewidth * 1.5 * (found + 1)) + 1;
    x1 += (core->framewidth * 0.8) / 2;
    x1 += core->chartleftmost;  
  }
  else
  {
    if (Edge[0]->txtdirection == 1)
    {
      x1 = core->chartrightmost - *core->excess_drag_width;
      x1 += (Edge[0]->pad * (core->framewidth * 1.5));
    }  
    else
    {
	  x1 = core->HLOC->size () - *core->startbar; 
      x1 = core->framewidth * 1.5 * x1;	
	  x1 =  x1 + *core->excess_drag_width;
	  x1 =  core->chartrightmost - x1;	
	  x1 -= (Edge[0]->pad * (core->framewidth * 1.5));
    }    
  }
  
  found = -1;
  if (Edge[1]->trailerCandleText.size () < 4)
    found = 0;
  else 
  for (qint32 counter = 0; counter < core->nbars_on_chart; counter ++)
  {
    if ( counter + *core->startbar < core->HLOC->size ()) 	  
      if (Edge[1]->trailerCandleText == (*core->HLOC).at (counter + *core->startbar).Text)
	    found = counter;    
  }	
  
  if (found == -1)
    return; 
  
  y2 = yOnPrice (core, Edge[1]->price);
  if (Edge[1]->trailerCandleText.size () > 3)
  {
	x2 =  core->chartwidth - (core->framewidth * 1.5 * (found + 1)) + 1;
    x2 += (core->framewidth * 0.8) / 2;
    x2 += core->chartleftmost;  
  }
  else
  {
	if (Edge[1]->txtdirection == 1)
    {
      x2 = core->chartrightmost - *core->excess_drag_width;
      x2 += (Edge[1]->pad * (core->framewidth * 1.5));
    }  
    else
    {
	  x2 = core->HLOC->size () - *core->startbar; 
      x2 = core->framewidth * 1.5 * x2;	
	  x2 =  x1 + *core->excess_drag_width;
	  x2 =  core->chartrightmost - x2;	
	  x2 -= (Edge[1]->pad * (core->framewidth * 1.5));
    }    
  }  	
  
  if (x1 < core->chartleftmost)
    return;
  if (x1 > core->chartrightmost)
    return;
  if (y1 < core->charttopmost)
    return;
  if (y1 > core->chartbottomost)
    return;
      
  if (x2 < core->chartleftmost)
    return;
  if (x2 > core->chartrightmost)
    return;
  if (y2 < core->charttopmost)
    return;
  if (y2 > core->chartbottomost)
    return;
  
  hvline->setLine (x1, y1, x2, y2);
  hvline->setVisible (true);
  foreach (edge, Edge)
  {
	qreal x = 0, y = 0;  
    rectf = edge->pricetxt->boundingRect();
    
    if (edge->sequence == 1)
    {
	  x = x1;
	  y = y1;
	}
	else
	if (edge->sequence == 2)
	{
	  x = x2;
	  y = y2;
    }  	
    
    if (edge->txtdirection == 0)
      x -=  (rectf.width () + 2);
    else
      x += 2;
    
    if (x > core->chartleftmost && 
        x < (core->chartrightmost - rectf.width ()) &&
        y > core->charttopmost &&
        y < (core->chartbottomost - rectf.height ()))
    {     
      edge->pricetxt->setPos ( x, y - 12);
      edge->pricetxt->setVisible (true);
    }
    else
      edge->pricetxt->setVisible (false);  
  }
}

// draw fibo line
void 
QTACObject::drawFibo ()
{
  QRectF rectf;	
  QLineF line;
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  LineEdge *edge;
  qreal x1 = 0, x2 = 0, y1 = 0, y2 = 0;
  
  if (hvline->isVisible ())
  {
    hvline->setVisible (false);
    Edge[0]->pricetxt->setVisible (false);
    Edge[1]->pricetxt->setVisible (false);
    for (qint32 counter = 0; counter < FiboLevelPrc.size (); counter ++)
    {
      FiboLevel[counter]->setVisible (false);
      FiboLevelLbl[counter]->setVisible (false);
      FiboLevelPrcLbl[counter]->setVisible (false);
    }  
  }  
    
  if (deleteit)
    return; 
  
  line = hvline->line ();
  
  y1 = yOnPrice (core, Edge[0]->price);
  x1 = line.x1 ();
  
  y2 = yOnPrice (core, Edge[1]->price);
  x2 = x1;
  
  if (x1 < core->chartleftmost)
    return;
  if (x1 > (core->chartrightmost - 75))
    return;
  if (y1 < core->charttopmost)
    return;
  if (y1 > core->chartbottomost)
    return;
      
  if (x2 < core->chartleftmost)
    return;
  if (x2 > (core->chartrightmost - 75))
    return;
  if (y2 < core->charttopmost)
    return;
  if (y2 > core->chartbottomost)
    return;
    
  X1 = x1; Y1 = y1; X2 = x2; Y2 = y2;
  hvline->setLine (x1, y1, x2, y2);
  hvline->setVisible (true);
  foreach (edge, Edge)
  {
	qreal x = 0, y = 0;  
    rectf = edge->pricetxt->boundingRect();
    
    if (edge->sequence == 1)
    {
	  x = x1;
	  y = y1;
	}
	else
	if (edge->sequence == 2)
	{
	  x = x2;
	  y = y2;
    }  	
    
    if (edge->txtdirection == 0)
      x -=  (rectf.width () + 2);
    else
      x += 2;
       
    edge->pricetxt->setPos ( x, y - 12);
    edge->pricetxt->setVisible (true);
  }
  
  FiboLevelPrice.clear ();
  for (qint32 counter = 0; counter < FiboLevelPrc.size (); counter ++)
  {
	QString prcstr;
	qreal price, x;  
	price = qAbs (Edge[0]->price - Edge[1]->price);
	price *= FiboLevelPrc[counter];
	if (Edge[0]->price < Edge[1]->price)
	  price += Edge[0]->price;
	else
	  price += Edge[1]->price;
	FiboLevelPrice += price;
	prcstr = QString::number (price, 'f', 2);	
	
    FiboLevelPrcLbl[counter]->setPlainText (prcstr);
 	rectf = FiboLevelPrcLbl[counter]->boundingRect();
    x =  core->chartrightmost - (rectf.width () + 2);
	((QGraphicsLineItem *)FiboLevel[counter])->setPen (hvline->pen ());
	((QGraphicsLineItem *)FiboLevel[counter])->setLine (line.x1 (), 
	                                             yOnPrice (core, price),
	                                         x, yOnPrice (core, price)); 
	FiboLevelPrcLbl[counter]->setPos (x + 1, yOnPrice (core, price) - 12);
	FiboLevelPrcLbl[counter]->setDefaultTextColor (hvline->pen().color ());
    FiboLevelLbl[counter]->setVisible (true); 
    FiboLevelPrcLbl[counter]->setVisible (true); 
    ((QGraphicsLineItem *)FiboLevel[counter])->setVisible (true);  
	
	if (counter > 0 && counter < FiboLevelPrc.size () - 1)
	{
      FiboLevelLbl[counter]->setDefaultTextColor (hvline->pen().color ());
      rectf = FiboLevelLbl[counter]->boundingRect();
      x =  line.x1 () - (rectf.width () + 2);
      FiboLevelLbl[counter]->setPos (x, yOnPrice (core, price) - 12);
      FiboLevelLbl[counter]->setVisible (true);
    }                        
  }
}

// draw horizontal or verical line
void 
QTACObject::drawHVLine ()
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  QRectF rectf;
  qreal x, y, top, bottom;
  
  if (type == QTACHART_OBJ_HLINE)
  {
	if (parentObject == NULL)
	{  
      top = core->charttop * core->points;
      bottom = core->chartbottom * core->points;
  
      if (price >= top || price <= bottom)
      {
        hvline->setVisible (false);
        title->setVisible (false);
      }  
      else  
      {
        hvline->setVisible (true);
        title->setVisible (true);
      }  
      
      y = yOnPrice (core, price);
      rectf = title->boundingRect();
      hvline->setLine (core->chartleftmost, y, core->chartrightmost - rectf.width (), y);
      title->setPos (core->chartrightmost - rectf.width (), y - 10);
      return;
    }  
     
     y = subYonPrice (price);
     if (y < basey || y > basey + height)
     {
	   hvline->setVisible (false);
	   title->setVisible (false);
	   return;
	 }  	 
     
     hvline->setVisible (true);
	 title->setVisible (true);
     hvline->setLine (core->chartleftmost, y, core->chartrightmost, y);
     title->setPos (core->ruller_cursor_x, y - 10);
     return;
  }
  
  if (type == QTACHART_OBJ_VLINE)
  {
    int found = -1;
    
    hvline->setVisible (false);
    title->setVisible (false);
    if (deleteit)
      return; 
  
    for (qint32 counter = 0; counter < core->nbars_on_chart; counter ++)
    {
      if ( counter + *core->startbar < core->HLOC->size ()) 	  
      if (trailerCandleText == (*core->HLOC).at (counter + *core->startbar).Text)
	    found = counter;    
    }	
  
    if (found == -1)
      return;
    
     
    x =  core->chartwidth - (core->framewidth * 1.5 * (found + 1)) + 1;
    x += (core->framewidth * 0.8) / 2;
    x += core->chartleftmost + 2;
    
    if (x + (title->boundingRect().width () / 2) > core->chartrightmost)
      return;
    
    if (x - (title->boundingRect().width () / 2) < core->chartleftmost)
      return;  
          
    rectf = title->boundingRect();
    hvline->setLine (x, core->charttopmost, x, core->chartbottomost - rectf.height () + 3);
    title->setPos (x - (rectf.width () / 2), core->chartbottomost - 12);
    hvline->setVisible (true);
    title->setVisible (true);
  }	  
}



