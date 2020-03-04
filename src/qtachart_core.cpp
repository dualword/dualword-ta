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

// constructor, destructor and some methods
// of QTAChartCore

#include <QtCore/qmath.h>
#include <QApplication>
#include <QStringBuilder>
#include "qtachart_core.h"
#include "defs.h"

// constructor
QTAChartCore::QTAChartCore (QWidget * parent)
{
  chart = &View;
  scene = &Scene;
  visibleitems = 0;
  visiblegriditems = 0;
  ITEMSsize = 0;
  GRIDsize = 0;
  priceUpdater = NULL;
  tfBtn = NULL;
  ITEMS = new QTCGraphicsItem * [MAXITEMS];
  GRID = new QTCGraphicsItem * [MAXGRIDLINES];
  if (parent != NULL)
   setParent (parent);
  
  title = new QGraphicsTextItem ;
  subtitle = new QGraphicsTextItem ;
  scaletitle = new QGraphicsTextItem ;
  typetitle = new QGraphicsTextItem ;
  bottom_text = new QGraphicsTextItem;
  ruller_cursor = new QGraphicsTextItem;
  expandBtn = new QToolButton;
  propertiesBtn = new QToolButton;
  functionBtn = new QToolButton;
  objectsBtn = new QToolButton;
  drawBtn = new QToolButton;
  helpBtn = new QToolButton;
  dataBtn = new QToolButton;
  zoomInBtn = new QToolButton;
  zoomOutBtn = new QToolButton;
  rightedge = new QGraphicsLineItem;
  leftedge = new QGraphicsLineItem;
  topedge = new QGraphicsLineItem;
  bottomedge = new QGraphicsLineItem;
  propScr = new QTACProperties;
  drawScr = new QTACDraw;
  functionScr = new QTACFunctions;
  objectsScr = new QTACObjects;
  helpScr = new QTACHelp;
  dataScr = new QTACData;
  textobjectdialog = new TextObjectDialog;
  textobjectdialog->setModal (true);
  lineobjectdialog = new LineObjectDialog;
  lineobjectdialog->setModal (true);
  chartEventFilter = new QTAChartEventFilter;
  sceneEventFilter = new QTAChartSceneEventFilter ();
  
  monthlit << "JAN" << "FEB" << "MAR" << "APR" << "MAY" << "JUN"
            << "JUL" << "AUG" << "SEP" << "OCT" << "NOV" << "DEC";
  
  return;
}

// destructor
QTAChartCore::~QTAChartCore (void)
{
  int nbuttons;
  
  deleteOnlinePrice ();
  
  foreach (const QTACObject *object, Object)
    delete object;
  
  nbuttons = TIMEFRAME.size ();
  if (nbuttons != 0)
  {
	for (qint32 counter = 0; counter < nbuttons; counter ++)
	  delete tfBtn[counter];	

    delete[] prxtfBtn;
    delete[] tfBtn;
  }
    
  delete lineobjectdialog;
  delete textobjectdialog;
  delete rightedge;
  delete leftedge;
  delete topedge;
  delete bottomedge;
  delete zoomOutBtn;
  delete zoomInBtn;
  delete helpBtn;
  delete dataBtn;
  delete propertiesBtn;
  delete drawBtn;
  delete functionBtn;
  delete expandBtn;
  delete ruller_cursor;
  delete bottom_text;
  delete scaletitle;
  delete typetitle;
  delete title;
  delete subtitle;
  delete propScr;
  delete drawScr;
  delete helpScr;
  delete dataScr;
  delete functionScr;
  delete objectsScr;
  delete chartEventFilter;
  delete sceneEventFilter;
    
  deleteITEMS ();
  delete[] ITEMS;
  
  deleteGRID ();
  delete[] GRID;
    
  return;
}

/// Aa
// add label
QTACObject *
QTAChartCore::addLabel (QGraphicsTextItem *item, qreal x, qreal y)
{
  QTACObject *label, *found = NULL;
  
  foreach (label, Object)
    if ((label->type == QTACHART_OBJ_LABEL ||
         label->type == QTACHART_OBJ_TEXT) &&
         label->text == item)
      found = label;    
  
  if (found == NULL)
    label = new QTACObject (this, QTACHART_OBJ_LABEL);
  else
  {
	label = found;
	label->type = QTACHART_OBJ_LABEL;  
  } 
  
  label->setText (item, x, y);
  label->setTitle ("Label");  
  
  return label;
}  

// add subchart button 
SubChartButton *
QTAChartCore::addSCB (QTACObject *obj, QString name)
{
  SubChartButton *btn;
  
  btn = new SubChartButton (obj);
  btn->setObjectName (name);
  connect (btn, SIGNAL (clicked ()), this, 
           SLOT (SCBtn_clicked ()));
  
  CloseSCB += btn;
  return btn;	
}

// add horizontal line
void
QTAChartCore::addHLine (QGraphicsLineItem *item, qreal price)
{
  QTACObject *hline, *found = NULL;
  
  foreach (hline, Object)
    if ( hline->type == QTACHART_OBJ_HLINE &&
         hline->hvline == item)
      found = hline; 
  
  if (found == NULL)	
    hline = new QTACObject (this, QTACHART_OBJ_HLINE);
  else
    hline = found;
 	    
  hline->setHLine (item, price);
}

