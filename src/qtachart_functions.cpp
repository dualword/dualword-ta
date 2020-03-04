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

// Full implementation of QTACFunctions
#include "common.h"
#include "qtachart_core.h"
#include "ui_qtacfunctions.h"
#include "qtachart_functions.h"

// constructor
QTACFunctions::QTACFunctions (QWidget * parent):
  QWidget (parent), ui (new Ui::QTACFunctions)
{
  ui->setupUi (this);
  
  button_width = 200;
  button_height = 40;
  
  layout = new QGridLayout(this);
  this->setLayout(layout);
  
  if (parent != NULL)
   setParent (parent);
}

static bool 
ButtonCmp(const QPushButton *b1, const QPushButton *b2)
{
     return b1->text() < b2->text();
}

void
QTACFunctions::createButtons (void)
{  
  QPushButton *btn;
  QColorDialog *colorDialog;
  DynParamsDialog *ParamDialog;
  	
  btn = addButton ("SMA");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 14.0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("EMA");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif    
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 14.0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("MACD");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 9.0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::red).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("MFI");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 14.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, 80);
  ParamDialog->addParam ("Medium level", "Medium level", DPT_INT, 50);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, 20);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("ROC");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 10.0);
  ParamDialog->addParam ("Level", "Level", DPT_INT, 0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("RSI");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 14.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, 70);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, 30);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("Slow Stoch");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif    
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 5.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, 80);
  ParamDialog->addParam ("Medium level", "Medium level", DPT_INT, 50);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, 20);
  ParamDialog->addParam ("%K color", "%K Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->addParam ("%D color", "%D Color", DPT_COLOR, (qreal) QColor (Qt::yellow).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("Fast Stoch");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 5.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, 80);
  ParamDialog->addParam ("Medium level", "Medium level", DPT_INT, 50);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, 20);
  ParamDialog->addParam ("%K color", "%K Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->addParam ("%D color", "%D Color", DPT_COLOR, (qreal) QColor (Qt::yellow).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("W%R");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 14.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, -20);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, -80);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("Bollinger Bands");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif    
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 20.0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::magenta).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("Parabolic SAR");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("ADX");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 13.0);
  ParamDialog->addParam ("Weak", "Weak", DPT_INT, 25);
  ParamDialog->addParam ("Strong", "Strong", DPT_INT, 50);
  ParamDialog->addParam ("Very strong", "Very strong", DPT_INT, 75);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("Aroon");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 25.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, 70);
  ParamDialog->addParam ("Medium level", "Medium level", DPT_INT, 50);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, 30);
  ParamDialog->addParam ("Up color", "UP Color", DPT_COLOR, (qreal) QColor (Qt::green).rgb ());
  ParamDialog->addParam ("Down color", "Down Color", DPT_COLOR, (qreal) QColor (Qt::red).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));  
  
  btn = addButton ("CCI");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 20.0);
  ParamDialog->addParam ("High level", "High level", DPT_INT, 100);
  ParamDialog->addParam ("Low level", "Low level", DPT_INT, -100);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("STDDEV");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 10.0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("Momentum");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 12.0);
  ParamDialog->addParam ("Level", "Level", DPT_INT, 0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("DMI");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 13.0);
  ParamDialog->addParam ("Weak", "Weak", DPT_INT, 25);
  ParamDialog->addParam ("Strong", "Strong", DPT_INT, 50);
  ParamDialog->addParam ("Very strong", "Very strong", DPT_INT, 75);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  btn = addButton ("ATR");
  connect (btn, SIGNAL (clicked ()), this, SLOT (button_clicked ()));
  ParamDialog = new DynParamsDialog (btn);
#ifdef Q_OS_WIN  
  colorDialog = new QColorDialog (parentchart);
#else
  colorDialog = new QColorDialog; // (parentchart);
#endif  
  ParamDialog->setColorDialog (colorDialog);
  ParamDialog->addParam ("Period", "Period", DPT_INT, 14.0);
  ParamDialog->addParam ("Color", "Color", DPT_COLOR, (qreal) QColor (Qt::cyan).rgb ());
  ParamDialog->setObjectName ("ParamDialog"); 
  // ParamDialog->setParent (btn);
  connect(ParamDialog->buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect(ParamDialog->buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected()));
  
  // sort buttons
  qSort(Button.begin(), Button.end(), ButtonCmp);
}

// destructor
QTACFunctions::~QTACFunctions ()
{
  
  delete ui;
}

// add a button
QPushButton *
QTACFunctions::addButton (QString text)
{
  QFont fnt;
  QPushButton *btn;
  QString stylesheet;

  stylesheet =
    "background: transparent; border: 1px solid transparent;border-color: darkgray;";
  
  btn = new QPushButton (text, this);
  btn->setFixedSize (QSize (button_width, button_height));
  fnt = btn->font ();
  fnt.setPixelSize (16);
  fnt.setBold (true);
  btn->setFont (fnt);
  btn->setStyleSheet (stylesheet);
  btn->setAutoFillBackground (false);
  btn->setFocusPolicy (Qt::NoFocus);
  Button += btn;
  return btn;
}

