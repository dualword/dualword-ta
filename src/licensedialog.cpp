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

#include <QResizeEvent>
#include "ui_licensedialog.h"
#include "licensedialog.h"
#include "appdata.h"
#include "common.h"

// constructor
LicenseDialog::LicenseDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::LicenseDialog)
{
  QString html;	
  ui->setupUi (this);	
  setWindowTitle ("E.U.L.A.");  
  
  html = ui->licenseBrowser->toHtml ();
  html.replace("APPAUTHOR", APPAUTHOR);
  html.replace("APPEMAIL", APPEMAIL);
  html.replace("APPNAME", APPNAME);
  html.replace("Authors", "Author");
  html.replace ("VERSION_MAJOR", QString::number(VERSION_MAJOR));  
  html.replace ("VERSION_MINOR", QString::number(VERSION_MINOR));  
  html.replace ("VERSION_PATCH", QString::number(VERSION_PATCH));  
  ui->licenseBrowser->setHtml (html);
  
  connect(ui->closeButton, SIGNAL(clicked ()), this, SLOT(close_clicked ()));
  correctWidgetFonts (this);
  if (parent != NULL)
    setParent (parent);
}

// destructor
LicenseDialog::~LicenseDialog ()
{
  delete ui;	
}

/// events
// resize
void 
LicenseDialog::resizeEvent (QResizeEvent * event)
{
  if (event->oldSize () == event->size ())
    return;
    	
  ui->closeButton->move (275, height () - 40);
  ui->licenseBrowser->setGeometry (10, 10, 620, height () - 60);	
}

// change
void 
LicenseDialog::changeEvent (QEvent * event)
{
  if (event->type()==QEvent::WindowStateChange && isMinimized())
    hide ();
}

/// slots
// close button
void 
LicenseDialog::close_clicked ()
{
  hide ();	
}