// add vertical line
void
QTAChartCore::addVLine (QGraphicsLineItem *item)
{
  QTACObject *vline, *found = NULL;
  
  foreach (vline, Object)
    if ( vline->type == QTACHART_OBJ_VLINE &&
         vline->hvline == item)
      found = vline; 
  
  if (found == NULL)	
    vline = new QTACObject (this, QTACHART_OBJ_VLINE);
  else
    vline = found;
 	    
  vline->setVLine (item);
}

void
QTAChartCore::addVLine (QGraphicsLineItem *item, QString trailerCandleText)
{
  QTACObject *vline, *found = NULL;
  
  foreach (vline, Object)
    if ( vline->type == QTACHART_OBJ_VLINE &&
         vline->hvline == item)
      found = vline; 
  
  if (found == NULL)	
    vline = new QTACObject (this, QTACHART_OBJ_VLINE);
  else
    vline = found;
 	    
  vline->setVLine (item, trailerCandleText);
}

// add trend line
void
QTAChartCore::addTLine (QGraphicsLineItem *item)
{
 QTACObject *sline, *found = NULL;
  
  foreach (sline, Object)
    if ( sline->type == QTACHART_OBJ_LINE &&
         sline->hvline == item)
      found = sline; 
  
  if (found == NULL)	
    sline = new QTACObject (this, QTACHART_OBJ_LINE);
  else
    sline = found;
 	    
  sline->setTLine (item);  	
}	

void
QTAChartCore::addTLine (QGraphicsLineItem *item, LineEdge e1, LineEdge e2)
{
 QTACObject *sline, *found = NULL;
  
  foreach (sline, Object)
    if ( sline->type == QTACHART_OBJ_LINE &&
         sline->hvline == item)
      found = sline; 
  
  if (found == NULL)	
    sline = new QTACObject (this, QTACHART_OBJ_LINE);
  else
    sline = found;
 	    
  sline->setTLine (item, e1, e2);  	
}

// add fibo
void
QTAChartCore::addFibo (QGraphicsLineItem *item)
{
 QTACObject *sline, *found = NULL;
  
  foreach (sline, Object)
    if ( sline->type == QTACHART_OBJ_FIBO &&
         sline->hvline == item)
      found = sline; 
  
  if (found == NULL)	
    sline = new QTACObject (this, QTACHART_OBJ_FIBO);
  else
    sline = found;
 	    
  sline->setFibo (item);  	
}

void
QTAChartCore::addFibo (QGraphicsLineItem *item, LineEdge e1, LineEdge e2)
{
 QTACObject *sline, *found = NULL;
  
  foreach (sline, Object)
    if ( sline->type == QTACHART_OBJ_FIBO &&
         sline->hvline == item)
      found = sline; 
  
  if (found == NULL)	
    sline = new QTACObject (this, QTACHART_OBJ_FIBO);
  else
    sline = found;
 	    
  sline->setFibo (item, e1, e2);  	
}

// add trailing text
void 
QTAChartCore::addText(QGraphicsTextItem *item, qreal x, qreal y)
{
  QTACObject *text, *found = NULL;
  
  // trailing text needs to be attached on a candle 
  if (bottom_text->toPlainText () == " ")
  {
	addLabel (item, x, y); 
    return;
  }  
  
  foreach (text, Object)
    if ( text->type == QTACHART_OBJ_TEXT &&
         text->text == item)
      found = text;  
  
  if (found == NULL)
  {  
    text = new QTACObject (this, QTACHART_OBJ_TEXT);
    text->setTitle ("Text");
  }
  else
    text = found;
    
  text->setText (item, x, y);	
}

void 
QTAChartCore::addText(QGraphicsTextItem *item, QString candleText, qreal price)
{
  QTACObject *text;
  	
  text = new QTACObject (this, QTACHART_OBJ_TEXT);
  text->setTitle ("Text");	
  text->setText (item, candleText, price);
}

// add online price
void 
QTAChartCore::addOnlinePrice ()
{
  // PriceUpdater *pupdater;	
  QFont font;
  QGraphicsTextItem *textitem;	
  if (onlineprice != NULL)
    return;
    
  onlineprice = new QTACObject (this, QTACHART_OBJ_LABEL);  
  textitem = new QGraphicsTextItem;
  if (events_enabled == false)
    textitem->setVisible (false);
  else
    textitem->setVisible (true);  
  textitem->setPlainText ("Please wait...");
  textitem->setDefaultTextColor (forecolor);
  font.setFamily ("Tahoma");
  font.setWeight (QFont::Bold);
  font.setPixelSize (14);
  font.setPointSize (14);
  textitem->setFont (font);
  onlineprice->setTitle ("Label");  
  onlineprice->setText (textitem, 100, 100);
  scene->addItem (textitem);
  priceUpdater = new PriceUpdater (Symbol, Feed, onlineprice);
}

/// Bb
/// Cc

// change the foreground color
void 
QTAChartCore::changeForeColor (QColor color)
{
  QPen pen;
  	
  forecolor = color;
  textcolor = gridcolor = framecolor = forecolor;
  
  title->setDefaultTextColor (textcolor); 
  subtitle->setDefaultTextColor (textcolor); 
  scaletitle->setDefaultTextColor (textcolor); 
  typetitle->setDefaultTextColor (textcolor); 
  
  pen.setColor (gridcolor);
  topedge->setPen (pen);
  bottomedge->setPen (pen);
  leftedge->setPen (pen);
  rightedge->setPen (pen);
  
  helpBtn->setStyleSheet(
    QString("background: transparent;color: %1;font: 11px;\
            font-weight: bold;").arg(textcolor.name()));
  
  foreach (QTACObject *object, Object)
    object->changeForeColor (forecolor);
  
  if (tfBtn != NULL)
    for (qint32 counter = 0; counter < TIMEFRAME.size (); counter ++)
      tfBtn[counter]->setStyleSheet 
	    (QString ("background: transparent; color: %1;font: 19px;\
	      font-weight: bold;border: 1px solid transparent;\
	      border-color: darkgray;").arg (forecolor.name ()));  
  	
}

