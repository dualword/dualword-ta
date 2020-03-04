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

#include <QApplication>
#include <QTabWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include "qtachart_core.h"

static qreal xpad, ypad;

///
// QTAChartSceneEventFilter
QTAChartSceneEventFilter::QTAChartSceneEventFilter (QObject * parent)
{
  padx = -1;
  pady = -1;
  phase = 0;	
  
  if (parent != NULL)
    setParent (parent);
}

QTAChartSceneEventFilter::~QTAChartSceneEventFilter ()
{
}

// control drag and add an object on the chart
void 
QTAChartSceneEventFilter::dragObjectCtrl (QObject *coreptr, QEvent *event)
{
  QTAChartCore *core;
  	
  core = reinterpret_cast<QTAChartCore *> (coreptr);	
  if (core->dragged_obj_type == QTACHART_OBJ_LABEL ||
      core->dragged_obj_type == QTACHART_OBJ_TEXT)
  {    
    dragText (core, event);
    return;
  }
  
  if (core->dragged_obj_type == QTACHART_OBJ_HLINE ||
      core->dragged_obj_type == QTACHART_OBJ_VLINE ||
      core->dragged_obj_type == QTACHART_OBJ_LINE ||
      core->dragged_obj_type == QTACHART_OBJ_FIBO)
  {
	dragHVLine (core, event);
	return;  
  }
}

// drag and add a line
void
QTAChartSceneEventFilter::dragHVLine (QObject *coreptr, QEvent *event)
{
  QTAChartCore *core;
  QGraphicsSceneMouseEvent *qMouse;
  QPointF point;
  QRectF rect;
  qreal x, y;
  
  core = reinterpret_cast<QTAChartCore *> (coreptr);	
  // mouse buttons
  if (event->type () == QEvent::GraphicsSceneMousePress ||
      event->type () == QEvent::GraphicsSceneMouseRelease ||
      event->type () == QEvent::GraphicsSceneMouseMove)
  {
	  
    qMouse = (QGraphicsSceneMouseEvent *) event;
    point = qMouse->scenePos ();
    x = point.x ();
    y = point.y ();
    
    if (core->dragged_obj_type == QTACHART_OBJ_HLINE)
    {
	  if (y < core->charttopmost || y > core->chartbottomost)
	    return;
      core->hvline->setLine (core->chartleftmost /*+ xpad*/, y + ypad, core->chartrightmost /*+ xpad*/, y + ypad);
      core->setRullerCursor (y);
      core->setBottomText (x);
      if (event->type () == QEvent::GraphicsSceneMouseRelease)
      {
        core->addHLine (core->hvline, priceOnY (core, y));
        core->object_drag = false;      
        QApplication::restoreOverrideCursor ();
      }  
    }  
    
    if (core->dragged_obj_type == QTACHART_OBJ_VLINE)
    {
	  if (x < core->chartleftmost || x > core->chartrightmost)
	    return;
      core->hvline->setLine (x + xpad, core->charttopmost /*+ ypad*/, x + xpad, core->chartbottomost /*+ ypad*/);
      core->setRullerCursor (y);
      core->setBottomText (x);
      if (event->type () == QEvent::GraphicsSceneMouseRelease)
      {
        core->addVLine (core->hvline);
        core->object_drag = false;      
        QApplication::restoreOverrideCursor ();
      }  
    }
    
    if (core->dragged_obj_type == QTACHART_OBJ_LINE)
    {
	  if (x < core->chartleftmost || x > core->chartrightmost)
	    return;
	  
	  if (y < core->charttopmost || y > core->chartbottomost)
	    return;
	  
	  if (core->hvline->line ().x1 () == core->hvline->line ().x2 () &&
	      core->hvline->line ().y1 () == core->hvline->line ().y2 () &&
	      core->hvline->line ().y2 () == 0)
	    phase = 0;
	  else
	    phase = 1;      
	  
	  core->setRullerCursor (y);
      core->setBottomText (x);
      
      if (phase == 1)
      {
		qreal x1, y1;
		
		x1 = core->hvline->line().x1 ();
		y1 = core->hvline->line().y1 ();  
		
		core->hvline->setLine (x1, y1, x + xpad, y + ypad);  
      }
      
      if (event->type () == QEvent::GraphicsSceneMousePress)
      {	 
	    if (phase == 0)	
		{
		  core->hvline->setLine (x, y, x, y);
		  phase = 1;	
	    }
	  }
	  
	  if (event->type () == QEvent::GraphicsSceneMouseRelease)
      { 
	    if (phase == 1)
	    {
		  core->addTLine (core->hvline);	
		  core->object_drag = false;      
          QApplication::restoreOverrideCursor ();
		  phase = 0;	
	    } 		
	  }
    }
    
    if (core->dragged_obj_type == QTACHART_OBJ_FIBO)
    {
	  if (x < core->chartleftmost || x > core->chartrightmost)
	    return;
	  
	  if (y < core->charttopmost || y > core->chartbottomost)
	    return;
	  
	  if (core->hvline->line ().x1 () == core->hvline->line ().x2 () &&
	      core->hvline->line ().y1 () == core->hvline->line ().y2 () &&
	      core->hvline->line ().y2 () == 0)
	    phase = 0;
	  else
	    phase = 1;      
	  
	  core->setRullerCursor (y);
      core->setBottomText (x);
      
      if (phase == 1)
      {
		 qreal y1;
		
		 y1 = core->hvline->line().y1 ();  
		 core->hvline->setLine (x + xpad, y1,  x + xpad, y + ypad);  
      }
      
      if (event->type () == QEvent::GraphicsSceneMousePress)
      {	 
	    if (phase == 0)	
		{
		  core->hvline->setLine (x, y, x, y);
		  phase = 1;	
	    }
	  }
	  
	  if (event->type () == QEvent::GraphicsSceneMouseRelease)
      { 
	    if (phase == 1)
	    {
		  core->addFibo (core->hvline);	
		  core->object_drag = false;      
          QApplication::restoreOverrideCursor ();
		  phase = 0;	
	    } 		
	  }
    }
  }  
}	


