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
#include <QDesktopWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QShowEvent>
#include "ui_dynparamsdialog.h"
#include "dynparamsdialog.h"
#include "qtachart_functions.h"

// constructor
DPColorButton::DPColorButton (QWidget * parent, int pidx, int bidx)
{
  if (parent != NULL)	
    setParent (parent);
  paramidx = pidx;	
  buttonidx = bidx;
}

// destructor
DPColorButton::~DPColorButton ()
{
  
}

// constructor
DynParamsDialog::DynParamsDialog (QWidget * parent):
QDialog (parent), ui (new Ui::DynParamsDialog)
{
  DynParamsDialog_constructor_body ();	
  ui->title->setText ((((QPushButton *) parent)->text ()));
  ui->removeLbl->setVisible (false);
  ui->removeCheckBox->setVisible (false);
  setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
  modify = false;
  
  if (parent != NULL)	
    setParent (parent);
}

// constructor for modify 
DynParamsDialog::DynParamsDialog (ParamVector PVector, QString title):
QDialog (), ui (new Ui::DynParamsDialog)
{
  DynParamsDialog_constructor_body ();	
  ui->title->setText (title);
  ui->removeLbl->setVisible (true);
  ui->removeCheckBox->setVisible (true);
  foreach (const DynParam *param, PVector)
    addParam (param->paramName, param->label, param->type, param->value);
  setWindowFlags(Qt::CustomizeWindowHint); 
  modify = true; 
}  

void
DynParamsDialog::DynParamsDialog_constructor_body ()
{
  QString stylesheet = "background-color: lightgray",
          boxstylesheet = "background-color: lightgray; color: black; border: 1px solid transparent;border-color: black;",
          lblstylesheet = "background-color: lightgray; color: black; border: 1px solid transparent;border-color: lightgray;",
          chkbxstylesheet = "border: 1px solid transparent; border-color: lightgray; color: black;";
  
  ui->setupUi (this); 
  ui->buttonBox->setStyleSheet (boxstylesheet);
  ui->buttonBox->setFixedSize (QSize (380, 32));
  buttonBox = ui->buttonBox;
  removeCheckBox = ui->removeCheckBox;
                                      
  foreach (QAbstractButton *button, ui->buttonBox->buttons ())
  {
    button->setFixedSize (QSize (button->width (), button->height ()));
    button->setFocusPolicy (Qt::NoFocus);
  }
  
  ui->title->setStyleSheet (boxstylesheet);
  ui->removeLbl->setStyleSheet (lblstylesheet);
  ui->removeCheckBox->setStyleSheet (chkbxstylesheet);

  setStyleSheet (stylesheet);	
  param_height = 40;	
  ncolorbuttons = 0;
  colorDialog = NULL;
  connect (buttonBox, SIGNAL(accepted ()), this, SLOT(function_accepted()));
  connect (buttonBox, SIGNAL(rejected ()), this, SLOT(function_rejected())); 
}

// destructor
DynParamsDialog::~DynParamsDialog ()
{
  DynParam *param;
  QPixmap *pixmap;
  QIcon *icon;
  
  foreach (icon, Icon)
    delete icon;
  
  foreach (pixmap, Pixmap)
    delete pixmap;
    
  foreach (param, Param)
    delete param;
  
  if (colorDialog != NULL)
    delete colorDialog;  
      
  delete ui;
}

// add a parameter
void 
DynParamsDialog::addParam (QString paramName, QString label, 
                           qint32 type, qreal defvalue)
{
  QString 
    stylesheet = "background-color: lightgray",
    lblstylesheet = "background-color: lightgray; color: black; border: 1px solid transparent;border-color: lightgray;",
    editstylesheet = "background-color: white; color: black; border: 1px solid transparent; border-color: black;";
  QLabel *lbl;
  QLineEdit *edit;
  DynParam *param;	
  QPixmap *pixmap;
  DPColorButton *colorbutton;
  QIcon *icon;
  QFont font;
  qint32 h;
  
  param = new DynParam;
  param->paramName = paramName;
  param->label = label;
  param->type = type;
  param->defvalue = defvalue;
  param->value = defvalue;
  Param += param;  
  
  lbl = new QLabel (label, this, Qt::Widget);
  font = lbl->font ();
  font.setWeight (QFont::Bold);
  lbl->setFont (font);
  lbl->setGeometry (10, Param.size () * param_height, 190, 32);
  lbl->setStyleSheet (lblstylesheet); 
  
  if (type == DPT_INT)
  {
	edit = new QLineEdit (QString::number (param->value, 'f', 0), this);
	font = edit->font ();
	font.setWeight (QFont::Bold);
	edit->setFont (font);
	edit->setGeometry (210, Param.size () * param_height, 50, 32);   
	edit->setStyleSheet (editstylesheet); 
	edit->setInputMask ("####");
	edit->setObjectName (label);
	edit->setFocusPolicy (Qt::StrongFocus);
	connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(text_changed(QString)));
  }
  
  if (type == DPT_REAL)
  {
	edit = new QLineEdit (QString::number (param->value, 'f', 4), this);
	font = edit->font ();
	font.setWeight (QFont::Bold);
	edit->setFont (font);
	edit->setGeometry (210, Param.size () * param_height, 120, 32);   
	edit->setStyleSheet (editstylesheet); 
	edit->setInputMask ("###9.9999");
	edit->setObjectName (label);
	edit->setFocusPolicy (Qt::StrongFocus);
	connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(text_changed(QString)));
  }
  
  if (type == DPT_COLOR)
  {
	colorbutton = new DPColorButton (this, Param.size () - 1, ncolorbuttons); 
	colorbutton->setFocusPolicy (Qt::StrongFocus); 
	pixmap = new QPixmap (24, 24);
    icon = new QIcon;
    pixmap->fill (Qt::blue);
    icon->addPixmap (*pixmap, QIcon::Normal, QIcon::On);
    colorbutton->setIcon (*icon); 
    colorbutton->setGeometry (210, Param.size () * param_height, 32, 32);   
	colorbutton->setStyleSheet (editstylesheet);
	colorbutton->setObjectName (label);  
    Pixmap += pixmap;
    Icon += icon;
    Button += colorbutton;
    ncolorbuttons ++;
    connect (colorbutton, SIGNAL(clicked (bool)), this, SLOT(color_clicked(void)));	
  }	
  
  h =  (Param.size () + 2) * param_height;
  resize (400, h);
  setMaximumSize (width (), h);
  setMinimumSize (width (), h);
}