// move chart backward by nbars
void
QTAChartCore::chartBackward (int nbars)
{
  int maxstartbar;
  qreal k;
  
  k = *excess_drag_width;
  if (k > 0)
  {
    k -= (framewidth * 1.5 * nbars);
    if (k < 0)
      *excess_drag_width = 0;
    else
      *excess_drag_width = k; 
    draw ();
    return;
  }
 
  maxstartbar = HLOC->size () - nbars_on_chart;
  if (maxstartbar < 0)
    maxstartbar = 0;

  if (*startbar >= maxstartbar)
    return;

  (*startbar) += nbars;

  if ((*startbar) > maxstartbar)
    (*startbar) = maxstartbar;
  draw ();
}

// move chart forward by nbars
void
QTAChartCore::chartForward (int nbars)
{
  if (((*startbar) - nbars + 1) > 0)
  {
    (*startbar) -= nbars;
    draw ();
    return;
  }  
  
  qreal k;
  k = *excess_drag_width;
  
  if (k < (chartwidth / 2))
  {
    k += (framewidth * 1.5 * nbars);
    *excess_drag_width = k;
    draw ();
  }  
}

// move to chart's begin
void
QTAChartCore::chartBegin (void)
{
  *excess_drag_width = 0;
  (*startbar) = HLOC->size () - nbars_on_chart;
  (*startbar) ++;
  if ((*startbar) < 0)
    (*startbar) = 0;
  draw ();
}

// move to chart's end
void
QTAChartCore::chartEnd ()
{
  if (HLOC->size () < nbars_on_chart && *excess_drag_width == 0)
    return;

  *excess_drag_width = 0;
  (*startbar) = 0;
  draw ();
}

// next page
void
QTAChartCore::chartPageNext (void)
{
  if (*excess_drag_width > 0)
  {
    (*startbar) = 0;
    *excess_drag_width = 0;
    draw ();
    return;
  }

  if ((*startbar) != 0)
    chartForward (nbars_on_chart - 1);
}

// previous page
void
QTAChartCore::chartPagePrevious (void)
{
  if (*excess_drag_width > 0)
  {
    *excess_drag_width = 0;
    draw ();
    return;
  }

  chartBackward (nbars_on_chart - 1);
}

// clear chart's GRID
void
QTAChartCore::clearGRID (void)
{
  for (qint32 counter = 0, maxcounter = GRIDsize; 
       counter < maxcounter; counter ++)
  { 
  	GRID[counter]->setSize (0, 0, 0, 0);
  	GRID[counter]->setVisible (false);
  } 	
}

// clear chart's ITEMS
void
QTAChartCore::clearITEMS (void)
{	
  for (qint32 counter = visibleitems; counter < ITEMSsize; counter ++)
  	ITEMS[counter]->setSize (0, 0, 0, 0);
}  

// create time frame buttons
void
QTAChartCore::createTFButtons (void)
{
  int nbuttons, pad;
  
  nbuttons = TIMEFRAME.size ();
  if (nbuttons == 0)
    return;

  pad = 360;  
  tfBtn = new QToolButton * [nbuttons];
  prxtfBtn = new QGraphicsProxyWidget * [nbuttons];
  
  for (qint32 counter = 0; counter < nbuttons; counter ++)
  {
	tfBtn[counter] = new QToolButton; 
	tfBtn[counter]->setFixedSize (QSize (32, 28));
	tfBtn[counter]->setText (TIMEFRAME[counter].TFSymbol); 
	tfBtn[counter]->setStyleSheet 
	  (QString ("background: transparent; color: %1;font: 19px;\
	    font-weight: bold;border: 1px solid transparent;\
	    border-color: %1;").arg (forecolor.name ()));
	// tfBtn[counter]->setToolTip (TOOLTIP + TIMEFRAME[counter].TFName  +"</span>");
    tfBtn[counter]->setFocusPolicy (Qt::NoFocus);
	prxtfBtn[counter] = scene->addWidget (tfBtn[counter], Qt::Widget);
	prxtfBtn[counter]->setGeometry (QRectF ((counter * 40) + pad, 5, 32, 28));
	connect (tfBtn[counter], SIGNAL (clicked ()), this,
           SLOT (tfBtn_clicked ()));
  } 	
}