// drag and add a Label/Text object
void 
QTAChartSceneEventFilter::dragText (QObject *coreptr, QEvent *event)
{
  QTAChartCore *core;
  QGraphicsSceneMouseEvent *qMouse;
  QPointF point;
  QRectF rect;
  qreal x, y;
  
  core = reinterpret_cast<QTAChartCore *> (coreptr);	
      
  // mouse buttons
  if (event->type () == QEvent::GraphicsSceneMousePress ||
      event->type () == QEvent::GraphicsSceneMouseRelease ||
      event->type () == QEvent::GraphicsSceneMouseMove)
  {
    qMouse = (QGraphicsSceneMouseEvent *) event;
    point = qMouse->scenePos ();
    x = point.x ();
    y = point.y ();
    rect = core->textitem->boundingRect ();	
    
    if (core->textitem->x() <= 0)
      padx = rect.width () / 2;
   
    if (padx == -1)
      padx = x - core->textitem->x();
	  
	x -= padx; 
	
	if (x < core->chartleftmost)
	  x = core->chartleftmost;
    else	
	if (x + rect.width () > core->chartrightmost)
	  x = core->chartrightmost - rect.width ();
	
	if (core->textitem->y() <= 0)
      pady = rect.height () / 2;
    
    if (pady == -1)
      pady = y - core->textitem->y();  
      
    y -= pady;
    
    if (y < core->charttopmost)
      y = core->charttopmost;
    else
    if (y + rect.height () > core->chartbottomost)
      y = core->chartbottomost - rect.height ();  
   
    core->textitem->setPos (x, y);
    core->setBottomText (x);
    if (event->type () == QEvent::GraphicsSceneMouseRelease)
    {
      if (core->dragged_obj_type == QTACHART_OBJ_LABEL)
        core->addLabel (core->textitem, x, y);
      else
      if (core->dragged_obj_type == QTACHART_OBJ_TEXT)
        core->addText (core->textitem, x, y);
        
      core->object_drag = false;      
      QApplication::restoreOverrideCursor ();
      padx = pady = -1;
    }
  }
}

