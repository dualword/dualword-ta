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

#include <QPen>
#include <QGraphicsScene>
#include "qtcgraphicsitem.h"

// constructor
QTCGraphicsItem::QTCGraphicsItem ( int reqtype, QGraphicsItem * parent)
{
  if (parent != NULL)	
    setParentItem (parent);
  QTCGraphicsItem_constructor_common (reqtype);
  
}

void 
QTCGraphicsItem::QTCGraphicsItem_constructor_common (int reqtype)
{
  type = reqtype; 	
  Scene = NULL;
  initialized = false;
  if (type == LineItem)
    lineItem = new QGraphicsLineItem ();
  else
  if (type == RectItem)
    rectItem = new QGraphicsRectItem (); 
  else
  if (type == EllipseItem)
    ellipseItem = new QGraphicsEllipseItem (); 
}

void 
QTCGraphicsItem::init (QGraphicsScene *scene)
{
  QGraphicsItem *item;	
  Scene = scene; 	
  
  if (initialized)
    return;
  
  if (type == LineItem)
    scene->addItem (lineItem);
  else
  if (type == RectItem)
    scene->addItem (rectItem);
  else
  if (type == EllipseItem)
    scene->addItem (ellipseItem); 
  
  foreach (item, childItems ())
  {
	if (type == LineItem)
      item->setParentItem (lineItem);
    else
    if (type == RectItem)
      item->setParentItem (rectItem);
    else
    if (type == EllipseItem)    
      item->setParentItem (ellipseItem);
  }  
  
  initialized = true;
}

// destructor
QTCGraphicsItem::~QTCGraphicsItem ()
{
  if (Scene != NULL)
  {	
    if (type == LineItem)
      Scene->removeItem (lineItem);
    else
    if (type == RectItem)
      Scene->removeItem (rectItem);	
    else
    if (type == EllipseItem)
      Scene->removeItem (ellipseItem);   
  }    
  
  if (type == LineItem)
    delete lineItem;
  else
  if (type == RectItem)
    delete rectItem;
  else
  if (type == EllipseItem)  
    delete (ellipseItem);   
}

/// line
// line
QLineF	
QTCGraphicsItem::line () const
{
  if (type == LineItem)
    return lineItem->line () ;
    
  return QLineF (0,0,0,0);	
}

// setLine
void	    
QTCGraphicsItem::setLine ( const QLineF & line )
{
  if (type == LineItem)
    lineItem->setLine (line);
}
  
void	    
QTCGraphicsItem::setLine ( qreal x1, qreal y1, qreal x2, qreal y2 )
{
  if (type == LineItem)
    lineItem->setLine (x1, y1, x2, y2);
}

/// rect
// rect
QRectF	
QTCGraphicsItem::rect () const
{
  if (type == RectItem)
    return rectItem->rect () ;
  else
  if (type == EllipseItem)
    return ellipseItem->rect () ;  
    
  return QRectF (0,0,0,0); 		
}

// setRect
void	    
QTCGraphicsItem::setRect ( const QRectF & rectangle )
{
  if (type == RectItem)
    rectItem->setRect (rectangle);
  else
  if (type == EllipseItem)
    ellipseItem->setRect (rectangle);
}

void	    
QTCGraphicsItem::setRect ( qreal x, qreal y, qreal width, qreal height )
{
   if (type == RectItem)
    rectItem->setRect (x, y, width, height);
  else
  if (type == EllipseItem)
    ellipseItem->setRect (x, y, width, height);  
}

// brush
QBrush    
QTCGraphicsItem::brush () const
{
  if (type == RectItem)
    rectItem->brush ();
  else
  if (type == EllipseItem)
    ellipseItem->brush ();  
  
  return QBrush (Qt::black);  	
}

// setBrush
void      
QTCGraphicsItem::setBrush ( const QBrush & brush )
{
  if (type == RectItem)
    rectItem->setBrush (brush);	
  else
  if (type == EllipseItem)
    ellipseItem->setBrush (brush);	  
}

/// general
// boundingRect
QRectF
QTCGraphicsItem::boundingRect () const
{
  if (type == LineItem)
    return lineItem->boundingRect () ;
  else
  if (type == RectItem)
    return rectItem->boundingRect () ;	
  else
  if (type == EllipseItem)
    return ellipseItem->boundingRect () ;	  
  
  return QRectF (0,0,0,0);  
}