/// Dd
// draw grid
void
QTAChartCore::drawGRID (void)
{
  QGraphicsTextItem *titem;
  QTCGraphicsItem *litem;
  QTAChartFrame pframe;
  QString stepstr, framestr;
  QFont tfont;
  QPen pen;
  qint64 step, maxstep;
  qreal x, y;
  qint32 width, keepgridstep;
  int type = LineItemType, counter = 0;
 
  clearGRID ();
  pen.setColor (gridcolor);
#ifdef Q_OS_MAC  
  pen.setStyle (Qt::DashLine);
#else
  pen.setStyle (Qt::DotLine);
#endif  
  tfont = masterFont;
  tfont.setPixelSize (7 + FONT_SIZE_PAD);
  tfont.setWeight (QFont::DemiBold);
  tfont.setFamily (DEFAULT_FONT_FAMILY);
  
  keepgridstep = gridstep;
  maxstep = priceOnY (this, title_height * 2) / points;
  step = chartbottom;
  
  if (chartbottom < gridstep || (chartbottom % gridstep) != 0)
  {
    stepstr = QString ("%1").arg (((qreal) step) * points, 10, 'f', 2);
    y = yOnPrice (this, step * points);
    if (counter >= GRIDsize)
    {
        litem = new QTCGraphicsItem (type);
        litem->setZValue (0.0);
        GRID[counter] = litem; 
        titem = new QGraphicsTextItem (stepstr, litem);
        titem->setFont (tfont);
        titem->setDefaultTextColor (textcolor); 
        if (counter == 0)
        {
          titem = new QGraphicsTextItem (litem);
          titem->setFont (tfont);
          titem->setDefaultTextColor (textcolor); 
        }  
        litem->init (scene);  
    }
      
	litem = GRID[counter]; 
	litem->setPen (pen);
	litem->setLine (QLineF (chartleftmost, y, chartrightmost, y));
    titem = ((QGraphicsTextItem *)(litem->children ().at(0)));
    titem->setDefaultTextColor (textcolor); 
    titem->setPlainText (stepstr);
    titem->setPos (ruller_cursor_x, y - 10);
    litem->setVisible (true);
    titem->setVisible (true);
    counter ++; 
    visiblegriditems = counter;
    if (counter > GRIDsize)
      GRIDsize = counter;
  }
  
  step = gridstep;
  while (step < chartbottom)
        step += gridstep;
  
// horizontal grid
  for (; step < maxstep; step += gridstep)
  {
    stepstr = QString ("%1").arg (((qreal) step) * points, 10, 'f', 2);
    y = yOnPrice (this, step * points);
    if (counter >= GRIDsize)
    {
        litem = new QTCGraphicsItem (type);
        litem->setZValue (0.0);
        GRID[counter] = litem; 
        titem = new QGraphicsTextItem (stepstr, litem);
        titem->setFont (tfont);
        titem->setDefaultTextColor (textcolor); 
        if (counter == 0)
        {
          titem = new QGraphicsTextItem (litem);
          titem->setFont (tfont);
          titem->setDefaultTextColor (textcolor); 
        }  
        litem->init (scene);  
      }
      
	  litem = GRID[counter]; 
	  litem->setPen (pen);
	  litem->setLine (QLineF (chartleftmost, y, chartrightmost, y));
      titem = ((QGraphicsTextItem *)(litem->children ().at(0)));
      titem->setDefaultTextColor (textcolor); 
      titem->setPlainText (stepstr);
      titem->setPos (ruller_cursor_x, y - 10);
      litem->setVisible (true);
      titem->setVisible (true);
      counter ++; 
      visiblegriditems = counter;
      if (counter > GRIDsize)
        GRIDsize = counter;
  }

  step = charttop;
  stepstr = QString ("%1").arg (((qreal) step) * points, 10, 'f', 2);
  y = yOnPrice (this, step * points);
  titem = ((QGraphicsTextItem *)(GRID[0]->children ().at(1)));
  titem->setDefaultTextColor (textcolor); 
  titem->setPlainText (stepstr);
  titem->setPos (ruller_cursor_x, y - 10);
  gridstep = keepgridstep;
// vertical grid
drawGRID_vertical:
  if (HLOC->size () < 2)
    return;
    
  width = framewidth;
  pframe = HLOC->at (*startbar);
  for (qint32 xcounter = *startbar + 1, i = 1, 
       max1 = (*startbar + nbars_on_chart), max2 = HLOC->size (); 
       xcounter < max1 && xcounter < max2; xcounter ++, i++)
  {
	QRectF rectf;
	
	framestr = "";  
	x = -1; 
	
	if (currenttf == "DAY")
	{
	  if (pframe.month != HLOC->at (xcounter).month)
	  {
		x = chartrightmost - ((qreal) ((width * 1.5) * i)); 
		x -= (*excess_drag_width + 1);
		framestr = monthlit[pframe.month - 1] + " " + QString::number (pframe.year);
	  } 
    }  
    else
    if (currenttf == "WEEK")
	{
	  if (pframe.year != HLOC->at (xcounter).year)
	  {
		x = chartrightmost - ((qreal) ((width * 1.5) * i)); 
		x -= (*excess_drag_width + 1);
		framestr = QString::number (pframe.year);
	  } 
    }
    else
    if (currenttf == "MONTH")
	{
	  if ((HLOC->at (xcounter).year + 1) % 5 == 0 && HLOC->at (xcounter).month == 12)
	  {
		x = chartrightmost - ((qreal) ((width * 1.5) * i)); 
		x -= (*excess_drag_width + 1);
		framestr = QString::number (pframe.year);
	  } 
    }
    else
    if (currenttf == "YEAR")
	{
	  if ((HLOC->at (xcounter).year + 1) % 10 == 0)
	  {
		x = chartrightmost - ((qreal) ((width * 1.5) * i)); 
		x -= (*excess_drag_width + 1);
		framestr = QString::number (pframe.year);
	  } 
    }
    
    if (x != -1)
    {
	  if (counter >= GRIDsize)
      {
        litem = new QTCGraphicsItem (type);
        litem->setZValue (0.0);
        titem = new QGraphicsTextItem (litem);
        titem->setFont (tfont);
        titem->setDefaultTextColor (textcolor); 
        litem->init (scene); 
        GRID[counter] = litem; 
      }
      
      litem = GRID[counter];
      litem->setPen (pen);
      litem->setLine (QLineF (x, charttopmost, x, (height - (bottomline_height + chartframe)) - 5));
      titem = ((QGraphicsTextItem *)(litem->children ().at(0)));
      titem->setDefaultTextColor (textcolor); 
      titem->setVisible (true);
      titem->setPlainText (framestr);
      titem->setPos (x, chartbottomost - 15);
      litem->setVisible (true);
      rectf = titem->boundingRect();

      if (chartrightmost - rectf.width () < x)
        titem->setVisible (false);
         
      counter ++; 
      visiblegriditems = counter;
      if (counter > GRIDsize)
        GRIDsize = counter;	
    }
    pframe = HLOC->at (xcounter);  
  }
}