// set the reference chart
void
QTACFunctions::setReferenceChart (void *chart)
{
  referencechart = chart;
  parentchart = reinterpret_cast<QTAChart *> (chart); 
  createButtons ();
}

// get the reference chart
void *
QTACFunctions::getReferenceChart (void)
{
  return referencechart;
}

// add indicator
void
QTACFunctions::addIndicator (DynParamsDialog *paramDialog)
{
  QTAChart *chart = reinterpret_cast<QTAChart *> (referencechart);
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> (getData (chart));
  QTACObject *indicator = NULL, *childobj;	
  QString fname;
  qint32 period;
  QColor color;
  	
  fname = paramDialog->getTitle ();
  
  if (fname == "SMA")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_CURVE);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", SMA, 0, 0, color, "Color");
	indicator->setTitle (fname);
  }
  
  if (fname == "EMA")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;
	color = paramDialog->getParam ("Color");  
	indicator = new QTACObject (core, QTACHART_OBJ_CURVE);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", EMA, 0, 0, color, "Color");
	indicator->setTitle (fname);
  }
  
  if (fname == "Parabolic SAR")
  {
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_DOT);
	indicator->setAttributes (QTACHART_CLOSE, 1, "", PSAR, 0, 0, color, "Color");
	indicator->setTitle (fname);  
  }
  
  if (fname == "RSI")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, 0, 100, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", RSI, 0, 100, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, 0, 100, color, "");
  }
  
  if (fname == "MFI")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, 0, 100, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", MFI, 0, 100, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Medium level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Medium level", DUMMY, 0, 100, color, "");
  }
  
  if (fname == "ROC")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", ROC, QREAL_MIN, QREAL_MAX, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", ROC, QREAL_MIN, QREAL_MAX, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Level", DUMMY, 0, 0, color, "");
  }
  
  if (fname == "W%R")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, -100, 0, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", WILLR, -100, 0, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, -100, 0, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, -100, 0, color, "");
  }
  
  if (fname == "Slow Stoch")
  {
	QColor colorD, colorK;  
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	colorD = paramDialog->getParam ("%D color");   
	colorK = paramDialog->getParam ("%K color");
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, 0, 100, colorD, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", STOCHSLOWD, 0, 100, colorD, "%D color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", STOCHSLOWK, 0, 100, colorK, "%K color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, 0, 100, colorK, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, 0, 100, colorK, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Medium level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Medium level", DUMMY, 0, 100, colorK, "");
  }
  
  if (fname == "Fast Stoch")
  {
	QColor colorD, colorK;  
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	colorD = paramDialog->getParam ("%D color");   
	colorK = paramDialog->getParam ("%K color");
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, 0, 100, colorD, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", STOCHFASTD, 0, 100, colorD, "%D color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", STOCHFASTK, 0, 100, colorK, "%K color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, 0, 100, colorK, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, 0, 100, colorK, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Medium level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Medium level", DUMMY, 0, 100, colorK, "");
  }
  
  if (fname == "MACD")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	 
	color = paramDialog->getParam ("Color");
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", MACDSIGNAL, QREAL_MIN, QREAL_MAX, QColor (Qt::white).rgb (), "");
	indicator->setTitle (fname); 
	childobj = new QTACObject (indicator, QTACHART_OBJ_VBARS);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", MACD, QREAL_MIN, QREAL_MAX, QColor (Qt::white).rgb (), ""); 
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", MACDSIGNAL, QREAL_MIN, QREAL_MAX, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, 0);
    childobj->setAttributes (QTACHART_CLOSE, 0, "", DUMMY, 0, 0, color, "");
  }
  
  if (fname == "Bollinger Bands")
  {
    period = (qint32) paramDialog->getParam ("Period");
    if (period < 1)
      return;
    color = paramDialog->getParam ("Color");   
    indicator = new QTACObject (core, QTACHART_OBJ_CURVE);
    indicator->setAttributes (QTACHART_CLOSE, period, "Period", BBANDSMIDDLE, 0, 0, color, "Color");
    indicator->setTitle (fname);  
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", BBANDSUPPER, 0, 0, color, "Color");
    // childobj->setParamDialog (paramDialog->getPVector (), fname);
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", BBANDSLOWER, 0, 0, color, "Color");
    // childobj->setParamDialog (paramDialog->getPVector (), fname);
  }
  
  if (fname == "ADX")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", ADX, 0, 100, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", ADX, 0, 100, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Weak"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Weak", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Strong"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Strong", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Very strong"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Very strong", DUMMY, 0, 100, color, "");
  }
  
  if (fname == "Aroon")
  {
	QColor colorUp, colorDown;  
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	colorUp = paramDialog->getParam ("Up color");   
	colorDown = paramDialog->getParam ("Down color");
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, 0, 100, colorDown, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", AROONDOWN, 0, 100, colorDown, "Down color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", AROONUP, 0, 100, colorUp, "Up color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, 0, 100, colorUp, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, 0, 100, colorUp, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Medium level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Medium level", DUMMY, 0, 100, colorUp, "");
  }
  
  if (fname == "CCI")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, QREAL_MIN, QREAL_MAX, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", CCI, QREAL_MIN, QREAL_MAX, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("High level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "High level", DUMMY, QREAL_MIN, QREAL_MAX, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Low level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Low level", DUMMY, QREAL_MIN, QREAL_MAX,
     color, "");
  }
  
  if (fname == "STDDEV")
  {
    period = (qint32) paramDialog->getParam ("Period");
    if (period < 1)
      return;  
    color = paramDialog->getParam ("Color");   
    indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
    indicator->setAttributes (QTACHART_CLOSE, period, "Period", DUMMY, 0, QREAL_MAX, color, "");
    indicator->setTitle (fname);  
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", STDDEV, 0, QREAL_MAX, color, "Color");
  }
  
  if (fname == "Momentum")
  {
	period = (qint32) paramDialog->getParam ("Period");
	if (period < 1)
	  return;  
	color = paramDialog->getParam ("Color");   
	indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
	indicator->setAttributes (QTACHART_CLOSE, period, "Period", MOMENTUM, QREAL_MIN, QREAL_MAX, color, "");
	indicator->setTitle (fname);  
	childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", MOMENTUM, QREAL_MIN, QREAL_MAX, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Level"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Level", DUMMY, 0, 0, color, "");
  }
  
  if (fname == "DMI")
  {
    period = (qint32) paramDialog->getParam ("Period");
    if (period < 1)
     return;  
    color = paramDialog->getParam ("Color");   
    indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
    indicator->setAttributes (QTACHART_CLOSE, period, "Period", DMI, 0, 100, color, "");
    indicator->setTitle (fname);  
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", DMI, 0, 100, color, "Color");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Weak"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Weak", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Strong"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Strong", DUMMY, 0, 100, color, "");
    childobj = new QTACObject (indicator, QTACHART_OBJ_HLINE);
    childobj->setHLine (NULL, paramDialog->getParam ("Very strong"));
    childobj->setAttributes (QTACHART_CLOSE, period, "Very strong", DUMMY, 0, 100, color, "");
  }
  
  if (fname == "ATR")
  {
    period = (qint32) paramDialog->getParam ("Period");
    if (period < 1)
      return;  
    color = paramDialog->getParam ("Color");   
    indicator = new QTACObject (core, QTACHART_OBJ_SUBCHART);
    indicator->setAttributes (QTACHART_CLOSE, period, "Period", ATR, 0, QREAL_MAX, color, "");
    indicator->setTitle (fname);  
    childobj = new QTACObject (indicator, QTACHART_OBJ_CURVE);
    childobj->setAttributes (QTACHART_CLOSE, period, "Period", ATR, 0, QREAL_MAX, color, "Color");
  }
  
  if (indicator != NULL)
    indicator->setParamDialog (paramDialog->getPVector (), fname);	
}

