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

#include <limits>
#include <QtCore/qmath.h>
#include <QtGlobal>
#include "qtachart_core.h"

// constructor of line edge class
LineEdge::LineEdge ()
{
  pad = 0;
  txtdirection = 0;
  sequence = 1;
  pricetxt = NULL;
}

// destructor of line edge class
LineEdge::~LineEdge ()
{
  
}

// constructor of close button on subcharts
SubChartButton::SubChartButton (QTACObject *object)
{
  owner = object;
  chartdata = owner->chartdata;
}

// destructor
SubChartButton::~SubChartButton ()
{
  
}

// get owner object
QTACObject *
SubChartButton::getOwner ()
{
  return owner;	
}

// constuctor for objects on chart or subcharts
QTACObject::QTACObject (void *data, QTAChartObjectType objtype)
{
  QTAChartCore *core = reinterpret_cast <QTAChartCore *> (data);

  chartdata = data;
  type = objtype;
  parentObject = NULL;
  core->Object += this;
  QTACObject_constructor_common ();
}

// constructor
QTACObject::QTACObject (QTACObject *parentsubchart, QTAChartObjectType objtype)
{
  parentObject = parentsubchart;
  parentObject->children += this;
  chartdata = parentObject->chartdata;
  type = objtype;
  QTACObject_constructor_common ();
}

// constructor's common function
void
QTACObject::QTACObject_constructor_common ()
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  QFont font;
  
  qRegisterMetaType<RTPrice>("RTPrice");
  connect(this ,SIGNAL(updateOnlinePrice (RTPrice)), 
          this, SLOT (updateOnlinePriceSlot (RTPrice)));
  
  subchart_dec = 1;
  quantum = 0;
  scene = core->scene;
  deleteit = false;
  onlineprice = false;
  filter = NULL;
  dataset = NULL;
  valueset = NULL;
  TAfunc = NULL;
  TAfunc2 = NULL;
  paramDialog = NULL;
  ITEMS = NULL;
  visibleitems = 0;  
  ITEMSsize = 0;
  valuesetsize = 0;
  lastperiod = 0;
  X1 = Y1 = X2 = Y2 = 0;
  trailerCandleText = "";
  price = 0;
  
  if (parentObject == NULL)
    setParent (core);
  else
    setParent (parentObject);
  
  bottomline = NULL;
  text = NULL;
  hvline = NULL;
  titlestr = "";
  datastr = "";
  
  title = new QGraphicsTextItem;
  font = title->font ();
  font.setPixelSize (8 + FONT_SIZE_PAD);
  font.setFamily (DEFAULT_FONT_FAMILY);
  font.setWeight (QFont::DemiBold);
  title->setFont (font);
  scene->addItem (title);
    
  closeBtn = NULL;
  editBtn = NULL;
  if (type == QTACHART_OBJ_SUBCHART)
  {
    core->nsubcharts ++;
    closeBtn = core->addSCB (this, "Close");
    closeBtn->setFixedSize (QSize (22, 22));
    closeicon =
      QIcon (QString::fromLocal8Bit (":/png/images/icons/PNG/X.png"));
    closeBtn->setIcon (closeicon);
    closeBtn->setIconSize (QSize (16, 16));
    closeBtn->setFocusPolicy (Qt::NoFocus);
    closeBtn->setStyleSheet ("background: transparent;color: white;font: 11px;");
    prxcloseBtn = scene->addWidget (closeBtn, Qt::Widget);
    
    editBtn = core->addSCB (this, "Edit");
    editBtn->setFixedSize (QSize (30, 16));
    editBtn->setText ("EDIT");
    editBtn->setFocusPolicy (Qt::NoFocus);
    editBtn->setStyleSheet ("background: transparent; border: 1px solid transparent;border-color: darkgray;color: white; font: 9px;");
    prxeditBtn = scene->addWidget (editBtn, Qt::Widget);
    
    if (paramDialog == NULL)
      editBtn->setVisible (false);
  }
  
  if (type == QTACHART_OBJ_LINE ||
      type == QTACHART_OBJ_FIBO)
  {
	Edge += new LineEdge;
	Edge += new LineEdge;  
	Edge[0]->pricetxt = new QGraphicsTextItem;
	Edge[1]->pricetxt = new QGraphicsTextItem;
	Edge[0]->pricetxt->setParent (this);
	Edge[1]->pricetxt->setParent (this);
	Edge[0]->pricetxt->installEventFilter (new QTACObjectEventFilter (Edge[0]->pricetxt));
	Edge[1]->pricetxt->installEventFilter (new QTACObjectEventFilter (Edge[1]->pricetxt));
	Edge[0]->pricetxt->setAcceptHoverEvents (true);
	Edge[1]->pricetxt->setAcceptHoverEvents (true);
	Edge[0]->pricetxt->setFont (font);
	Edge[1]->pricetxt->setFont (font);
	Edge[0]->pricetxt->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
	Edge[1]->pricetxt->setToolTip ( "<span style=\"font: 9px;color:black;\">Double click to edit</span>");
  }
  
  if (type == QTACHART_OBJ_FIBO)
  {
	FiboLevelPrc << 0 << 0.236 << 0.382 << 0.5 << 0.618 << 0.764 << 1;	  
    for (qint32 counter = 0, maxcounter = FiboLevelPrc.size (); 
         counter < maxcounter; counter ++)
    {
	  QGraphicsTextItem *textitem;
	  FiboLevel += scene->addLine (0, 0, 0, 0);
	  textitem = new QGraphicsTextItem;
	  textitem->setParent (this);
	  FiboLevelLbl += textitem;
	  scene->addItem (textitem);
	  textitem = new QGraphicsTextItem;
	  textitem->setParent (this);
	  FiboLevelPrcLbl += textitem;
	  scene->addItem (textitem);
    }
  }
  
  if (type != QTACHART_OBJ_LABEL && type != QTACHART_OBJ_TEXT)
    text = title;
}