bool 
QTAChartSceneEventFilter::eventFilter (QObject * object, QEvent * event)
{
  QTAChartCore *chartcore;
  QTAChart *chart;
  QGraphicsScene *scene = (QGraphicsScene *) object;
  QGraphicsSceneMouseEvent *qMouse;
  QGraphicsSceneWheelEvent *qWheel;
  QList < QGraphicsView * >view = scene->views ();
  QPointF point;
  qreal x, y, diff, adiff;
  
  event->accept ();
  chart = reinterpret_cast<QTAChart *> (view[0]->parentWidget ());
  chartcore = reinterpret_cast<QTAChartCore *> (getData (chart));
  
  if (chartcore->object_drag)
  {
    dragObjectCtrl (chartcore, event);
    return false;
  }
  
  if (!chartcore->events_enabled)
    return false;

  // mouse buttons
  if (event->type () == QEvent::GraphicsSceneMousePress ||
      event->type () == QEvent::GraphicsSceneMouseRelease ||
      event->type () == QEvent::GraphicsSceneMouseMove)
  {
    qMouse = (QGraphicsSceneMouseEvent *) event;
    point = qMouse->scenePos ();

    x = point.x ();
    y = point.y ();
    chartcore->setBottomText (x);
    chartcore->setRullerCursor (y);
    
    if (x >= chartcore->chartleftmost &&
        y >= chartcore->charttopmost &&
        x <= chartcore->chartrightmost + chartcore->right_border_width &&
        y <= chartcore->chartbottomost)
      chartcore->last_x = x;
    else
    {
      QApplication::restoreOverrideCursor ();
      chartcore->drag = false;
      return false;
    }
  }
  else
  {
	// mouse wheel
    if (event->type () == QEvent::GraphicsSceneWheel)
    {
      qWheel = (QGraphicsSceneWheelEvent *) event;

      if (qWheel->delta () > 0)
        chartcore->chartForward (1);	// right
      else
        chartcore->chartBackward (1);	//left

      chartcore->setRullerCursor (chartcore->ruller_cursor_y);
      chartcore->setBottomText (chartcore->last_x);
    }
    return false;
  }  
    
  // mouse button press
  if (event->type () == QEvent::GraphicsSceneMousePress)
  {
    // left button
    if (qMouse->button () == Qt::LeftButton && chartcore->drag == false )
    {
      if (y > chartcore->title_height && x < chartcore->width)
      {
        chartcore->drag = true;
        chartcore->initial_mouse_x = x;
        QApplication::setOverrideCursor (QCursor (Qt::DragMoveCursor));
      }
    }
  }

  // mouse button release
  if (event->type () == QEvent::GraphicsSceneMouseRelease
      && chartcore->drag == true)
  {
    chartcore->drag = false;
    QApplication::restoreOverrideCursor ();
    return true;
  }

  // drag
  if (chartcore->drag == true)
  {
    int sense = 4, bars;
    diff = chartcore->initial_mouse_x - x;
    adiff = qAbs (diff);

    // pointer moved right
    if (adiff >= sense)
    {
      bars = (int) (adiff / chartcore->framewidth);
      if (bars > 0)
      {
        if (diff < 0)
          chartcore->chartBackward (bars);
        else		// pointer moved left
          chartcore->chartForward (bars);
        chartcore->initial_mouse_x = point.x ();
        return true;
      }
    }
  }
  
  return QObject::eventFilter (object, event);
}

///
// QTAChartEventFilter
QTAChartEventFilter::QTAChartEventFilter (QObject * parent)
{
  if (parent != NULL)
    setParent (parent);	
}

QTAChartEventFilter::~QTAChartEventFilter ()
{
  
}