// resize
void
QTACFunctions::resizeEvent (QResizeEvent * event)
{
  DynParamsDialog *paramDialog;
  QSize newsize;
  int w, h, counter;
  
  if (event->oldSize () == event->size ())
    return;
  
  newsize = event->size ();
  w = newsize.width () - 2;
  h = newsize.height () - 2;
  layout->setGeometry (QRect (0, 0, w, h));
  for (counter = 0; counter < Button.size (); counter ++)
  {
	layout->setRowMinimumHeight (counter % 6, Button[counter]->height ());
    layout->setColumnMinimumWidth (counter/6, Button[counter]->width ());  
    layout->addWidget (Button[counter], counter % 6, counter / 6, Qt::AlignHCenter);
    paramDialog = Button[counter]->findChild<DynParamsDialog *>("ParamDialog");
    if (paramDialog != NULL)
    {
	  paramDialog->move ((width () - paramDialog->width ()) / 2, 25);	
    }
  }  
}

// button clicked
void
QTACFunctions::button_clicked (void)
{
  DynParamsDialog *paramDialog;	
  QPushButton *btn;	
  btn = (QPushButton *) QObject::sender();
  paramDialog = btn->findChild<DynParamsDialog *>("ParamDialog");
  if (paramDialog != NULL)
  {
    paramDialog->setReferenceChart (referencechart);  
	paramDialog->move ((width () - paramDialog->width ()) / 2, 25);	  
    paramDialog->show ();
  }  
}

// function accepted
void
QTACFunctions::function_accepted (void)
{
  QTAChart *chart = reinterpret_cast<QTAChart *> (referencechart);	
  DynParamsDialog *paramDialog;		
   
  paramDialog = reinterpret_cast<DynParamsDialog *> (QObject::sender()->parent ());
  addIndicator (paramDialog);
  
  chart->goBack ();
}

// function rejected
void
QTACFunctions::function_rejected (void)
{
  
}