// draw volumes
void
QTAChartCore::drawVolumes (void)
{
  QTACObject *vbars;
  
  if (volumes != NULL)
    return;

  volumes = new QTACObject (this, QTACHART_OBJ_SUBCHART);
  volumes->setTitle ("Volume");
  volumes->setAttributes (QTACHART_VOLUME, 0, "", DUMMY, 0, QREAL_MAX, forecolor, "");
  vbars = new QTACObject (volumes, QTACHART_OBJ_VBARS);
  vbars->setAttributes (QTACHART_VOLUME, 0, "", DUMMY, 0, QREAL_MAX, forecolor, "");
  propScr->setVolumes (true);
  show_volumes = propScr->Volumes ();
}

// delete grid
void
QTAChartCore::deleteGRID (void)
{
  for (qint32 counter = 0; counter < GRIDsize; counter ++)
    delete GRID[counter];
  
  GRIDsize = 0;   
}

// delete volumes
void
QTAChartCore::deleteVolumes (void)
{
  if (volumes != NULL)
    deleteObject (volumes);
}

// delete object
void
QTAChartCore::deleteObject (QTACObject *obj)
{
  if (Object.indexOf (obj) != -1)
  {
    if (obj == volumes)
    {
      propScr->setVolumes (false);
      show_volumes = propScr->Volumes ();
      volumes = NULL;
    }
    
    if (obj == onlineprice)
    {
	  propScr->setOnlinePrice (false);
	  show_onlineprice = propScr->OnlinePrice ();
	  onlineprice = NULL;	
    }
    
    Object.remove (Object.indexOf(obj, 0));
    
    if (obj->type == QTACHART_OBJ_SUBCHART)
    {
      obj->subchart_dec = 0;
      nsubcharts --;
    }
    delete obj;
    draw ();
  }
}

// delete online price
void
QTAChartCore::deleteOnlinePrice ()
{
  if (onlineprice == NULL)
    return;
    
  deleteObject  (onlineprice);
  priceUpdater = NULL;
  
}

// delete ITEMS
void
QTAChartCore::deleteITEMS ()
{
  for (qint32 counter = 0; counter < ITEMSsize; counter ++)
    delete ITEMS[counter];
  
  ITEMSsize = 0;  
}

/// Ee
/// Ff
/// Gg
// form the bottom text
QString
QTAChartCore::getBottomText (int x)
{
  QString btext = " ";
  int bar = 0;
  
  bar = barOnX (this, x);
  if (Q_LIKELY (bar != -1))
    btext = (*HLOC)[bar].Text; 
    
  return btext;  
}

/// Hh
// hide all chart's objects
void
QTAChartCore::hideAllItems (void)
{
  QList < QGraphicsItem * >sceneitems;

  sceneitems = scene->items ();
  if (Q_UNLIKELY(sceneitems.size () == 0))
    return;

  foreach (QGraphicsItem *item, sceneitems) 
    item->setVisible (false);
  scene->setBackgroundBrush (Qt::black);
  scene->update ();
}

/// Ii
// initialize the core
void
QTAChartCore::init ()
{
  drawScr->setReferenceChart (reinterpret_cast<void*>(this->parent ()));
  functionScr->setReferenceChart (reinterpret_cast<void*>(this->parent ()));
  propScr->setReferenceChart (reinterpret_cast<void*>(this->parent ()));
  objectsScr->setReferenceChart (reinterpret_cast<void*>(this->parent ()));
}
/// Jj
/// Kk
/// Ll
// chart setting callback
static int 
sqlcb_chart_setting (void *data, int argc, char **argv, char **column)
{
  QString colname;
  float *rsetting;
  qint64 *isetting;
  char *csetting;
  
  if (argc > 1)
    return 1;
  
  colname = QString::fromUtf8(column[0]);
  colname = colname.toLower ();
  if (colname == "timeframe")	
  {
	csetting = (char *) data;
	strcpy (csetting, argv[0]);  
  } 	
  else
  if (colname == "edw")	
  {
	rsetting = (float *) data;
	*rsetting = (float) atof (argv[0]);
  }  
  else
  {
	isetting = (qint64 *) data;
	*isetting = (qint64) atoll (argv[0]);
  }

  return 0;	
}