// destructor
QTACObject::~QTACObject ()
{
  QPen pen;	
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  
  if (type == QTACHART_OBJ_SUBCHART)
  {
    closeBtn->setVisible (false);
    scene->removeItem (prxcloseBtn);
    core->garbageQWidget += closeBtn;
    core->CloseSCB.remove (core->CloseSCB.indexOf (closeBtn)); 
    editBtn->setVisible (false);
    scene->removeItem (prxeditBtn);
    core->garbageQWidget += editBtn;
    core->CloseSCB.remove (core->CloseSCB.indexOf (editBtn)); 
  }  
  
  if (type == QTACHART_OBJ_FIBO)
  {
	for (qint32 counter = 0; counter < FiboLevelPrc.size (); counter ++)
	{
	  FiboLevel[counter]->setVisible (false);	
	  scene->removeItem (FiboLevel[counter]);
	  scene->removeItem (FiboLevelLbl[counter]);
	  scene->removeItem (FiboLevelPrcLbl[counter]);
	  delete FiboLevel[counter];	
    }  
    FiboLevel.clear ();
  }  
  
  foreach (const LineEdge *edge, Edge)
    delete edge;
  
  if (hvline != NULL)
  {
	 scene->removeItem (hvline);
	 delete hvline; 
	 hvline = NULL;
  }	  
  
  if (bottomline != NULL)
  {
    scene->removeItem (bottomline);
    delete bottomline;
    bottomline = NULL;
  }
  
  if (scene->items().contains (title))
    scene->removeItem (title);
  delete title; 
  if (text == title)
    text = NULL; 
  title = NULL;
  
  if (text != NULL)
  {
	scene->removeItem (text); 
    delete text;
    text = NULL;
  }

  clearITEMS ();
  deleteITEMS ();
  delete[] ITEMS;

  if (children.size () != 0)
    foreach (const QTACObject *child, children)
      delete child;

  if (type == QTACHART_OBJ_SUBCHART)
    core->nsubcharts -= subchart_dec;
  
  if (valueset != NULL)
    delete valueset;
  
  if (paramDialog != NULL)
    delete paramDialog;  
  
  core->clearITEMS ();
}

// update online price signal emittion and slot 
void 
QTACObject::emitUpdateOnlinePrice (RTPrice rtprice)
{
  emit updateOnlinePrice (rtprice);	
}

void 
QTACObject::updateOnlinePriceSlot (RTPrice rtprice)
{
  QString str = rtprice.price + " " + rtprice.change + " " + rtprice.prcchange;	
  if (str == text->toPlainText ())
    return;
  
  rtprice.prcchange.replace ("%", " ");
  if (rtprice.prcchange.toFloat () > 0)
    text->setDefaultTextColor (Qt::green);  
  else  
  if (rtprice.prcchange.toFloat () < 0)
    text->setDefaultTextColor (Qt::red);  
  else
    text->setDefaultTextColor (forecolor);  
    
  text->setPlainText (str);
}

// change foreground color
void 
QTACObject::changeForeColor (QColor color)
{
  QPen pen;
  QTACObject *child;
  	
  if (type != QTACHART_OBJ_SUBCHART)
    return;
  
  forecolor = color;
  title->setDefaultTextColor (forecolor);   
  
  pen.setColor (forecolor);
  foreach (child, children)
  {
	if (child->type == QTACHART_OBJ_VBARS)
	  child->forecolor = forecolor;
	
	if (child->type == QTACHART_OBJ_HLINE)
	{
	  child->forecolor = forecolor;  
	  child->hvline->setPen (forecolor);
	  child->title->setDefaultTextColor (forecolor);
	}  
  }
  
  if (editBtn != NULL)
    editBtn->setStyleSheet (
      QString ("background: transparent; border: 1px solid transparent; \
                border-color: %1;color: %1; font: 9px;").arg (forecolor.name ()));  
}

