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

#ifndef QTACHART_OBJECT_H
#define QTACHART_OBJECT_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QIcon>
#include <QToolButton>
#include <QGraphicsProxyWidget>
#include "qtcgraphicsitem.h"
#include "common.h"
#include "qtachart_eventfilters.h"
#include "function_dataset.h"
#include "dynparamsdialog.h"

typedef QVector < qreal >Coordinate;
typedef QVector < QGraphicsItem *>ItemVector;
typedef QList < QTCGraphicsItem *>GrItemVector; 
typedef QVector < QGraphicsTextItem *>TextItemVector;
typedef PriceVector VolumeVector;

// object types
enum QTAChartObjectType
{
  QTACHART_OBJ_LABEL,				// label object
  QTACHART_OBJ_TEXT,     			// text object
  QTACHART_OBJ_VLINE,				// verical line
  QTACHART_OBJ_HLINE,				// horizontial line
  QTACHART_OBJ_LINE,				// trend line
  QTACHART_OBJ_CHANNEL,				// channel (not used)
  QTACHART_OBJ_FIBO,				// fibonacci
  QTACHART_OBJ_SUBCHART,			// subchart object
  QTACHART_OBJ_VBARS,				// vertical bars
  QTACHART_OBJ_CURVE,				// curve 
  QTACHART_OBJ_DOT					// dot
};

// data sets
enum QTAChartDataSet
{
  QTACHART_OPEN,					// open
  QTACHART_CLOSE,					// close
  QTACHART_HIGH,					// high
  QTACHART_LOW,						// low
  QTACHART_VOLUME,					// volume
  QTACHART_NULL						// no dataset needed
};

// forward declaration of QTACObject
class QTACObject;

// line edge
class LineEdge 
{
public:
  LineEdge ();						// constructor
  ~LineEdge ();						// destructor
  
  QString trailerCandleText; // bottom text of candle a text object is attached on
  QGraphicsTextItem *pricetxt; // the QGraphicsTextItem of price
  qreal price;  // the price
  qreal pad;  // if there is no trailing candle, the number of frames from last bar;
  qint16 txtdirection; // 0 left of edge, 1 right of edge
  qint16 sequence;  // 1 for x1,y1, 2 for x2,y2
};

typedef QVector < LineEdge *>EdgeVector;

// subchart button class
class SubChartButton : public QToolButton
{
  Q_OBJECT
public:
  explicit SubChartButton (QTACObject *); // constructor
  ~SubChartButton (); // destructor
  
  QTACObject *getOwner (void); // get owner object

private:
  // variables
  const void *chartdata;		// (QTAChartCore *) chartdata
  QTACObject *owner;
};
Q_DECLARE_TYPEINFO (SubChartButton, Q_MOVABLE_TYPE);

// object class
class QTACObject : public QObject
{
  Q_OBJECT
public:
// functions
  QTACObject (void *, QTAChartObjectType); 				// constructor
  QTACObject (QTACObject *, QTAChartObjectType); 	    // constructor
  ~QTACObject (); 										// destructor
  
  void changeForeColor (QColor color); // change the foreground color
  void clearITEMS (void);	// clear all the objects on the chart
  void deleteITEMS (void);  // delete all the objects on the chart
  void draw ();			// draw the object;
  void drawVBars ();	// draw vertical bars
  void drawLabel ();	// draw a label
  void drawText ();		// draw a label
  void drawHVLine ();	// draw a horizontal or vertical line 
  void drawTLine ();	// draw a trend line 
  void drawFibo ();		// draw a fibo line
  void drawCurve ();	// draw a curve  
  void drawDot ();		// draw a dot
  void emitUpdateOnlinePrice (RTPrice rtprice); // update online price signal emittion
  
  QTACObject *getParentObject ();	// return parent object
  QString getTitle (void);			// get object's title
  QString getTrailerCandleText (void); // get text's trailer candle 
  QString getTrailerCandleText2 (void); // get text's trailer candle 
  QString getText (void) { return text->toPlainText (); } // get object's text for label or text objects
  QFont   getFont (void) { return text->font (); } // get object's font for label or text objects
  QColor  getColor (void); // get object's color for drawing objects
  void    getCoordinates (qreal *x1, qreal *y1,
						  qreal *x2, qreal *y2)
		  { *x1 = X1; *x2 = X2; *y1 = Y1; *y2 = Y2; } // get objects received coordinates				  
  qreal   getPrice (void); // return the price level 
  qreal   getPrice2 (void); // return the price level 
  qreal   getPad (void); // get the edge's pad
  qreal   getPad2 (void); // get the edge's pad
  qint16  getTxtDirection (); // get text's direction of label
  qint16  getTxtDirection2 (); // get text's direction of label
  int     getPeriod () { return period; } // get the period
  DynParamsDialog *getParamDialog () { return paramDialog; } // indicators' parameters dialog
  DataSet valueSet ();			// get object's value set
  
  bool modifyIndicator (); // modify technical indicator 
  