// load settings
void
QTAChartCore::loadSettings ()
{
  QString SQLCommand = "";
  qint64 isetting = -1;
  float rsetting;
  int rc;
  char csetting[16];
  
  // volume
  SQLCommand = "SELECT VOLUME FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
	if (isetting == -1)
	  return; 
	    
	if (isetting == 1)
	  show_volumes = true;
	else
	  show_volumes = false;    
	propScr->setVolumes (show_volumes);  
  } 
  
  // linear
  SQLCommand = "SELECT LINEAR FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
	if (isetting == 1)
	  setLinearScale (true); 
	else
	  setLinearScale (false); 
  }
  
  // online price
  SQLCommand = "SELECT ONLINEPRICE FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
	if (isetting == 1)
	  show_onlineprice = true;
	else
	  show_onlineprice = false; 
	propScr->setOnlinePrice (show_onlineprice);           
  }
  
  // grid
  SQLCommand = "SELECT GRID FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
	if (isetting == 1)
	  show_grid = true;
	else
	  show_grid = false;  
	propScr->setGrid (show_grid);    
  }
  
  // chart style
  SQLCommand = "SELECT CHARTSTYLE FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
    setChartStyle (isetting);
  }  
    
  // frame width
  SQLCommand = "SELECT FRAMEWIDTH FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
    framewidth = isetting;  
    
  // linecolor
  SQLCommand = "SELECT LINECOLOR FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
    linecolor = QColor ((QRgb) isetting);     
    propScr->setLineColor (linecolor);
  }
  
  // barcolor
  SQLCommand = "SELECT BARCOLOR FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
    barcolor = QColor ((QRgb) isetting);     
    propScr->setBarColor (barcolor);
  }
  
  // forecolor
  SQLCommand = "SELECT FORECOLOR FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
    forecolor = QColor ((QRgb) isetting);
    textcolor = gridcolor = framecolor = forecolor;
    propScr->setForeColor (forecolor);
  }
  
  // backcolor
  SQLCommand = "SELECT BACKCOLOR FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
  {
    backcolor = QColor ((QRgb) isetting);     
    propScr->setBackColor (backcolor);
  }
  
  // startbar
  SQLCommand = "SELECT STARTBAR FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &isetting, NULL);
  if (rc == SQLITE_OK)
    xstartbar = isetting;  
  
  // excess drag width
  SQLCommand = "SELECT EDW FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) &rsetting, NULL);
  if (rc == SQLITE_OK)
    xexcess_drag_width =  rsetting;
  
  // time frame  
  SQLCommand = "SELECT TIMEFRAME FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_chart_setting, (void *) csetting, NULL);
  if (rc == SQLITE_OK)
    currenttf = QString::fromUtf8 (csetting);  
  
  scene->setBackgroundBrush (backcolor);
  
  if (backcolor.rgb () < forecolor.rgb ())
    cursorcolor = 
      QColor ((QRgb)
      (forecolor.rgb () + (qAbs (backcolor.rgb () - forecolor.rgb ()) / 4)));
  else 
  if (backcolor.rgb () > forecolor.rgb ())
    cursorcolor = 
      QColor ((QRgb)
      (forecolor.rgb () - (qAbs (backcolor.rgb () - forecolor.rgb ()) / 4)));
  ruller_cursor->setDefaultTextColor (cursorcolor);      
  
  changeForeColor (forecolor);
}

/// Mm
// manage objects
void
QTAChartCore::manageObjects (void)
{
  events_enabled = false;
  hideAllItems ();
  expandicon =
    QIcon (QString::
           fromLocal8Bit (":/png/images/icons/PNG/back_alt.png"));
  expandBtn->setIcon (expandicon);
  expandBtn->setStyleSheet 
  ("background: transparent;color: white;font: 11px;");
  expandBtn->setToolTip (TOOLTIP + "Back (Alt+Z)</span>");
  prxobjectsScr->resize (width, height - 40);
  expandBtn->setVisible (true);
  prxobjectsScr->setVisible (true);	  
}

/// Nn
/// Oo
/// Pp
/// Qq
/// Rr
// restore the bottom text
void
QTAChartCore::restoreBottomText ()
{
  QFont font;
  
  font = bottom_text->font ();
  font.setPixelSize (8 + FONT_SIZE_PAD);
  bottom_text->setFont (font);	
}

/// Ss

// subchart button slot
void
QTAChartCore::SCBtn_clicked ()
{
  SubChartButton *btn;
  btn = reinterpret_cast <SubChartButton *> (QObject::sender());	
  if (btn->objectName () == "Close")
  {
    deleteObject (btn->getOwner ());
    return;
  }  
  else  
  if (btn->objectName () == "Edit")  
  {
	bool modrslt;	
	QTACObject *object;
  	events_enabled = false;	
  	object = btn->getOwner ();
	modrslt = object->modifyIndicator ();
	if (modrslt == false)
	  deleteObject (object);
  } 
  events_enabled = true;  
  draw ();
}