// clear chart's ITEMS
void
QTACObject::clearITEMS ()
{
   for (qint32 counter = 0; counter < ITEMSsize; counter ++)
   	 ITEMS[counter]->setSize (0, 0, 0, 0);
}

// delete ITEMS
void
QTACObject::deleteITEMS ()
{
  for (qint32 counter = 0; counter < ITEMSsize; counter ++)
    delete ITEMS[counter];
  
  ITEMSsize = 0;  
}

// get parent
QTACObject *
QTACObject::getParentObject ()
{
  return parentObject;
}

// get title
QString
QTACObject::getTitle ()
{
  return titlestr;
}

// get trailer candle title
QString 
QTACObject::getTrailerCandleText (void) 
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[0]->trailerCandleText;
	else
	  return QString ("");    
  }	
  return trailerCandleText; 
}

QString 
QTACObject::getTrailerCandleText2 (void) 
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[1]->trailerCandleText;
	else
	  return QString ("");    
  }	
  return QString (""); 
}

// get color
QColor  
QTACObject::getColor (void)
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  	return hvline->pen ().color ();  
  	
  return text->defaultTextColor (); 
}

// get the price 
qreal   
QTACObject::getPrice (void) 
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[0]->price;
	else
	  return 0.0;    
  }	
  return price; 
}

qreal   
QTACObject::getPrice2 (void) 
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[1]->price;
	else
	  return 0.0;    
  }	
  return 0.0; 
}

// get the edge's pad
qreal   
QTACObject::getPad (void) 
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[0]->pad;
	else
	  return 0.0;    
  }	
  return 0.0; 
}

qreal   
QTACObject::getPad2 (void) 
{ 
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[1]->pad;
	else
	  return 0.0;    
  }	
  return 0.0; 
}

// get text's direction of label
qint16  
QTACObject::getTxtDirection ()
{
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[0]->txtdirection;
	else
	  return 0.0;    
  }	
  return 0.0;	
}

qint16  
QTACObject::getTxtDirection2 ()
{
  if (type == QTACHART_OBJ_LINE || type == QTACHART_OBJ_FIBO)
  {
	if (Edge.size () > 0)
	  return Edge[1]->txtdirection;
	else
	  return 0.0;    
  }	
  return 0.0;	
}

// calculate and return value set
DataSet
QTACObject::valueSet ()
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);	
  
  // calculate the dataset using tafunc
  if (TAfunc != NULL)
  {
	if (Q_UNLIKELY (valueset == NULL))
	{
	  valueset = TAfunc (dataset, period);
	  valuesetsize = dataset->size ();
	}  
	else
	{
      if (Q_UNLIKELY (valuesetsize != dataset->size ()) ||
          lastperiod != period)
      {	
        delete valueset;
        valueset = NULL;   
        valueset = TAfunc (dataset, period);
        valuesetsize = dataset->size ();
        lastperiod = period;
      }  
    }  
  }  
  else
  if (TAfunc2 != NULL)
  {
	if (Q_UNLIKELY (valueset == NULL))
	{
	  valueset = TAfunc2 (core->HLOC, period); 
	  valuesetsize = core->HLOC->size ();
	}  
	else
	{
      if (Q_UNLIKELY (valuesetsize != dataset->size ())
          || lastperiod != period)
      {	
        delete valueset;
        valueset = NULL;   
        valueset = TAfunc2 (core->HLOC, period); 
        valuesetsize = core->HLOC->size ();
        lastperiod = period;
      }  
    }  
  }
  
  if (children.size () > 0)
  {
	QTACObject *child;	
	foreach (child, children)
	{
	  child->setPeriod (period);
	  child->valueSet ();	
    }  
  }
 
  return valueset;	
}

// modify technical indicator;
bool 
QTACObject::modifyIndicator ()
{
  int xperiod = period;	
  
  paramDialog->exec ();
  if (deleteit == true)
  	return false;  
  
  if (xperiod != period)
    valueSet ();
    
  return true;
}