  void setAttributes (QTAChartDataSet dstype,
                      int period,
                      QString periodParamName,
                      DataSet (*TAfunc) (DataSet, int),
                      qreal rangemin,
                      qreal rangemax,
                      QColor color,
                      QString colorParamName);	// set object's attributes
  void setAttributes (QTAChartDataSet dstype,
                      int period,
                      QString periodParamName,
                      DataSet (*TAfunc2) (FrameVector *, int),
                      qreal rangemin,
                      qreal rangemax,
                      QColor color,
                      QString colorParamName);  // set object's attributes      
  void setAttributes_common (QTAChartDataSet dstype,
                             int period,
                             QString periodParamName,
                             qreal rangemin,
                             qreal rangemax,
                             QColor color,
                             QString colorParamName);  // set object's attributes
  void setDataTitle (int x); // set subchart's title to display indicator's data                                               
  void setHLine (QGraphicsLineItem *hline, qreal value); // set the price level or value of a horizontal line
  void setVLine (QGraphicsLineItem *vline); // set the date/time stamp of a vertical line
  void setVLine (QGraphicsLineItem *vline, QString trailerCandleText); // set the date/time stamp of a vertical line
  void setTLine (QGraphicsLineItem *sline); // set the edges of a trend line
  void setTLine (QGraphicsLineItem *sline, LineEdge e1, LineEdge e2); // set the edges of a trend line
  void setFibo (QGraphicsLineItem *sline); // set the edges of a fibo line
  void setFibo (QGraphicsLineItem *sline, LineEdge e1, LineEdge e2); // set the edges of a fibo line
  void setTitle (QString);	   		// set object's title
  void setText (QGraphicsTextItem *item, qreal x, qreal y); // set the text of a label or text object
  void setText (QGraphicsTextItem *item, QString candleText, qreal price); // set the text of a text object
  void setParamDialog (ParamVector Param, QString title); // set indicators' parameter dialog
  void setPrice (qreal p) { price = p; } // set price
  void setPeriod (int p) { period = p; } // set period 
// variables
  const void *chartdata;     // (QTAChartCore *) chartdata
  QTCGraphicsItem **ITEMS;   // graphics items on screen
  QGraphicsLineItem *hvline; // horizontal or verical line
  QGraphicsTextItem *text;	 // for label or text objects
  QGraphicsTextItem *title;	 // object's title
  QVector < QTACObject *> children;		//  vector of children
  EdgeVector Edge;           // edges of line
  ItemVector FiboLevel;	     // graphics line items for fibo levels
  TextItemVector FiboLevelLbl; // text labels for FiboLevel
  TextItemVector FiboLevelPrcLbl; // text labels for FiboLevel prices
  PriceVector FiboLevelPrc; // percentage fibo levels 100, 61.8, 50, 38.2, 23.6, 0
  qint32 subchart_dec;		     // decrement of nsubcharts. initially 1, 0 if delete later
  qint32 type;			     // object's type
  qint32 ITEMSsize;			 // size of ITEMS;
  bool deleteit;			 // true if object is going to be deleted
  bool onlineprice;			 // true for online price object

signals:
  void updateOnlinePrice (RTPrice rtprice); // update online price signal
  
private:
// functions
  void QTACObject_constructor_common ();	// constructors' body function
  void minmax (); // find dataset's min and max on chart
  inline qreal subYonPrice (qreal price)  // returns the y of a subchart that corresponds to the price argument
  { return basey + ((height - 10) - ((price + qAbs (rangemin)) / quantum)); }

// variables
  QIcon closeicon; // icon for close button of subcharts
  QTACObjectEventFilter *filter; // event filter
  SubChartButton *closeBtn;	// subchart's close button
  SubChartButton *editBtn;	// subchart's edit button
  QGraphicsProxyWidget *prxcloseBtn; // proxy to closeBtn
  QGraphicsProxyWidget *prxeditBtn; // proxy to editBtn
  QGraphicsLineItem *bottomline; // subchart's bottom line
  QGraphicsScene *scene;			// ptr on chartdata->Scene
  DynParamsDialog *paramDialog; // indicators' parameters dialog
  QTACObject *parentObject;		// parent subchart
  DataSet (*TAfunc) (DataSet, int); // function to be used
  DataSet (*TAfunc2) (FrameVector *, int); // function to be used
  DataSet dataset;   // object's dataset
  DataSet valueset;   // object's valueset generated by TAfunc (dataset)
  Coordinate X, Y;	// coordinates
  PriceVector FiboLevelPrice; // price the FiboLevel is attached  
  QString titlestr;			// title string
  QString datastr;			// indicator's data string
  QString trailerCandleText;// bottom text of candle a text object is attached on
  QString periodParamName;		// name of the parameter of DynParamsDialog for period
  QString colorParamName;		// name of the parameter of DynParamsDialog for color
  qreal width;				// width
  qreal height;				// height
  qreal basex;				// subchart's x coordinate 
  qreal basey;				// subchart's y coordinate
  qreal relx;				// relative x coordinate
  qreal rely;				// relative y coordinate
  qreal objx;				// object's x coordinate
  qreal objy;				// object's y coordinate
  qreal X1, Y1, X2, Y2;		// drawing object's received coordinates 
  qreal rangemin, rmin;			// range minimum. rmin QREAL_MIN means determine from dataset
  qreal rangemax, rmax;			// range maximum. rmax QREAL_MAX means determine from dataset
  qreal quantum;				// (rmax - rmin) / (height - 10)
  qreal price;					// price level for horizontal line object
  QColor forecolor;			// foreground color
  QColor backcolor;			// background color
  qint32 thickness;			// object's thickness in points
  qint32 visibleitems;		// number of visible items
  int period;				// TAfunc's period
  int lastperiod;			// period used for last calculation
  int valuesetsize;			// size of valueset
  QTAChartDataSet dataset_type;   // object's dataset type
  
private slots:
  void modification_accepted();
  void modification_rejected();
  void updateOnlinePriceSlot (RTPrice rtprice);  
};

Q_DECLARE_TYPEINFO (QTACObject, Q_MOVABLE_TYPE);
typedef QVector < QTACObject *> ObjectVector;

#endif // QTACOBJECT_H