// save settings
void
QTAChartCore::saveSettings ()
{
  TemplateManagerDialog *tmpldlg;	
  QString SQLCommand = "";
  int rc;
  
  tmpldlg = new TemplateManagerDialog;
  tmpldlg->setReferenceChart (parent ());
  
  SQLCommand += "BEGIN;";
  SQLCommand.append ('\n');
  SQLCommand += "DELETE FROM chart_settings WHERE KEY = '" + SymbolKey + "';";
  SQLCommand.append ('\n');
  SQLCommand += "INSERT INTO chart_settings ";
  SQLCommand += "(key, volume, linear, chartstyle, timeframe, onlineprice,";
  SQLCommand += " linecolor, barcolor, framewidth, startbar, edw, ";  
  SQLCommand += " forecolor, backcolor, grid) ";
  SQLCommand += "VALUES ('"+ SymbolKey + "',";
  
  // volume
  if (show_volumes)
    SQLCommand += "1, ";
  else
    SQLCommand += "0, ";
  
  // linear
  if (linear)
    SQLCommand += "1, ";
  else
    SQLCommand += "0, ";
  
  // chartstyle
  SQLCommand += QString::number (chart_style) + ", ";
  
  // timeframe
  SQLCommand += "'" + currenttf + "', ";
  
  // online price
  if (show_onlineprice)
    SQLCommand += "1, ";
  else
    SQLCommand += "0, ";
  
  // line color
  SQLCommand += QString::number ((qreal) linecolor.rgb (), 'f', 0) + ", ";
   
  // bar color
  SQLCommand += QString::number ((qreal) barcolor.rgb (), 'f', 0) + ", ";  
  
  // frame width
  SQLCommand += QString::number (framewidth) + ", ";  
  
  // startbar
  SQLCommand += QString::number (*startbar) + ", ";  
  
   // excess_drag_width
  SQLCommand += QString::number (*excess_drag_width, 'f', 1) + ", ";  
  
  // foreground color
  SQLCommand += QString::number ((qreal) forecolor.rgb (), 'f', 0) + ", ";
   
  // background color
  SQLCommand += QString::number ((qreal) backcolor.rgb (), 'f', 0) + ", ";  
  
  // grid
  if (show_grid)
    SQLCommand += "1); ";
  else
    SQLCommand += "0); ";
  
  SQLCommand.append ('\n');    
  
  SQLCommand += "DROP TABLE IF EXISTS template_" + SymbolKey + ";";
  SQLCommand += "CREATE TABLE template_" + SymbolKey + " AS ";
  SQLCommand += "SELECT * FROM templatemodel;";
  SQLCommand.append ('\n');    
  SQLCommand += tmpldlg->qtachart2sql (QString ("template_") + SymbolKey);
  SQLCommand.append ('\n');
  SQLCommand += "COMMIT;";
  SQLCommand.append ('\n');
  
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
  if (rc != SQLITE_OK)
    setGlobalError(CG_ERR_TRANSACTION, __FILE__, __LINE__);
  
  delete tmpldlg;
}

// set chart's title
void
QTAChartCore::setTitle (QString Title, QString Subtitle)
{
  QString titlestring, subtitlestring;
  QByteArray ba;

  Title.trimmed ();
  Subtitle.trimmed ();
  if (Title == "")
    Title = "Unknown";
  if (Subtitle == "")
    Subtitle = "Unknown";
      
  memset (titletext, 0, STRMAXLEN);
  titlestring = qPrintable (Title);
  ba = titlestring.toLocal8Bit ();
  strncpy (titletext, ba.data (), STRMAXLEN - 1);
  titlestring = QString::fromLocal8Bit (titletext, -1);
  title->setPlainText (TO_UTF8 (titlestring));
  title->setDefaultTextColor (textcolor);
    
  memset (subtitletext, 0, STRMAXLEN);
  subtitlestring = qPrintable (Subtitle);
  subtitlestring = subtitlestring.left (40);
  ba = subtitlestring.toLocal8Bit ();
  strncpy (subtitletext, ba.data (), STRMAXLEN - 1);
  subtitlestring = QString::fromLocal8Bit (subtitletext, -1);
  subtitle->setPlainText (TO_UTF8 (subtitlestring));
  subtitle->setDefaultTextColor (textcolor);
}

// set chart's style
void
QTAChartCore::setChartStyle (int style)
{
  propScr->setChartStyle (style); 
  chart_style = propScr->ChartStyle ();
   	
  if (style == QTACHART_CANDLE)
    typetitle->setPlainText ("Candle");
  else  
  if (style == QTACHART_HEIKINASHI)  
    typetitle->setPlainText ("Heikin-Ashi");
  else 
  if (style == QTACHART_BAR)
    typetitle->setPlainText ("Bar");
  else 
  if (style == QTACHART_LINE)
    typetitle->setPlainText ("Line");
}

// set chart's style
void
QTAChartCore::setLinearScale (bool scale)
{
  propScr->setLinearScale (scale);	
  linear = propScr->LinearScale ();
  
  if (linear)
    scaletitle->setPlainText ("Linear");
  else  
    scaletitle->setPlainText ("Logarithmic");
}

// set the bottom text
void
QTAChartCore::setBottomText (int x)
{
  bottom_text->setPlainText (getBottomText (x));
  bottom_text->setDefaultTextColor (textcolor);
  
  foreach (QTACObject *obj, Object)
  {
	if (obj->type == QTACHART_OBJ_SUBCHART)
	  obj->setDataTitle (x);
  }
}

// set the bottom text to custom string
void
QTAChartCore::setCustomBottomText (QString string)
{
  QFont font;
  
  font = bottom_text->font ();
  font.setPixelSize (10 + FONT_SIZE_PAD);
  bottom_text->setFont (font);
  
  bottom_text->setPlainText (string);
  bottom_text->setDefaultTextColor (textcolor);
  foreach (QTACObject *obj, Object)
  {
	if (obj->type == QTACHART_OBJ_SUBCHART)
	  obj->setDataTitle (chartrightmost - *excess_drag_width);
  }
}

