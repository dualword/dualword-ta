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

#include "optsize.h"

#include <QDesktopServices>
#include <QNetworkProxy>
#include <QCloseEvent>
#include <QShowEvent>
#include <string.h>
#include <QHideEvent>
#include "ui_optionsdialog.h"
#include "optionsdialog.h"
#include "common.h"
#include "mainwindow.h"

// constructor
OptionsDialog::OptionsDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::OptionsDialog)
{
  QPushButton *btn;
  QFont font;	
  	
  ui->setupUi (this);
  
  btn = ui->buttonBox->button (QDialogButtonBox::Ok);
  font = btn->font ();
  font.setWeight (QFont::DemiBold);
  btn->setFont (font);
  btn->setFocusPolicy (Qt::NoFocus);
  
  btn = ui->buttonBox->button (QDialogButtonBox::Cancel);
  font = btn->font ();
  font.setWeight (QFont::DemiBold);
  btn->setFont (font);
  btn->setFocusPolicy (Qt::NoFocus);
  
  ui->tabWidget->setStyleSheet("QTabBar::tab { height: 25px;}");
  connect(ui->buttonBox, SIGNAL(accepted ()), this, SLOT(ok_clicked ()));
  connect(ui->buttonBox, SIGNAL(rejected ()), this, SLOT(cancel_clicked ()));
  connect(ui->newVersionButton, 
          SIGNAL(clicked ()), this, SLOT(newVersionBtn_clicked ()));        
  
  setWindowFlags (((this->windowFlags() | Qt::CustomizeWindowHint)
                        & ~Qt::WindowCloseButtonHint));
    
  correctWidgetFonts (this);
  if (parent != NULL)
    setParent (parent);
    
  loadOptions ();  
}

// destructor
OptionsDialog::~OptionsDialog ()
{
  // saveOptions ();	
  delete ui;
}

// load application options
void
OptionsDialog::loadOptions ()
{
  CG_ERR_RESULT result;	
  
  result = loadAppOptions (&Application_Settings->options);
  if (result != CG_ERR_OK)
  {
     showMessage (errorMessage (result));
     this->hide ();
     return;   
  }
  
  for (qint32 counter = 0; counter <ui->tabWidget->count (); counter ++)
  {
	QObjectList objectlist;
       
    objectlist = ui->tabWidget->widget (counter)->children ();
    foreach (const QObject *object, objectlist)
    {
	  QString objName = object->objectName ();
	  
	  if (objName == "splashCheckBox") 
	  {
	    if (Application_Settings->options.showsplashscreen)
	      ((QCheckBox *) object)->setCheckState (Qt::Checked);
	    else
	      ((QCheckBox *) object)->setCheckState (Qt::Unchecked);
	  }
	  
	  if (objName == "newVersionCheckBox") 
	  {
	    if (Application_Settings->options.checknewversion)
	      ((QCheckBox *) object)->setCheckState (Qt::Checked);
	    else
	      ((QCheckBox *) object)->setCheckState (Qt::Unchecked);
	  }
	  
	  if (objName == "londonPricesCheckBox") 
	  {
	    if (Application_Settings->options.longbp)
	      ((QCheckBox *) object)->setCheckState (Qt::Checked);
	    else
	      ((QCheckBox *) object)->setCheckState (Qt::Unchecked);
	  }
	  
	  if (objName == "enableProxyBox") 
	  {
	    if (Application_Settings->options.enableproxy)
	      ((QCheckBox *) object)->setCheckState (Qt::Checked);
	    else
	      ((QCheckBox *) object)->setCheckState (Qt::Unchecked);        
	  }
	  
	  if (objName == "proxyHostEdit")
	    ((QLineEdit *) object)->setText (Application_Settings->options.proxyhost);   
	  
	  if (objName == "proxyUserEdit")
	    ((QLineEdit *) object)->setText (Application_Settings->options.proxyuser);     
	  
	  if (objName == "proxyPasswordEdit")
	    ((QLineEdit *) object)->setText (Application_Settings->options.proxypass);  
	  
	  if (objName == "proxyPortSpinBox")
	    ((QSpinBox *) object)->setValue (Application_Settings->options.proxyport);  
	  
	  if (objName == "netTimeOutSpinBox")
	    ((QSpinBox *) object)->setValue (Application_Settings->options.nettimeout);     
    }		
  }
  
  QNetworkProxy proxy;
  if (!Application_Settings->options.enableproxy)
    proxy.setType(QNetworkProxy::NoProxy);
  else
    proxy.setType(QNetworkProxy::HttpProxy);  
  
  proxy.setHostName(Application_Settings->options.proxyhost);
  proxy.setPort(Application_Settings->options.proxyport);
  proxy.setUser(Application_Settings->options.proxyuser);
  proxy.setPassword(Application_Settings->options.proxypass);
  QNetworkProxy::setApplicationProxy(proxy);  
}