// set the price level for a horizontal line 
void 
QTACObject::setHLine (QGraphicsLineItem *line, qreal value)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);	
  QRectF rectf;
  qreal y;
  
  if (deleteit)
    return;
  
  if (parentObject == NULL)
    title->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
    
  if (line == NULL)
  {
    line = new QGraphicsLineItem ();	
    line->setVisible (true);
    line->setLine (0, 0, 0, 0);
    line->setPen (QPen (core->gridcolor));
    scene->addItem (line);
  }  
  
  price = value;	
  hvline = line;
  if (parentObject == NULL)
  {
    title->setParent (this);
  
    if (filter == NULL)
    {
      filter = new QTACObjectEventFilter (title);  
	  title->installEventFilter (filter);
	  title->setAcceptHoverEvents (true);
    }

    titlestr = QString ("%1").arg (price, 10, 'f', 2);
    title->setPlainText (titlestr);
    title->setDefaultTextColor (hvline->pen().color ());
    titlestr = "Horizontal Line " + QString ("%1").arg (price, 10, 'f', 2);
    
    y = yOnPrice (core, price);
    rectf = title->boundingRect();
    hvline->setLine (core->chartleftmost, y, core->chartrightmost - rectf.width (), y);
    title->setPos (core->chartrightmost - rectf.width (), y - 10);
    return;
  }
  
  titlestr = QString ("%1").arg (value, 10, 'f', 2);
  title->setPlainText (titlestr);
  title->setDefaultTextColor (core->forecolor);
  titlestr = "Horizontal line " + QString ("%1").arg (price, 10, 'f', 2);
}

// set the price level for a horizontal line 
void 
QTACObject::setVLine (QGraphicsLineItem *line)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);	
  QRectF rectf;
  qreal x;
  
  if (deleteit)
    return;
  
  hvline = line;
  title->setParent (this);
  
  if (parentObject == NULL)
    title->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
  
  if (filter == NULL)
  {
    filter = new QTACObjectEventFilter (title);  
	title->installEventFilter (filter);
	title->setAcceptHoverEvents (true);
  }
  
  x = line->boundingRect().x ();
  trailerCandleText = core->getBottomText ((int) x);
  titlestr = trailerCandleText.mid (3, 20);
  title->setPlainText (titlestr);
  title->setDefaultTextColor (hvline->pen().color ());
  titlestr = "Vertical line " + titlestr;
  
  rectf = title->boundingRect();
  hvline->setLine (x, core->charttopmost, x, core->chartbottomost - rectf.height () + 3);
  title->setPos (x - (rectf.width () / 2), core->chartbottomost - 12);
}

void 
QTACObject::setVLine (QGraphicsLineItem *line, QString text)
{
  if (deleteit)
    return;
  
  hvline = line;
  title->setParent (this);
  
  if (parentObject == NULL)
    title->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
  
  if (filter == NULL)
  {
    filter = new QTACObjectEventFilter (title);  
	title->installEventFilter (filter);
	title->setAcceptHoverEvents (true);
  }
  
  trailerCandleText = text;
  titlestr = trailerCandleText.mid (3, 20);
  title->setPlainText (titlestr);
  title->setDefaultTextColor (hvline->pen().color ());
  titlestr = "Vertical line " + titlestr;
}