// set the charts properties
void
QTAChartCore::setChartProperties (void)
{
  events_enabled = false;
  hideAllItems ();
  expandicon =
    QIcon (QString::
           fromLocal8Bit (":/png/images/icons/PNG/back_alt.png"));
  expandBtn->setIcon (expandicon);
  expandBtn->setStyleSheet
  ("background: transparent;color: white;font: 11px;");
  expandBtn->setToolTip (TOOLTIP + "Back (Alt+Z)</span>");
  prxpropScr->resize (width, height - 40);
  prxpropScr->setVisible (true);
  expandBtn->setVisible (true);
  propScr->setVolumes (show_volumes);
  propScr->setGrid (show_grid);
  setChartStyle (chart_style);
  setLinearScale (linear);
  propScr->setOnlinePrice (show_onlineprice);
}

// select object to draw
void
QTAChartCore::selectDrawObject (void)
{
  events_enabled = false;
  hideAllItems ();
  prxdrawScr->resize (width, height - 40);
  expandicon =
    QIcon (QString::
           fromLocal8Bit (":/png/images/icons/PNG/back_alt.png"));
  expandBtn->setIcon (expandicon);
  expandBtn->setStyleSheet
  ("background: transparent;color: white;font: 11px;");
  expandBtn->setToolTip  (TOOLTIP + "Back (Alt+Z)</span>");
  expandBtn->setVisible (true);
  prxdrawScr->setVisible (true);
}

// select function to apply
void
QTAChartCore::selectFunction (void)
{
  events_enabled = false;
  hideAllItems ();
  prxfunctionScr->resize (width, height - 40);
  expandicon =
    QIcon (QString::
           fromLocal8Bit (":/png/images/icons/PNG/back_alt.png"));
  expandBtn->setIcon (expandicon);
  expandBtn->setStyleSheet
  ("background: transparent;color: white;font: 11px;");
  expandBtn->setToolTip (TOOLTIP + "Back (Alt+Z)</span>");
  expandBtn->setVisible (true);
  prxfunctionScr->setVisible (true);
}

// set the price cursor
void
QTAChartCore::setRullerCursor (int y)
{
  QFont tfont;
  QString pricestr;
  qreal price;
  
  if (y <= charttopmost)
    y = charttopmost;
  
  if (y >= chartbottomost)
    y = chartbottomost;
    
  price = priceOnY (this, y);
  if (Q_UNLIKELY (price < 0))
    return;
  pricestr = QString ("%1").arg (price, 10, 'f', 2);
  ruller_cursor_y = y;
  ruller_cursor->setPos (ruller_cursor_x, y - 10);
  ruller_cursor->setPlainText (pricestr);
}

// show all chart's objects
void
QTAChartCore::showAllItems (void)
{
  QList < QGraphicsItem * >sceneitems;
  QGraphicsItem *item;

  sceneitems = scene->items ();
  if (Q_UNLIKELY (sceneitems.size () == 0))
    return;
  foreach (item, sceneitems) 
    item->setVisible (true);
}

// show help
void
QTAChartCore::showHelp (void)
{
  events_enabled = false;
  hideAllItems ();
  prxhelpScr->resize (width, height - 40);
  expandicon =
    QIcon (QString::
           fromLocal8Bit (":/png/images/icons/PNG/back_alt.png"));
  expandBtn->setIcon (expandicon);
  expandBtn->setStyleSheet 
  ("background: transparent;color: white;font: 11px;");
  expandBtn->setToolTip (TOOLTIP + "Back (Alt+Z)</span>");
  expandBtn->setVisible (true);
  prxhelpScr->setVisible (true);
}

// show data
void
QTAChartCore::showData (void)
{
  events_enabled = false;
  hideAllItems ();
  prxdataScr->resize (width, height - 40);
  expandicon =
    QIcon (QString::
           fromLocal8Bit (":/png/images/icons/PNG/back_alt.png"));
  expandBtn->setIcon (expandicon);
  expandBtn->setStyleSheet 
  ("background: transparent;color: white;font: 11px;");
  expandBtn->setToolTip (TOOLTIP + "Back (Alt+Z)</span>");
  expandBtn->setVisible (true);
  prxdataScr->setVisible (true);
}


/// Tt
// time frame button slot
void
QTAChartCore::tfBtn_clicked ()
{
  QString tfsymbol;
  QToolButton *btn;	
  
  btn = (QToolButton *) QTAChartCore::sender ();
  tfsymbol = btn->text ();
 
  for (qint32 counter = 0, maxcounter = TIMEFRAME.size (); 
       counter < maxcounter; counter ++)
  {
	if (TIMEFRAME.at(counter).TFSymbol == tfsymbol)
	{
	   QString title;	
	   HLOC = &TIMEFRAME[counter].HLOC;
       HEIKINASHI = &TIMEFRAME[counter].HEIKINASHI; 
       startbar = &(TIMEFRAME[counter].TFStartBar);	
       excess_drag_width = &TIMEFRAME[counter].TFExcess_Drag_Width;
       title = Symbol + " - " + TIMEFRAME[counter].TFName;
       setTitle (title, QString::fromUtf8 (subtitletext));
       currenttf = TIMEFRAME.at(counter).TFName;
    }  
  }
  
  reloaded = true;
  draw ();
}	
/// Uu
/// Vv
/// Ww
/// Xx
/// Yy
/// Zz
