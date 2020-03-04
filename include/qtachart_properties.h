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

#ifndef QTACHART_PROPERTIES_H
#define QTACHART_PROPERTIES_H

#include <QWidget>
#include <QFrame>
#include <QPixmap>
#include <QIcon>
#include <QToolButton>
#include <QCheckBox>
#include <QColorDialog>

namespace Ui
{
  class QTACProperties;
}

class QTACProperties:public QWidget
{
Q_OBJECT public:
  explicit QTACProperties (QWidget * parent = 0);
  ~QTACProperties ();

  void setChartStyle (int);	// set chart's style
  int ChartStyle (void);	// return's chart style

  void setGrid (bool);		// set grid on/off
  bool Grid (void);		// returns grid state
  
  void setLinearScale (bool);		// set price scale to linear on/off
  bool LinearScale (void);	// return linear price scale state 
  
  void setOnlinePrice (bool);		// set online price on/off
  bool OnlinePrice (void);	// return online price state 

  QColor lineColor (void); // return the line color
  void setLineColor (QColor); // set line color 
  
  QColor barColor (void); // return the bar color
  void setBarColor (QColor); // set bar color 
  
  QColor foreColor (void); // return the foreground color
  void setForeColor (QColor); // set foreground color 
  
  QColor backColor (void); // return the background color
  void setBackColor (QColor); // set background color 

  void setVolumes (bool);	// set volumes on off
  bool Volumes (void);		// returns volumes state
  
  void setReferenceChart (void *chart);		// sets the reference chart

private:
  Ui::QTACProperties * ui;

  QColor linecolor;
  QColor barcolor;
  QColor forecolor;
  QColor backcolor;
  QButtonGroup *styleGroup;
  QCheckBox *showVolumes;
  QCheckBox *showGrid;
  QCheckBox *lineChart;
  QCheckBox *candleChart;
  QCheckBox *barChart;
  QCheckBox *heikinAshi;
  QCheckBox *linearScale;
  QCheckBox *onlinePrice;
  QPushButton *lineColorButton;
  QPushButton *barColorButton;
  QPushButton *foreColorButton;
  QPushButton *backColorButton;
  QPixmap *linepixmap, *barpixmap, *forepixmap, *backpixmap;
  QIcon *lineicon, *baricon, *foreicon, *backicon;
  QColorDialog *lineColorDialog;
  QColorDialog *barColorDialog;
  QColorDialog *foreColorDialog;
  QColorDialog *backColorDialog;
  QFrame *frame;
  void *referencechart;						// reference chart

private slots:
  void lineColorButton_clicked (void);
  void barColorButton_clicked (void);
  void lineColorDialog_finished ();
  void barColorDialog_finished ();
  
  void foreColorButton_clicked (void);
  void backColorButton_clicked (void);
  void foreColorDialog_finished ();
  void backColorDialog_finished ();

protected:
  virtual void resizeEvent (QResizeEvent * event);  
  virtual void showEvent (QShowEvent * event); 

};

#endif // QTACHART_PROPERTIES_H