// set the edges of a trend line 
void 
QTACObject::setTLine (QGraphicsLineItem *sline)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  QLineF line;
  QRectF rectf;
  QString prcstr;
  LineEdge *edge;
  
  if (Q_UNLIKELY (deleteit))
    return;
  
  title->setParent (this);
  hvline = sline;
  line = hvline->line ();
    
  // edge 1
  edge = Edge[0];
  edge->price = priceOnY (core, (int) line.y1 ());
  edge->trailerCandleText = core->getBottomText ((int) line.x1 ());
  if (line.x1 () < line.x2 ())
    edge->txtdirection = 0;
  else
    edge->txtdirection = 1;
  edge->sequence = 1;
  prcstr = QString::number (edge->price, 'f', 2);
  
  edge->pricetxt->setFont (title->font ());
  edge->pricetxt->setPlainText (prcstr);
  edge->pricetxt->setDefaultTextColor (hvline->pen().color ());
  titlestr = "Trend line " + prcstr + " - ";
  
  // edge 2
  edge = Edge[1];
  edge->price = priceOnY (core, (int) line.y2 ());
  edge->trailerCandleText = core->getBottomText ((int) line.x2 ());
  if (line.x1 () < line.x2 ())
    edge->txtdirection = 1;
  else
    edge->txtdirection = 0;
    
  edge->sequence = 2;
  prcstr = QString::number (edge->price, 'f', 2);
  edge->pricetxt->setFont (title->font ());
  edge->pricetxt->setPlainText (prcstr);
  edge->pricetxt->setDefaultTextColor (hvline->pen().color ());
  titlestr += prcstr;
  
  hvline->setLine (line.x1 (), line.y1 (), line.x2 (), line.y2 ());
  foreach (edge, Edge)
  {
	qreal x = 0, y = 0;  
    rectf = edge->pricetxt->boundingRect();
    
    if (edge->sequence == 1)
    {
	  X1 = x = line.x1 ();
	  Y1 = y = line.y1 ();
	}
	else
	if (edge->sequence == 2)
	{
	  X2 = x = line.x2 ();
	  Y2 = y = line.y2 (); 	
    }  	
    
    if (edge->trailerCandleText.size () < 4)
    {
      edge->pad = core->HLOC->size () - *core->startbar; 
      edge->pad = (core->framewidth * 1.5 * edge->pad);
      if (edge->txtdirection == 1)
      {
        edge->pad = core->chartrightmost - *core->excess_drag_width;
      }
      else
      {
		edge->pad =  edge->pad + *core->excess_drag_width;
		edge->pad =  core->chartrightmost - edge->pad;
      }  
      edge->pad -= x;
	  edge->pad = qAbs (edge->pad) / (core->framewidth * 1.5);  
    }	  	
    
    if (edge->txtdirection == 0)
      x -=  (rectf.width () + 2);
    else
      x += 2;
    
    if (!scene->items ().contains (edge->pricetxt))
      scene->addItem (edge->pricetxt);
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

void 
QTACObject::setTLine (QGraphicsLineItem *sline, LineEdge e1, LineEdge e2)
{
  if (Q_UNLIKELY (deleteit))
    return;
  
  title->setParent (this);
  hvline = sline;	
  
  Edge[0]->price = e1.price;
  Edge[0]->pad = e1.pad;
  Edge[0]->txtdirection = e1.txtdirection;
  Edge[0]->trailerCandleText = e1.trailerCandleText;
  Edge[0]->sequence = e1.sequence;
  Edge[0]->pricetxt->setPlainText (QString::number (e1.price, 'f', 2));
  Edge[0]->pricetxt->setDefaultTextColor (hvline->pen().color ());
  scene->addItem (Edge[0]->pricetxt);
  
  Edge[1]->price = e2.price;
  Edge[1]->pad = e2.pad;
  Edge[1]->txtdirection = e2.txtdirection;
  Edge[1]->trailerCandleText = e2.trailerCandleText;
  Edge[1]->sequence = e2.sequence;
  Edge[1]->pricetxt->setPlainText (QString::number (e2.price, 'f', 2));
  Edge[1]->pricetxt->setDefaultTextColor (hvline->pen().color ());
  scene->addItem (Edge[1]->pricetxt);
  
  titlestr = "Trend line " + QString::number (e1.price, 'f', 2) + " - ";
  titlestr += QString::number (e2.price, 'f', 2);
}

// set the edges of a fibo line 
void 
QTACObject::setFibo (QGraphicsLineItem *sline)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  QLineF line;
  QRectF rectf;
  QString prcstr;
  LineEdge *edge;
  
  if (Q_UNLIKELY (deleteit))
    return;
  
  title->setParent (this);
  hvline = sline;
  line = hvline->line ();
    
  // edge 1
  edge = Edge[0];
  edge->price = priceOnY (core, (int) line.y1 ());
  edge->trailerCandleText = core->getBottomText ((int) line.x1 ());
  edge->txtdirection = 0;
  edge->sequence = 1;
  prcstr = line.y1 () < line.y2 () ? "1.0" : "0.0";
  edge->pricetxt->setFont (title->font ());
  edge->pricetxt->setPlainText (prcstr);
  edge->pricetxt->setDefaultTextColor (hvline->pen().color ());
  
  // edge 2
  edge = Edge[1];
  edge->price = priceOnY (core, (int) line.y2 ());
  edge->trailerCandleText = core->getBottomText ((int) line.x2 ());
  edge->txtdirection = 0;
  edge->sequence = 2;
  prcstr = line.y1 () < line.y2 () ? "0.0" : "1.0";
  edge->pricetxt->setFont (title->font ());
  edge->pricetxt->setPlainText (prcstr);
  edge->pricetxt->setDefaultTextColor (hvline->pen().color ());
  
  hvline->setLine (line.x1 (), line.y1 (), line.x2 (), line.y2 ());
  foreach (edge, Edge)
  {
	qreal x = 0, y = 0;  
    rectf = edge->pricetxt->boundingRect();
    
    if (edge->sequence == 1)
    {
	  X1 = x = line.x1 ();
	  Y1 = y = line.y1 ();
	}
	else
	if (edge->sequence == 2)
	{
	  X2 = x = line.x2 ();
	  Y2 = y = line.y2 (); 	
    }  	
    
    if (edge->trailerCandleText.size () < 4)
    {
      edge->pad = core->HLOC->size () - *core->startbar; 
      edge->pad = (core->framewidth * 1.5 * edge->pad);
      edge->pad = core->chartrightmost - *core->excess_drag_width;
      edge->pad -= x;
	  edge->pad = qAbs (edge->pad) / (core->framewidth * 1.5);  
    }	  	
    
    if (edge->txtdirection == 0)
      x -=  (rectf.width () + 2);
    else
      x += 2;
    
    if (!scene->items ().contains (edge->pricetxt))
      scene->addItem (edge->pricetxt);
        
    edge->pricetxt->setPos ( x, y - 12);
    edge->pricetxt->setVisible (true);
  } 
  
  FiboLevelPrice.clear ();
  for (qint32 counter = 0, maxcounter = FiboLevelPrc.size (); 
       counter < maxcounter; counter ++)
  {
	qreal price, x;  
	price = qAbs (Edge[0]->price - Edge[1]->price);
	price *= FiboLevelPrc[counter];
	if (Edge[0]->price < Edge[1]->price)
	  price += Edge[0]->price;
	else
	  price += Edge[1]->price;
	FiboLevelPrice += price;
	
	prcstr = QString::number (price, 'f', 2);	
	FiboLevelPrcLbl[counter]->setFont (title->font ());
    FiboLevelPrcLbl[counter]->setPlainText (prcstr);
    FiboLevelPrcLbl[counter]->setDefaultTextColor (hvline->pen().color ());
	rectf = FiboLevelPrcLbl[counter]->boundingRect();
    x =  core->chartrightmost - (rectf.width () + 2);
	((QGraphicsLineItem *)FiboLevel[counter])->setPen (hvline->pen ());
	((QGraphicsLineItem *)FiboLevel[counter])->setLine (line.x1 (), 
	                                             yOnPrice (core, price),
	                                         x, yOnPrice (core, price)); 
	FiboLevelPrcLbl[counter]->setPos (x + 1, yOnPrice (core, price) - 12);
    FiboLevelLbl[counter]->setVisible (true);
	                                         
	if (counter > 0 && counter < FiboLevelPrc.size () - 1)
	{
	  prcstr = QString::number (FiboLevelPrc[counter], 'f', 2);	
	  FiboLevelLbl[counter]->setFont (title->font ());
      FiboLevelLbl[counter]->setPlainText (prcstr);
      FiboLevelLbl[counter]->setDefaultTextColor (hvline->pen().color ());
      rectf = FiboLevelLbl[counter]->boundingRect();
      x =  line.x1 () - (rectf.width () + 2);
      FiboLevelLbl[counter]->setPos (x, yOnPrice (core, price) - 12);
      FiboLevelLbl[counter]->setVisible (true);
    }                            
  }
  titlestr = "Fibonacci";
}

