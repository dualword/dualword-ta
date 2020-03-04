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

#include "ui_infodialog.h"
#include "infodialog.h"
#include "manualdialog.h"
#include "appdata.h"
#include "common.h"

// constructor
InfoDialog::InfoDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::InfoDialog)
{
  QString appname, version, appurl;	
  ui->setupUi (this);	
  
  appname = APPNAME;
  version =  "Version " + 
             QString::number(VERSION_MAJOR,10) + "." +
             QString::number(VERSION_MINOR,10) + "." +
             QString::number(VERSION_PATCH,10);
  
  appurl =  "<html></style></head>";
  appurl += "<body style=\" font-family:'Sans Serif'; font-size:10pt;"; 
  appurl += "font-weight:600; font-style:normal;\">";
  appurl += "<p align=\"center\" style=\" margin-top:0px;";
  appurl += "margin-bottom:0px; margin-left:0px; margin-right:0px;";
  appurl += "-qt-block-indent:0; text-indent:0px;\">";
  appurl += "<a href=\"";
  appurl += APPWEBPAGE;
  appurl += "\">";
  appurl += "<span style=\" font-size:12pt; font-weight:bold;";
  appurl += "text-decoration: underline; color:#0000ff;\">";
  appurl += APPWEBPAGE;
  appurl += "</span></a></p></body></html>";
        
  setWindowTitle ("Information");           
  ui->appnameLabel->setText (appname);
  ui->versionLabel->setText (version);
  ui->copyrightLabel->setText (APPCOPYRIGHT);
  ui->urlLabel->setText (appurl);
  ui->osLabel->setText (fullOperatingSystemVersion ());
  
  licensedlg = new LicenseDialog (this);
  manualdlg = new ManualDialog (this);
  manualdlg->setWindowModality (Qt::NonModal);
  
  connect(ui->closeButton, SIGNAL(clicked ()), this, SLOT(close_clicked ()));
  connect(ui->licenseButton, SIGNAL(clicked ()), this, SLOT(license_clicked ()));
  connect(ui->manualButton, SIGNAL(clicked ()), this, SLOT(manual_clicked ()));
  correctWidgetFonts (this);
  
  if (parent != NULL)
    setParent (parent);
}

// destructor
InfoDialog::~InfoDialog ()
{
  delete ui;	
}

// events

// change
void 
InfoDialog::changeEvent (QEvent * event)
{
  if (event->type()==QEvent::WindowStateChange && isMinimized())
    hide ();
}

/// slots
// license button
void
InfoDialog::license_clicked ()
{
  licensedlg->show ();	
}

// manual button
void
InfoDialog::manual_clicked ()
{
  manualdlg->show ();
  manualdlg->raise ();	
}

// close button
void 
InfoDialog::close_clicked ()
{
  hide ();	
}