// paint
void      
QTCGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
  return;
}

// pen
QPen	    
QTCGraphicsItem::pen () const
{
  if (type == LineItem)
    return lineItem->pen () ;
  else
  if (type == RectItem)
    return rectItem->pen () ;
  else
  if (type == EllipseItem)
    return ellipseItem->pen () ;  
  
  return QPen (Qt::black);  	
}

// setPen
void      
QTCGraphicsItem::setPen ( const QPen & pen )
{
  if (type == LineItem)
    lineItem->setPen (pen) ;
  else
  if (type == RectItem)
    rectItem->setPen (pen) ;
  else
  if (type == EllipseItem)
    ellipseItem->setPen (pen) ;  	
}

// zValue
qreal     
QTCGraphicsItem::zValue () const
{
  if (type == LineItem)
    return lineItem->zValue ();
  else
  if (type == RectItem)
    return rectItem->zValue ();
  else
  if (type == EllipseItem)
    return ellipseItem->zValue ();
  
  return 0;  
}

// setZValue
void      
QTCGraphicsItem::setZValue ( qreal z )
{
  if (type == LineItem)
    lineItem->setZValue (z);
  else
  if (type == RectItem)
    rectItem->setZValue (z);
  else
  if (type == EllipseItem)
    ellipseItem->setZValue (z);  	
}

// opacity
qreal     
QTCGraphicsItem::opacity () const
{
  if (type == LineItem)
    return lineItem->opacity ();
  else
  if (type == RectItem)
    return rectItem->opacity ();
  else
  if (type == EllipseItem)
    return ellipseItem->opacity ();  
  
  return 0;  
}

// setOpacity
void      
QTCGraphicsItem::setOpacity ( qreal z )
{
  if (type == LineItem)
    lineItem->setOpacity (z);
  else
  if (type == RectItem)
    rectItem->setOpacity (z);	
  else
  if (type == EllipseItem)
    ellipseItem->setOpacity (z);  
}


// pos
QPointF   
QTCGraphicsItem::pos () const
{
  if (type == LineItem)
    return lineItem->pos ();
  else
  if (type == RectItem)
    return rectItem->pos ();
  else
  if (type == EllipseItem)
    return ellipseItem->pos (); 
  
  return QPointF (0,0);  	
}

// setPos
void      
QTCGraphicsItem::setPos (const QPointF & pos)
{
  if (type == LineItem)
    lineItem->setPos (pos);
  else
  if (type == RectItem)
    rectItem->setPos (pos);	
  else
  if (type == EllipseItem)
    ellipseItem->setPos (pos);	  
}

void      
QTCGraphicsItem::setPos (qreal x, qreal y)
{
  if (type == LineItem)
    lineItem->setPos (x, y);
  else
  if (type == RectItem)
    rectItem->setPos (x, y);	
  else
  if (type == EllipseItem)
    ellipseItem->setPos (x, y);	  
}

// isVisible
bool      
QTCGraphicsItem::isVisible () const
{
  if (type == LineItem)
    return lineItem->isVisible ();
  else
  if (type == RectItem)
    return rectItem->isVisible ();	
  else
  if (type == EllipseItem)
    return ellipseItem->isVisible ();	 
  
  return true;   
}

// setVisible
void      
QTCGraphicsItem::setVisible (bool visible)
{
  if (type == LineItem)
    lineItem->setVisible (visible);
  else
  if (type == RectItem)
    rectItem->setVisible (visible);	
  else
  if (type == EllipseItem)
    ellipseItem->setVisible (visible);	  
}

// setSize
void      
QTCGraphicsItem::setSize (qreal x, qreal y, qreal width, qreal height)
{
  if (type == LineItem)
    lineItem->setLine (x, y, width, height);
  else
  if (type == RectItem)
    rectItem->setRect (x, y, width, height);
  else
  if (type == EllipseItem)
    ellipseItem->setRect (x, y, width, height);
}

// children
QList<QGraphicsItem *> 
QTCGraphicsItem::children () const
{
  if (type == LineItem)
    return lineItem->childItems ();
  
  if (type == RectItem)
    return rectItem->childItems ();
  
  return ellipseItem->childItems ();  
}