// get title label
QString 
DynParamsDialog::getTitle ()
{
  return ui->title->text ();	
}

// get parameter data
qreal
DynParamsDialog::getParam (QString paramName)
{
  DynParam *param;
  
  foreach (param, Param)
  {
    if (param->paramName == paramName)
      return param->value;
   }
   return 0;   
}

// get parameter vector
ParamVector 
DynParamsDialog::getPVector ()
{
  return Param;	
}

// set the reference chart
void
DynParamsDialog::setReferenceChart (void *chart)
{
  referencechart = chart;
}

void 
DynParamsDialog::setColorDialog (QColorDialog *dialog)
{
  colorDialog = dialog;
  colorDialog->setModal (true);
  connect (colorDialog, SIGNAL (accepted ()), this, SLOT (colorDialog_accepted ()));
}

// slots
void
DynParamsDialog::color_clicked (void)
{
  DPColorButton *btn;	
  
  btn = reinterpret_cast <DPColorButton *> (QObject::sender());
  colorDialog->setCurrentColor (Param[btn->paramidx]->value);

#ifdef Q_OS_WIN    
  colorDialog->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                         colorDialog->size(), qApp->desktop()->availableGeometry()));  
#endif
  
  colorDialog->show ();
  colorDialog->open ();
  paramidx = btn->paramidx;
  cbidx = btn->buttonidx;
}

void
DynParamsDialog::colorDialog_accepted ()
{
  QColor color;
  
  color = colorDialog->currentColor ();
  Pixmap[cbidx]->fill (color);
  Icon[cbidx]->addPixmap (*Pixmap[cbidx], QIcon::Normal, QIcon::On);
  Param[paramidx]->value = (qreal) color.rgb ();
  Button[cbidx]->setIcon (*Icon[cbidx]);
}

// function accepted
void
DynParamsDialog::function_accepted (void)
{
  DynParam *param;	
  
  foreach (param, Param)
  {
	if (param->paramName == "Period" && param->value < 2)
	  param->value = 2;
	  
    param->defvalue = param->value;
  }
}

// function rejected
void
DynParamsDialog::function_rejected (void)
{
   
}

void
DynParamsDialog::text_changed(QString)
{
  DynParam *param;		
  QLineEdit *edit;
  QString objname;
  bool ok;
  
  edit = (QLineEdit *) QObject::sender();	
  objname = edit->objectName ();
  foreach (param, Param)
    if (param->paramName == objname)
      param->value = edit->text ().toFloat (&ok);
}

// show event
void 
DynParamsDialog::showEvent (QShowEvent * event)
{
  const DynParam *param;	
  QLineEdit *edit;
  qint32 bidx = 0;
  qint32 h;
  
  if (event->spontaneous ())	
    return;
  
  if (modify)
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                        this->size(), qApp->desktop()->availableGeometry()));
                                        
  h =  (Param.size () + 2) * param_height;
  if (ui->removeLbl->isVisible ())
  {
    h += param_height;
    ui->removeLbl->move (10, (Param.size () + 1) * param_height);
    ui->removeCheckBox->move (210, (Param.size () + 1) * param_height + 5); 
    ui->buttonBox->move (10, (Param.size () + 2) * param_height);  
    ui->removeCheckBox->setCheckState (Qt::Unchecked); 
  }  
  else
    ui->buttonBox->move (10, (Param.size () + 1) * param_height);
      
  resize (400, h);
  setMaximumSize (width (), h);
  setMinimumSize (width (), h);
  
  foreach (param, Param)
  {
	if (param->type == DPT_INT)
	{
	   edit = findChild<QLineEdit *>(param->label);	
	   edit->setText (QString::number (param->defvalue, 'f', 0));
	   edit->clearFocus ();
    }
    
    if (param->type == DPT_REAL)
	{
	   edit = findChild<QLineEdit *>(param->label);	
	   edit->setText (QString::number (param->defvalue, 'f', 4));
	   edit->clearFocus ();
    }  
    
    if (param->type == DPT_COLOR)
    {
	  Pixmap[bidx]->fill (QColor ((QRgb) param->defvalue));
      Icon[bidx]->addPixmap (*Pixmap[bidx], QIcon::Normal, QIcon::On);
      Button[bidx]->setIcon (*Icon[bidx]);	
      Button[bidx]->clearFocus ();
      bidx ++;
    }	
  }  
}