bool 
QTAChartEventFilter::eventFilter (QObject * watched, QEvent * event)
{
  QTAChart *chart;
  QTAChartCore *chartcore;
  
  if (watched->inherits ("QGraphicsView"))
  {
    QGraphicsView *View = (QGraphicsView *) watched;
    chart = reinterpret_cast<QTAChart *> (View->parentWidget ());
    chartcore = reinterpret_cast<QTAChartCore *> (getData (chart));

    if (!chartcore->events_enabled)
      return NULL;

    if (event->type () == QEvent::Show)
      chartcore->Scene.setFocus (Qt::OtherFocusReason);	
    
    // keyboard
    if (event->type () == QEvent::KeyPress ||
        event->type () == QEvent::FocusIn)
    {
      QKeyEvent *keyEvent = static_cast < QKeyEvent * >(event);
      Qt::Key keyPressed = (Qt::Key) keyEvent->key ();

      QApplication::restoreOverrideCursor ();
      chartcore->setRullerCursor (chartcore->ruller_cursor_y);
      chartcore->setBottomText (chartcore->last_x);
      
      // tab
      if (keyPressed == Qt::Key_Tab)
      {
		QTabWidget *tabWidget;
		int idx;
		
		tabWidget = (QTabWidget *) chart->parentWidget ()->parentWidget ();
		idx = tabWidget->currentIndex ();
		idx ++;
		
		if (idx == tabWidget->count ())
		  return true;
		 
        tabWidget->setCurrentIndex (idx);
        tabWidget->update ();
        return true;
      }
      // backtab
      else if (keyPressed == Qt::Key_Backtab)
      {
		QTabWidget *tabWidget;
		int idx;
		
		tabWidget = (QTabWidget *) chart->parentWidget ()->parentWidget ();
		idx = tabWidget->currentIndex ();
		if (idx == 0)
		  return true;
		
		idx --;  
        tabWidget->setCurrentIndex (idx);
        tabWidget->update ();
        return true;        
      }
      // left
      else if (keyPressed == Qt::Key_Left)
      {
        chartcore->chartBackward (1);
        return true;
      }
      // right
      else if (keyPressed == Qt::Key_Right)
      {
        chartcore->chartForward (1);
        return true;
      }
      // home
      else if (keyPressed == Qt::Key_Home)
      {
        chartcore->chartBegin ();
        return true;
      }
      // end
      else if (keyPressed == Qt::Key_End)
      {
        chartcore->chartEnd ();
        return true;
      }
      // pgup
      else if (keyPressed == Qt::Key_PageUp)
      {
        chartcore->chartPagePrevious ();
        return true;
      }
      // pgdn
      else if (keyPressed == Qt::Key_PageDown)
      {
        chartcore->chartPageNext ();
        return true;
      }
      // esc
      else if (keyPressed == Qt::Key_Escape)
      {
        if (chartcore->object_drag == true)
        {
          chartcore->object_drag = false;
          if (chartcore->textitem != NULL)
          {
		    delete chartcore->textitem; 
		    chartcore->textitem = NULL;
		  }
		  
		  if (chartcore->hvline != NULL)
		  {
			delete chartcore->hvline;
			chartcore->hvline = NULL;
		  }
		  	  
		  QApplication::restoreOverrideCursor ();
	    }
        return true;
      }
    }
  }
  return QObject::eventFilter (watched, event);
}

///
// QTACObjectEventFilter
QTACObjectEventFilter::QTACObjectEventFilter (QObject * parent)
{
  if (parent != NULL)
    setParent (parent);
}

QTACObjectEventFilter::~QTACObjectEventFilter ()
{
  
}

