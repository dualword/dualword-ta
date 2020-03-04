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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include "common.h"
#include "optionsdialog.h"
#include "infodialog.h"
#include "templatemanagerdialog.h"

typedef class _ChartTab ChartTab;

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
  QTabWidget *tabWidget;  // tabwidget;
  bool newversion;						// new version flag
  
  CG_ERR_RESULT addChart (TableDataVector & datavector);	// add a new chart using tablename data
  void setExpandChart (bool);
  void checkNewVersion ();				// check if there is new version available
  bool expandedChart ();

private:
  Ui::MainWindow *ui;
  AppSettings appsettings;				// application setting
  SQLists comboitems;					// string lists used to populate
  //   various comboboxes
  LoadCSVDialog *loadcsvdlg;			// dialog to load CSV file
  DownloadDataDialog *downloaddatadlg;	// dialog to download data
  DataManagerDialog *datamanagerdlg;	// data manager
  TemplateManagerDialog *templatemanagerdlg; // template manager 
  ProgressDialog *progressdlg;		    // progress dialog
  WaitDialog *waitdlg;					// wait dialog
  OptionsDialog *optionsdlg;			// options dialog
  InfoDialog *infodlg;					// options dialog
  bool expandedChartFlag;				// expanded = true, normal = false;
  QNetworkAccessManager *UpdateManager; // network manager to check for updates
  QNetworkRequest request;				 // network request
  bool closed;							 // true if application is already closed
  
  void loadFonts ();					// load application fonts
private slots:
  void managerButton_clicked ();
  void screenshotButton_clicked ();
  void optionsButton_clicked ();
  void infoButton_clicked ();
  void homeButton_clicked ();
  void forumButton_clicked ();
  void exitButton_clicked ();
  void closeTab_clicked (int);
  void currentTab_changed (int);
  void updateReplyFinished (QNetworkReply * updatereply);
  void closing ();

protected:
  virtual void resizeEvent (QResizeEvent * event);
  virtual void focusInEvent (QFocusEvent * event);
};

#endif // MAINWINDOW_H