// save application options
void
OptionsDialog::saveOptions ()
{
  QObjectList objectlist;
  CG_ERR_RESULT result;	
  
  for (qint32 counter = 0; counter <ui->tabWidget->count (); counter ++)
  {
    objectlist = ui->tabWidget->widget (counter)->children ();
    foreach (const QObject *object, objectlist)
    {
	  if (object->objectName () == "splashCheckBox") 
	  {
	    if (((QCheckBox *) object)->checkState () == Qt::Checked)
	      Application_Settings->options.showsplashscreen = true;
	    else
	      Application_Settings->options.showsplashscreen = false;
	  }
	  
	  if (object->objectName () == "newVersionCheckBox") 
	  {
	    if (((QCheckBox *) object)->checkState () == Qt::Checked)
	      Application_Settings->options.checknewversion = true;
	    else
	      Application_Settings->options.checknewversion = false; 
	  }    
	  
	  if (object->objectName () == "londonPricesCheckBox") 
	  {
	    if (((QCheckBox *) object)->checkState () == Qt::Checked)
	      Application_Settings->options.longbp = true;
	    else
	      Application_Settings->options.longbp = false; 
	  }
	  
	  if (object->objectName () == "enableProxyBox") 
	  {
	    if (((QCheckBox *) object)->checkState () == Qt::Checked)
	      Application_Settings->options.enableproxy = true;
	    else
	      Application_Settings->options.enableproxy = false;        
	  }
	  
	  if (object->objectName () == "proxyHostEdit")
	    Application_Settings->options.proxyhost = ((QLineEdit *) object)->text ().trimmed (); 
	  
	  if (object->objectName () == "proxyUserEdit")
	    Application_Settings->options.proxyuser = ((QLineEdit *) object)->text ().trimmed ();    
	  
	  if (object->objectName () == "proxyPasswordEdit")
	    Application_Settings->options.proxypass = ((QLineEdit *) object)->text ().trimmed ();    
	  
	  if (object->objectName () == "proxyPortSpinBox")
	    Application_Settings->options.proxyport = (qint16) ((QSpinBox *) object)->value ();    
	  
	  if (object->objectName () == "netTimeOutSpinBox")
	    Application_Settings->options.nettimeout = (qint16) ((QSpinBox *) object)->value ();      
    }		
  }	
  
  result = saveAppOptions (&Application_Settings->options);
  if (result != CG_ERR_OK)
  {
     showMessage (errorMessage (result));
     this->hide ();
     return;   
  }
}	

// signals
void
OptionsDialog::ok_clicked ()
{
  saveOptions ();
  loadOptions ();
  this->hide ();
}

void
OptionsDialog::cancel_clicked ()
{
  loadOptions ();	
  this->hide ();
}

void
OptionsDialog::newVersionBtn_clicked ()
{
  MainWindow *mainwindow;

  mainwindow = reinterpret_cast <MainWindow *> (parentWidget ());
  mainwindow->checkNewVersion ();
  delay (2);
  if (mainwindow->newversion)
  {
    if (showDownloadMessage ())
      QDesktopServices::openUrl (QUrl (APPWEBPAGE));  
  }
  else
    showMessage ("You run the current version for your platform.");   
}

// events
void 
OptionsDialog::closeEvent (QCloseEvent * event)
{
  saveOptions ();
  loadOptions ();
}

void 
OptionsDialog::showEvent (QShowEvent * event)
{
  
}
