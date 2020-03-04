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

#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QWebFrame>
#include <QWebElement>
#include <QUrl>
#include <QResizeEvent>
#include "ui_manualdialog.h"
#include "manualdialog.h"
#include "defs.h"
#include "appdata.h"
#include "common.h"

// constructor
ManualDialog::ManualDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::ManualDialog)
{
  QFile manualtmpl, manual;	
  QString html, manualtmplpath, appwebpage;
  
  ui->setupUi (this);	
  setWindowTitle ("Manual");  
  
  appwebpage = "<a href=\"";
  appwebpage += APPWEBPAGE;
  appwebpage +=  "\">";
  appwebpage += APPWEBPAGE;
  appwebpage += "</a>";
  
  manualpath = QDir::homePath () + "/" +  ".config" + "/" + APPDIR + "/" + "manual.html";
  manualtmpl.setFileName (":/html/Manual.html");
  manualtmpl.open (QIODevice::ReadOnly|QIODevice::Text);
    
  manual.setFileName (manualpath);
  manual.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
  
  QTextStream in (&manualtmpl);
  QTextStream out (&manual);
  html = in.readLine (0);
  while (!in.atEnd ())
  {
	html.replace ("APPNAME", APPNAME);  
	html.replace ("VERSION_MAJOR", QString::number(VERSION_MAJOR));  
	html.replace ("VERSION_MINOR", QString::number(VERSION_MINOR));  
	html.replace ("VERSION_PATCH", QString::number(VERSION_PATCH));  
	html.replace ("APPWEBPAGE", appwebpage);  
	html.replace ("DBNAME", DBNAME);  
	html.replace ("APPDIR", APPDIR);  
	out << html << "\n";  
    html = in.readLine (0);
  }  
  manual.close ();
  manualtmpl.close ();
  // QFile::remove(manualpath);
  
  // ui->manualView->setUrl (QUrl (manualpath));
  ui->manualView->load (QUrl::fromLocalFile (manualpath));
  ui->manualView->page ()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks); 
  // ui->manualView->page ()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks); 
  ui->manualView->setTextSizeMultiplier ( ui->manualView->textSizeMultiplier () * 1.2 );
  connect(ui->closeButton, SIGNAL(clicked ()), this, SLOT(close_clicked ()));
  connect (ui->manualView, SIGNAL (linkClicked ( const QUrl & )), this,
             SLOT (link_clicked (const QUrl &)));
             
  correctWidgetFonts (this);   
  if (parent != NULL)
    setParent (parent);        
}

// destructor
ManualDialog::~ManualDialog ()
{
  QFile::remove(manualpath);	
  delete ui;	
}

/// events
// resize
void 
ManualDialog::resizeEvent (QResizeEvent * event)
{
  if (event->oldSize () == event->size ())
    return;
    	
  ui->line->setGeometry (0, height () - 45, width (), 1);
  ui->closeButton->move ((width () - 90) / 2, height () - 35);
  ui->manualView->setGeometry (10, 10, width () - 20, height () - 60);	
}

// change
void 
ManualDialog::changeEvent (QEvent * event)
{
  if (event->type()==QEvent::WindowStateChange && isMinimized())
    hide ();
}

/// slots
// close button
void 
ManualDialog::close_clicked ()
{
  hide ();	
}

void 
ManualDialog::link_clicked (const QUrl &url) 
{
  QDesktopServices::openUrl (url);	
}