void 
QTACObject::setFibo (QGraphicsLineItem *sline, LineEdge e1, LineEdge e2)
{
  QString prcstr;
  	
  if (Q_UNLIKELY (deleteit))
    return;
  
  title->setParent (this);
  hvline = sline;	
  
  Edge[0]->price = e1.price;
  Edge[0]->pad = e1.pad;
  Edge[0]->txtdirection = e1.txtdirection;
  Edge[0]->trailerCandleText = e1.trailerCandleText;
  Edge[0]->sequence = e1.sequence;
  Edge[0]->pricetxt->setPlainText (QString (hvline->line ().y1 () < hvline->line ().y2 () ? "1.0" : "0.0"));
  Edge[0]->pricetxt->setDefaultTextColor (hvline->pen().color ());
  scene->addItem (Edge[0]->pricetxt);
  
  Edge[1]->price = e2.price;
  Edge[1]->pad = e2.pad;
  Edge[1]->txtdirection = e2.txtdirection;
  Edge[1]->trailerCandleText = e2.trailerCandleText;
  Edge[1]->sequence = e2.sequence;
  Edge[1]->pricetxt->setPlainText (QString(hvline->line ().y1 () < hvline->line ().y2 () ? "0.0" : "1.0"));
  Edge[1]->pricetxt->setDefaultTextColor (hvline->pen().color ());
  scene->addItem (Edge[1]->pricetxt);
  
  FiboLevelPrice.clear ();
  for (qint32 counter = 0, maxcounter = FiboLevelPrc.size (); 
       counter < maxcounter; counter ++)
  {
	qreal price;  
	price = qAbs (Edge[0]->price - Edge[1]->price);
	price *= FiboLevelPrc[counter];
	if (Edge[0]->price < Edge[1]->price)
	  price += Edge[0]->price;
	else
	  price += Edge[1]->price;
	FiboLevelPrice += price;
	
	prcstr = QString::number (price, 'f', 2);	
	FiboLevelPrcLbl[counter]->setFont (title->font ());
    FiboLevelPrcLbl[counter]->setPlainText (prcstr);
    FiboLevelPrcLbl[counter]->setDefaultTextColor (hvline->pen().color ());
	((QGraphicsLineItem *)FiboLevel[counter])->setPen (hvline->pen ());
		                                         
	if (counter > 0 && counter < FiboLevelPrc.size () - 1)
	{
	  prcstr = QString::number (FiboLevelPrc[counter], 'f', 2);	
	  FiboLevelLbl[counter]->setFont (title->font ());
      FiboLevelLbl[counter]->setPlainText (prcstr);
      FiboLevelLbl[counter]->setDefaultTextColor (hvline->pen().color ());
    }                            
  }
  
  titlestr = "Fibonacci";	
}

