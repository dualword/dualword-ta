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

#ifndef QTACHART_H
#define QTACHART_H

#include <QWidget>
#include <QList>

enum
{
  QTACHART_TFDAY = 0,		// timeframe: day
  QTACHART_TFWEEK,		// timeframe: week
  QTACHART_TFMONTH,		// timeframe: month
  QTACHART_LINEAR = 100,      // chart: linear price scale
  QTACHART_LOGARITHMIC, // chart: logarithmic price scale 
  QTACHART_CANDLE = 200,		// chart: candle chart
  QTACHART_HEIKINASHI,	// chart: heikin-ashi candle chart
  QTACHART_BAR,			// chart: bar chart
  QTACHART_LINE 		// chart: line chart
};

struct QTAChartFrame		// frame data as loaded from sqlite table
{
  QString Text;
  qreal High;
  qreal Low;
  qreal Open;
  qreal Close;
  qreal AdjClose;
  qreal Volume;
  quint16 year;
  quint16 month;
  quint16 day;
  char Date[16];
  char Time[16];
};

Q_DECLARE_TYPEINFO (QTAChartFrame, Q_MOVABLE_TYPE);
typedef QList < QTAChartFrame > FrameVector;

struct QTAChartData		// fundamenta data as loaded from sqlite table
{
  QString bv;
  QString mc;
  QString ebitda;
  QString pe;
  QString peg;
  QString dy;
  QString epscurrent;
  QString epsnext;
  QString es;
  QString ps;
  QString pbv;
};

Q_DECLARE_TYPEINFO (QTAChartData, Q_MOVABLE_TYPE);

namespace Ui
{
  class QTAChart;
}

class QTAChart:public QWidget
{
Q_OBJECT public:
  explicit QTAChart (QWidget * parent = 0);
  ~QTAChart ();
  // valiables

  // functions
  void goBack (void);	// backBtn_clicked (void) implementation
  void loadFrames (FrameVector & data, QString symbol, QString name);	// populate chart's frame vector
  void loadData (QTAChartData data);	// load chart's data 
  void restoreBottomText (); // restores bottom text
  void setAlwaysRedraw (bool);	// always redraw the chart on/off
  void setTitle (QString title, QString subtitle);	// set the chart's title
  void setCustomBottomText (QString string); // set the bottom text to custom string
  void setSymbol (QString symbol); // set chart's symbol 
  void setSymbolKey (QString symbol); // set chart's symbol key 
  void setFeed (QString feed); // set symbol's data feed 
  void setLinear (bool); // set linear scale
  void showVolumes (bool);	// turn volumes show on/off
  void showOnlinePrice (bool);	// turn online price show on/off
  void showGrid (bool);		// turn grid on off

private:
  Ui::QTAChart * ui;

  // variables
  const void *chartdata;		// chart's data area

  // functions
private slots:
  void backBtn_clicked (void);
  void propertiesBtn_clicked (void);
  void helpBtn_clicked (void);
  void dataBtn_clicked (void);
  void zoomInBtn_clicked (void);
  void zoomOutBtn_clicked (void);
  void expandBtn_clicked (void);
  void drawBtn_clicked (void);
  void functionBtn_clicked (void);
  void objectsBtn_clicked (void);

protected:
  // functions
  virtual void resizeEvent (QResizeEvent * event);
  virtual void keyPressEvent (QKeyEvent * event);
  virtual void showEvent (QShowEvent * event);

  friend void *getData (QTAChart *);	// internal use
};

Q_DECLARE_TYPEINFO (QTAChart, Q_MOVABLE_TYPE);
#endif // QTACHART_H