bool 
QTACObjectEventFilter::eventFilter (QObject * watched, QEvent * event)
{
  QTAChartCore * core;	
  QTACObject *object;
    
  object = reinterpret_cast<QTACObject *> (watched->parent ());
  core = reinterpret_cast<QTAChartCore *> ((void *) object->chartdata);
  
  if (object->deleteit)
    return QObject::eventFilter (object, event);
  
  if (core->object_drag)
    return QObject::eventFilter (object, event);
  
  if (event->type () == QEvent::GraphicsSceneHoverEnter)
  {
	if (object->type == QTACHART_OBJ_HLINE ||  
	    object->type == QTACHART_OBJ_VLINE)
	{   	   
	   object->title->setOpacity (0.7);
	   object->hvline->setOpacity (0.7);
	   core->dragged_obj_type = object->type;
	   core->events_enabled = false;
	}  
	  
	if (object->type == QTACHART_OBJ_LABEL ||  
	    object->type == QTACHART_OBJ_TEXT)
	{    
      object->text->setOpacity (0.7);
	  core->textitem = object->text;
	  core->dragged_obj_type = object->type;
	  core->events_enabled = false;
	}  
	
	if (object->type == QTACHART_OBJ_LINE)
	{  
	   if (watched == object->Edge[0]->pricetxt)
	   {
		 LineEdge edge;
		 qreal x1, x2, y1, y2;
		 edge = *object->Edge[1];
		 *object->Edge[1]= *object->Edge[0]; 
		 *object->Edge[0] = edge;
		 object->Edge[0]->sequence = 1;
		 object->Edge[1]->sequence = 2;
		 x1 = object->hvline->line ().x2 ();
		 x2 = object->hvline->line ().x1 ();
		 y1 = object->hvline->line ().y2 ();
		 y2 = object->hvline->line ().y1 ();
		 object->hvline->setLine (x1, y1, x2, y2);
		 core->hvline = object->hvline; 
	   }	 
	   
	   object->title->setOpacity (0.7);	    
	   object->hvline->setOpacity (0.7);
	   object->Edge[0]->pricetxt->setOpacity (0.7);
	   object->Edge[1]->pricetxt->setOpacity (0.7);
	   core->dragged_obj_type = object->type;
	   core->events_enabled = false;
	}
	
	if (object->type == QTACHART_OBJ_FIBO)
	{  
	  if (watched == object->Edge[0]->pricetxt)
	  {
		 LineEdge edge;
		 qreal x1, x2, y1, y2;
		 edge = *object->Edge[1];
		 *object->Edge[1]= *object->Edge[0]; 
		 *object->Edge[0] = edge;
		 object->Edge[0]->sequence = 1;
		 object->Edge[1]->sequence = 2;
		 x1 = object->hvline->line ().x2 ();
		 x2 = object->hvline->line ().x1 ();
		 y1 = object->hvline->line ().y2 ();
		 y2 = object->hvline->line ().y1 ();
		 object->hvline->setLine (x1, y1, x2, y2);
		 core->hvline = object->hvline; 
	   }	
	   
	  object->title->setOpacity (0.7);	    
	  object->hvline->setOpacity (0.7);
	  object->Edge[0]->pricetxt->setOpacity (0.7);
	  object->Edge[1]->pricetxt->setOpacity (0.7);
	  core->dragged_obj_type = object->type;
	  core->events_enabled = false;
	  for (qint32 counter = 0; counter < object->FiboLevelPrc.size (); counter ++)
	  {
		object->FiboLevelLbl[counter]->setOpacity (0.7);
		object->FiboLevelPrcLbl[counter]->setOpacity (0.7); 
		object->FiboLevel[counter]->setOpacity (0.7); 
      }
	}
	
	if (object->type == QTACHART_OBJ_CURVE || object->type == QTACHART_OBJ_DOT)
    {
	  QTACObject *child;	
      object->title->setOpacity (0.7);	    
	  core->object_drag = false;
	  core->events_enabled = true;
	 
	  for (qint32 counter = 0; counter < object->ITEMSsize; counter ++)
		object->ITEMS[counter]->setOpacity (0.7);
      
      foreach (child, object->children)
      {
		for (qint32 counter = 0; counter < child->ITEMSsize; counter ++)
		  child->ITEMS[counter]->setOpacity (0.7);  
      }		
    }
    
	return false;
  }	
  
  if (event->type () == QEvent::GraphicsSceneHoverLeave)
  {
	if (object->type == QTACHART_OBJ_HLINE ||  
	    object->type == QTACHART_OBJ_VLINE)
	{
	  object->title->setOpacity (1.0);
	  object->hvline->setOpacity (1.0);
	  core->object_drag = false;
	  core->events_enabled = true;	 
    }
    	  
	if (object->type == QTACHART_OBJ_LABEL ||  
	    object->type == QTACHART_OBJ_TEXT)
	{    
	  object->text->setOpacity (1.0);
	  core->object_drag = false;
	  core->events_enabled = true;
	}  
	
	if (object->type == QTACHART_OBJ_LINE)
	{
	  object->title->setOpacity (1.0);	
	  object->hvline->setOpacity (1.0);
	  object->Edge[0]->pricetxt->setOpacity (1.0);
	  object->Edge[1]->pricetxt->setOpacity (1.0);
	  core->object_drag = false;
	  core->events_enabled = true;	 
    }
    
    if (object->type == QTACHART_OBJ_FIBO)
    {
      object->title->setOpacity (1.0);	    
	  object->hvline->setOpacity (1.0);
	  object->Edge[0]->pricetxt->setOpacity (1.0);
	  object->Edge[1]->pricetxt->setOpacity (1.0);
	  core->object_drag = false;
	  core->events_enabled = true;
	 
	  for (qint32 counter = 0; counter < object->FiboLevelPrc.size (); counter ++)
	  {
		object->FiboLevelLbl[counter]->setOpacity (1.0);
		object->FiboLevelPrcLbl[counter]->setOpacity (1.0); 
		object->FiboLevel[counter]->setOpacity (1.0); 
      }
    }  
    
    if (object->type == QTACHART_OBJ_CURVE || object->type == QTACHART_OBJ_DOT)
    {
	  QTACObject *child;	
      object->title->setOpacity (1.0);	    
	  core->object_drag = false;
	  core->events_enabled = true;
	 
	  for (qint32 counter = 0; counter < object->ITEMSsize; counter ++)
		object->ITEMS[counter]->setOpacity (1.0);
	  
	  foreach (child, object->children)
      {
		for (qint32 counter = 0; counter < child->ITEMSsize; counter ++)
		  child->ITEMS[counter]->setOpacity (1.0);  
      }	
    }
    return false;
  }
  
  if (event->type () == QEvent::GraphicsSceneMouseDoubleClick)
  {
	if (object->type == QTACHART_OBJ_CURVE || object->type == QTACHART_OBJ_DOT)
	{
	  bool modrslt;	
	  core->events_enabled = false;	
	  modrslt = object->modifyIndicator ();
	  if (modrslt == false)
	    object->deleteit = true;
	  core->events_enabled = true;
	}  
	    
	if (object->type == QTACHART_OBJ_HLINE ||  
	    object->type == QTACHART_OBJ_VLINE)
	{
	  bool modrslt;
	  modrslt  = core->lineobjectdialog->modify (object);
	  if (modrslt == false)
	  {
	    object->deleteit = true;
	    core->object_drag = false;
	    core->events_enabled = true;
	  }
    }
    	  
	if (object->type == QTACHART_OBJ_LABEL ||  
	    object->type == QTACHART_OBJ_TEXT)
	{
	  bool modrslt;	
	  modrslt  = core->textobjectdialog->modify (object->text);
	  if (modrslt == false)
	  {
	    object->deleteit = true;
	    core->object_drag = false;
	    core->events_enabled = true;
	  }  
    }	
    
    if (object->type == QTACHART_OBJ_LINE ||
        object->type == QTACHART_OBJ_FIBO)
	{
	  bool modrslt;
	  modrslt  = core->lineobjectdialog->modify (object);
	  if (modrslt == false)
	  {
	    object->deleteit = true;
	    core->object_drag = false;
	    core->events_enabled = true;
	  }
    }
    
    if (object->deleteit == false)
      core->draw ();
  }  
  
  if (event->type () == QEvent::GraphicsSceneMousePress)
  {
	QGraphicsSceneMouseEvent *qMouse;
	QPointF point;  
	
	qMouse = (QGraphicsSceneMouseEvent *) event; 
	point = qMouse->scenePos ();
	
	if (qMouse->button () == Qt::LeftButton) 
	{
	  if (object->type == QTACHART_OBJ_HLINE ||  
	      object->type == QTACHART_OBJ_VLINE ||
	      object->type == QTACHART_OBJ_LINE ||
	      object->type == QTACHART_OBJ_FIBO)
	  if (object->title->opacity () < 1)  
	  { 
		 core->object_drag = true;
		 core->hvline = object->hvline;
		 xpad = core->hvline->line().x2() - point.x ();
		 ypad = core->hvline->line().y2() - point.y ();
         QApplication::setOverrideCursor (QCursor
                                      (Qt::DragMoveCursor));
      }
	  
	  if (object->type == QTACHART_OBJ_LABEL ||  
	      object->type == QTACHART_OBJ_TEXT)
	  if (object->text->opacity () < 1)
      {	
	    core->object_drag = true;
        QApplication::setOverrideCursor (QCursor
                                      (Qt::DragMoveCursor));
      }
    }	  
  }
  return QObject::eventFilter (object, event);
}