// set text of label or text objects
void
QTACObject::setText (QGraphicsTextItem *textitem, qreal x, qreal y)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qreal height, width;
  
  X1 = x;
  Y1 = y;
  
  text = textitem;
  forecolor = text->defaultTextColor ();
  text->setParent (this);
  text->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
  
  if (filter == NULL)
  {
    filter = new QTACObjectEventFilter (text);  
	text->installEventFilter (filter); 
    text->setAcceptHoverEvents (true);
  }  
  
  height = qAbs (core->chartbottomost - core->charttopmost);
  width =  qAbs (core->chartrightmost - core->chartleftmost);
  price = priceOnY (core, (int) y);
  x -= core->chartleftmost;
  y -= core->charttopmost;
  relx = x / width;
  rely = y / height;
  
  if (type == QTACHART_OBJ_LABEL)
  {
    drawLabel ();
  }  
  else
  if (type == QTACHART_OBJ_TEXT)   
  {
	trailerCandleText = core->bottom_text->toPlainText ();  
    drawText ();
  } 
}

void
QTACObject::setText(QGraphicsTextItem *textitem, QString candleText, qreal prc)
{
  if (type != QTACHART_OBJ_TEXT)
    return;
  	
  price = prc;
  trailerCandleText = candleText;	
  
  text = textitem;
  text->setParent (this);
  text->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
  
  if (filter == NULL)
  {
    filter = new QTACObjectEventFilter (text);  
	text->installEventFilter (filter); 
    text->setAcceptHoverEvents (true);
  }  
  
  forecolor = text->defaultTextColor ();
}

// set indicator's parameter dialog
void 
QTACObject::setParamDialog (ParamVector Param, QString title)
{
  QColorDialog *colorDialog;
  
  if (paramDialog != NULL)
    delete paramDialog;
   	
  paramDialog = new DynParamsDialog (Param, title);	
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (paramDialog);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif    
  paramDialog->setColorDialog (colorDialog);
  paramDialog->setVisible (false);
  paramDialog->setModal (true);
  connect(paramDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(modification_accepted()));
  connect(paramDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(modification_rejected()));
  
  if (editBtn != NULL)
    editBtn->setVisible (true);
}

// set objects title
void
QTACObject::setTitle (QString str)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  titlestr = str;
  
  if (type == QTACHART_OBJ_CURVE)
    title->setPlainText (titlestr + "(" + (period > 0?QString::number (period, 'f', 0):"") + ")" );
  else
    title->setPlainText (titlestr);
    
  title->setDefaultTextColor (core->forecolor);
}

// set objects attributes
void
QTACObject::setAttributes_common (QTAChartDataSet dstype,
						   int per, QString perParamName,
						   qreal xmin, qreal xmax,
                           QColor color, QString colParamName)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  dataset_type = dstype;
  forecolor = color;
  rmin = xmin;
  rmax = xmax;
  period = per;
  periodParamName = perParamName;
  colorParamName = colParamName;

  // set dataset to HLOC
  if (dataset_type == QTACHART_OPEN)
    dataset = &core->OPEN;

  if (dataset_type == QTACHART_CLOSE)
    dataset = &core->CLOSE;
    
  if (dataset_type == QTACHART_HIGH)
    dataset = &core->HIGH;
    
  if (dataset_type == QTACHART_LOW)
    dataset = &core->LOW;
    
  if (dataset_type == QTACHART_VOLUME)
    dataset = &core->VOLUME;
  
  if (type == QTACHART_OBJ_CURVE || 
      type == QTACHART_OBJ_DOT || 
      type == QTACHART_OBJ_VBARS) 
  {
	visibleitems = 0;  
    ITEMSsize = 0;
    ITEMS = new QTCGraphicsItem * [MAXITEMS];
    
    if (parentObject != NULL)
      scene->removeItem (title);  
  }
  
  if (parentObject == NULL)
    title->setToolTip ( "<span style=\"font: 9px;color: black;\">Double click to edit</span>");
  
  valueSet ();   
}  


void
QTACObject::setAttributes (QTAChartDataSet dstype,
						   int per, QString perParamName,
                           DataSet (*tafunc) (DataSet, int),
                           qreal xmin,
                           qreal xmax,
                           QColor color, QString colParamName)
{
  TAfunc = tafunc;
  setAttributes_common (dstype, per, perParamName, xmin, xmax, color, colParamName);
}

void
QTACObject::setAttributes (QTAChartDataSet dstype,
						   int per, QString perParamName, 
                           DataSet (*tafunc) (FrameVector *, int),
                           qreal xmin,
                           qreal xmax,
                           QColor color, QString colParamName)
{
  TAfunc2 = tafunc;
  setAttributes_common (dstype, per, perParamName, xmin, xmax, color, colParamName);
}

// set subchart's title to display indicator's data   
void 
QTACObject::setDataTitle (int x) 
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  
  datastr = ""; 	
  if (type == QTACHART_OBJ_SUBCHART)
  {
	QTACObject *child;
	int bar;
  
    foreach (child, children)
    {
	  if (child->type == QTACHART_OBJ_CURVE || child->type == QTACHART_OBJ_VBARS)
	  {
		qreal data = 0;  
		bar = barOnX (core, x);
		
		if (bar >= 0 && bar < child->valueset->size ())
		{
		  if (child->valueset->at(bar) > 1000 && child->valueset->at(bar) < 1000000)
		  {
			data = child->valueset->at(bar) / 1000;   	
		    datastr += " " + QString::number (data, 'f', 2) + "K";
		  }  
		  else
		  if (child->valueset->at(bar) >= 1000000)
		  {
			data = child->valueset->at(bar) / 1000000;   	
		    datastr += " " + QString::number (data, 'f', 2) + "M";  
	      }
	      else
	        datastr += " " + QString::number (child->valueset->at(bar), 'f', 2);  
		}  
      }	
    }
    title->setPlainText (titlestr + "(" + (period > 0?QString::number (period, 'f', 0):"") + "):" + datastr);
  } 
}

// find dataset's min and max on chart
void
QTACObject::minmax ()
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qreal ritem;
  
  if (parentObject != NULL && type == QTACHART_OBJ_HLINE)
  {  
	rangemin = rangemax = price;
	return; 
  }
  
  rangemin = rmin;
  if (rmin == QREAL_MIN)
    rangemin = QREAL_MAX;
  
  rangemax = rmax;
  if (rmax == QREAL_MAX)
    rangemax = QREAL_MIN;
  
  for (qint32 k = *core->startbar, 
              maxk = *core->startbar + core->nbars_on_chart,
              vsetsize = valueset->size (); 
       k < maxk; k++)
  {
    if (k < vsetsize)
    {
      ritem = valueset->at(k);

      // find dataset's minimum
      if (rmin == QREAL_MIN)
        if (rangemin > ritem)
          rangemin = ritem;

      // find dataset's maximum
      if (rmax == QREAL_MAX)
        if (rangemax < ritem)
          rangemax = ritem;
    }
  }
  
  if (type == QTACHART_OBJ_VBARS && parentObject != NULL)
  {
	if (rangemin > 0 && rangemax > 0)
	  rangemin = 0;
	if (rangemin < 0 && rangemax < 0)
	  rangemax = 0;      
  }
  
  rangemax = ((rangemax >= 0)?rangemax * 1.05:(qAbs (rangemax) * 1.05) * -1);
  rangemin = ((rangemin >= 0)?rangemin * 1.05:(qAbs (rangemin) * 1.05) * -1);
}

// slots
void
QTACObject::modification_accepted()
{
  QPen pen;
  QTACObject *child;
  	
  paramDialog->setVisible (false);
  if (paramDialog->removeCheckBox->checkState () == Qt::Checked)
  {
	clearITEMS ();
	title->setVisible (false);
	deleteit = true;
	if (type == QTACHART_OBJ_CURVE || children.size () > 0)
	{
	  foreach (child, children)
	  	child->clearITEMS ();  
    }	
	return;
  }
  
  if (colorParamName != "")
    forecolor = paramDialog->getParam (colorParamName);  
  
  if (periodParamName != "")
    period = (int) paramDialog->getParam (periodParamName);
  
  if (children.size () > 0)
  {
    foreach (child, children)
    {
	  if (child->type == QTACHART_OBJ_HLINE)
	  	child->setHLine (child->hvline, paramDialog->getParam (child->periodParamName));
	  else
	  if (child->colorParamName != "")
        child->forecolor = paramDialog->getParam (child->colorParamName); 
      else
      if (child->periodParamName != "")
        child->period = (int) paramDialog->getParam (child->periodParamName);  
    }	  
  }
  
  if (type == QTACHART_OBJ_CURVE)
    title->setPlainText (titlestr + "(" + (period > 0?QString::number (period, 'f', 0):"") + ")" );
          
  draw ();
}

void
QTACObject::modification_rejected()
{
  paramDialog->setVisible (false);		
}
