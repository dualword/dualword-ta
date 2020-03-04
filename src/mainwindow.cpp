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

#include <QDesktopServices>
#include <QTime>
#include <QDir>
#include <QFontDatabase>
#include <QFileInfo>
#include <QFileDialog>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDateTime>
#include <QColor>
#include <QPixmap>
#include <QResizeEvent>
#include <QFocusEvent>
#include "sqlite3.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "splashdialog.h"
#include "qtachart.h"

AppSettings *Application_Settings;
SQLists *ComboItems;
FrameVector Frames;
LoadCSVDialog *loadcsvdialog;
DownloadDataDialog *downloaddatadialog;
TemplateManagerDialog *templatemanager;
ProgressDialog *progressdialog;
QProgressBar *GlobalProgressBar;
QString Year, Month, Day, UID, RunCounter;
QStringList UserAgents;
bool FULL = false;

QAtomicInt GlobalError = CG_ERR_OK;

// load application fonts
void 
MainWindow::loadFonts ()
{
  QFile file;
  QByteArray barray;
  
  file.setFileName (":/fonts/fonts/DejaVuSans.ttf");
  if (file.open(QIODevice::ReadOnly))
  {
    barray = file.readAll ();
    QFontDatabase::addApplicationFontFromData (barray);
    file.close ();
  }
  
  file.setFileName (":/fonts/fonts/Monaco.ttf");
  if (file.open(QIODevice::ReadOnly))
  {
	barray.clear ();    
    barray = file.readAll ();
    QFontDatabase::addApplicationFontFromData (barray);
    file.close ();
  }
  
  file.setFileName (":/fonts/fonts/DroidSerif-Regular.ttf");
  if (file.open(QIODevice::ReadOnly))
  {
	barray.clear ();  
    barray = file.readAll ();
    QFontDatabase::addApplicationFontFromData (barray);
    file.close ();
  }
  
  file.setFileName (":/fonts/fonts/tahoma.ttf");
  if (file.open(QIODevice::ReadOnly))
  {
	barray.clear ();  
    barray = file.readAll ();
    QFontDatabase::addApplicationFontFromData (barray);
    file.close ();
  }
  
  barray.clear ();
}  

// check if there is new version available
void
MainWindow::checkNewVersion ()	
{
  newversion = false; 
  UpdateManager = new QNetworkAccessManager (this);
  connect (UpdateManager, SIGNAL (finished (QNetworkReply *)), this,
           SLOT (updateReplyFinished (QNetworkReply *)));
  request.setRawHeader("User-Agent", nativeHttpHeader ().toAscii ());  
  request.setUrl(
#ifdef Q_OS_LINUX  
       (QUrl ("http://ns1.chartgeany.com/ossversion/linux/current.txt")));         
#else
#ifdef Q_OS_WIN
       (QUrl ("http://ns1.chartgeany.com/ossversion/windows/current.txt"))); 
#else       
#ifdef Q_OS_MAC
       (QUrl ("http://ns1.chartgeany.com/ossversion/mac/current.txt"))); 
#endif
#endif
#endif
  UpdateManager->get (request); 
}

static int
sqlcb_dbversion (void *versionptr, int argc, char **argv, char **column)
{
  QString colname;
  int version = -1;

  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "VERSION")
      version = QString::fromUtf8 (argv[counter]).toInt ();
  }
  *(int *) versionptr = version;
  return 0;
}

static int
sqlcb_dbdata (void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  
  if (dummy != NULL)
    return 1;
    
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "UID")
      UID = QString::fromUtf8 (argv[counter]);
    if (colname == "RUNCOUNTER")
      RunCounter = QString::fromUtf8 (argv[counter]);  
  }
 
  return 0;
}

// constructor
MainWindow::MainWindow (QWidget * parent):
  QMainWindow (parent), ui (new Ui::MainWindow)
{
  SplashDialog *splash = NULL;	
  QDateTime datetime;
  QFileInfo dbfile;
  QFile initcopy;
  QString stylesheet = "background: transparent; background-color: white; color:black",
          SQLCommand = "";
  int rc, dbversion;
  bool showlicense = false;
  
  ui->setupUi (this);
  UpdateManager = new QNetworkAccessManager (this);
  UserAgents << APPNAME;
  closed = false;
      
  SQLCommand += "PRAGMA locking_mode = EXCLUSIVE;BEGIN EXCLUSIVE;COMMIT;";
  SQLCommand += "PRAGMA max_page_count = 4294967291;";
  SQLCommand += "PRAGMA synchronous = 0;";
  
  expandedChartFlag = false;
  newversion = false;
  setWindowTitle (QApplication::applicationName ());
  this->setStatusBar ( 0 );
  this->setStyleSheet ("background-color:white;");
  tabWidget = ui->tabWidget;
  ui->tabWidget->clear ();
  ui->tabWidget->setUsesScrollButtons(true);
  ui->tabWidget->setStyleSheet("QTabBar::tab { height: 25px;}");
  ui->managerButton->setStyleSheet (stylesheet);
  ui->screenshotButton->setStyleSheet (stylesheet);
  ui->infoButton->setStyleSheet (stylesheet);
  ui->optionsButton->setStyleSheet (stylesheet);
  ui->exitButton->setStyleSheet (stylesheet);
  ui->homeButton->setStyleSheet (stylesheet);
  ui->forumButton->setStyleSheet (stylesheet);

  // current year, month - 1, day
  datetime = QDateTime::currentDateTime ();
  Year = QString::number(datetime.date ().year ());
  Month = QString::number(datetime.date ().month () - 1);
  Day = QString::number(datetime.date ().day ());
  
  // connect to signals
  connect (ui->tabWidget, SIGNAL(tabCloseRequested(int)), this,
           SLOT(closeTab_clicked (int)));
  connect (ui->tabWidget, SIGNAL(currentChanged(int)), this,
           SLOT(currentTab_changed (int)));
  connect (ui->managerButton, SIGNAL (clicked ()), this,
           SLOT (managerButton_clicked ()));
  connect (ui->screenshotButton, SIGNAL (clicked ()), this,
           SLOT (screenshotButton_clicked ()));
  connect (ui->optionsButton, SIGNAL (clicked ()), this,
           SLOT (optionsButton_clicked ())); 
  connect (ui->infoButton, SIGNAL (clicked ()), this,
           SLOT (infoButton_clicked ())); 
  connect (ui->homeButton, SIGNAL (clicked ()), this,
           SLOT (homeButton_clicked ()));  
  connect (ui->forumButton, SIGNAL (clicked ()), this,
           SLOT (forumButton_clicked ()));                                          
  connect (ui->exitButton, SIGNAL (clicked ()), this,
           SLOT (exitButton_clicked ()));

  // export application settings
  Application_Settings = &appsettings;
  
  // set the sqlite db path
  appsettings.sqlitefile = QDir::homePath () + "/" + ".config" + "/" + APPDIR + "/" + DBNAME;

  // check db existence and create it if needed
  dbfile.setFile (appsettings.sqlitefile);
  if (dbfile.exists () == false)
  {
	showlicense = true;  
	if (!QDir (QDir::homePath () + "/" + ".config" + "/" + APPDIR).exists ())
	  QDir ().mkpath (QDir::homePath () + "/" + ".config" + "/" + APPDIR);
    
    // open sqlite db
    rc = sqlite3_open(qPrintable (appsettings.sqlitefile), &appsettings.db);
    if (rc != SQLITE_OK) // if open failed, quit application
    {
      showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
      sqlite3_close (appsettings.db);
      qApp->exit (1);
      exit (1);
    }
    
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, this, NULL);
    if (rc != SQLITE_OK) // if open failed, quit application
    {
      showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
      sqlite3_close (appsettings.db);
      qApp->exit (1);
      exit (1);
    }
    
	if (dbman (1, appsettings) != CG_ERR_OK)
	{
	  showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));  
	  qApp->exit (1);
      exit (1);
    }  
/*  Keep this here for development and debug. DO NOT DELETE 
    initcopy.setFileName ("geanymasterbase.dat");
    initcopy.copy (appsettings.sqlitefile);
*/
  }
  else
  {
	// open sqlite db
    rc = sqlite3_open(qPrintable (appsettings.sqlitefile), &appsettings.db);
    if (rc != SQLITE_OK) // if open failed, quit application
    {
      showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
      sqlite3_close (appsettings.db);
      qApp->exit (1);
      exit (1);
    }  
  }
  
  // export classes and variables
  ComboItems = &comboitems;

  // apply pragmas
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, this, NULL);
  if (rc != SQLITE_OK) // if open failed, quit application
  {
    showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
    sqlite3_close (appsettings.db);
    qApp->exit (1);
    exit (1);
  }

  // check version
  SQLCommand = "SELECT * FROM VERSION;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), sqlcb_dbversion, (void *) &dbversion, NULL);
  if (rc != SQLITE_OK) // if open failed, quit application
  {
    showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
    sqlite3_close (appsettings.db);
    qApp->exit (1);
    exit (1);
  }

  if (dbversion == -1) // invalid dbversion
  {
	showMessage (QString::fromUtf8 ("Invalid data file. Application quits."));
    sqlite3_close (appsettings.db);
    qApp->exit (1);
    exit (1);
  }    

  if (dbversion < DBVERSION) // upgrade db file
  {
	dbversion ++;
	if (dbman (dbversion, appsettings) != CG_ERR_OK)
	{
	  showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));  
	  sqlite3_close (appsettings.db);
	  qApp->exit (1);
      exit (1);
    }  
  }

  // show splash
  loadAppOptions (&Application_Settings->options);
  if (Application_Settings->options.showsplashscreen == true)
  {
    splash = new SplashDialog (this);
    splash->show ();
    delay (1);
  }  
  
  // vacuum database if possible
  if (RunCounter.toInt () % 10 == 0)
    sqlite3_exec(Application_Settings->db, "VACUUM;", NULL, NULL, NULL);
  
  // increase run counter
  SQLCommand = "UPDATE VERSION SET RUNCOUNTER = RUNCOUNTER + 1;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, this, NULL);
  
  // load run counter and UID
  SQLCommand = "SELECT * FROM VERSION;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), sqlcb_dbdata, NULL, NULL);
  
  // check for new vesrion
  checkNewVersion ();
    
  // load application fonts
  loadFonts ();
  
  // initialize SQL statements
  strcpy (comboitems.formats_query, "select FORMAT from FORMATS");
  strcpy (comboitems.timeframes_query, "select TIMEFRAME from TIMEFRAMES_ORDERED");
  strcpy (comboitems.currencies_query, "select SYMBOL from CURRENCIES");
  strcpy (comboitems.markets_query, "select MARKET from MARKETS");
  strcpy (comboitems.datafeeds_query, "select FEEDNAME from DATAFEEDS");

  // create widgets
  loadcsvdlg = new LoadCSVDialog (this);
  downloaddatadlg = new DownloadDataDialog (this);
  datamanagerdlg = new DataManagerDialog (this);
  progressdlg = new ProgressDialog (this);
  waitdlg = new WaitDialog (this);
  templatemanagerdlg = new TemplateManagerDialog (this);
  optionsdlg = new OptionsDialog (this);
  infodlg = new InfoDialog (this);
  
  // export classes and variables
  loadcsvdialog = loadcsvdlg;
  downloaddatadialog = downloaddatadlg;
  progressdialog = progressdlg;
  templatemanager = templatemanagerdlg;

  // check if some initialization failed
  if (GlobalError != CG_ERR_OK)
  {
    ui->managerButton->setEnabled (false);
    ui->screenshotButton->setEnabled (false);
    ui->infoButton->setEnabled (false);
    ui->optionsButton->setEnabled (false);
    ui->tabWidget->setEnabled (false);
  }

  // initially remove all tabs
  while (ui->tabWidget->count () > 1)
    ui->tabWidget->removeTab (0);

  ui->tabWidget->resize (width () - 2, height () - 60);
  
  if (Application_Settings->options.showsplashscreen == true)
  {
    delay (3);
    splash->hide ();
    delete splash;
  }  
   
  if (showlicense)
  {
    infodlg->licensedlg->show ();
    delay (2);
  }  
    
  if (newversion)
    if (Application_Settings->options.checknewversion == true)
      if (showDownloadMessage ())
        QDesktopServices::openUrl (QUrl (APPWEBPAGE));
  
  waitdlg->setMessage (QString::fromUtf8 ("Exiting. Please wait..."));
}

// destructor
MainWindow::~MainWindow ()
{
  closing ();
}

//closing
void
MainWindow::closing ()
{
  if (closed)
    return;
    	
  delete optionsdlg;
  delete ui->tabWidget;  
  delete ui;
  
  // close database
  sqlite3_close (Application_Settings->db);
  
  closed = true;
}

// add a new chart
CG_ERR_RESULT
MainWindow::addChart (TableDataVector & datavector)
{
  QList<QAbstractButton*> allPButtons;	
  QTAChart *tachart;
  QTAChartData Data;
  TableDataClass tdc;
  QString SQLCommand, title, subtitle, tablename, 
          symbol, timeframe, name, source;
  int rc;
  CG_ERR_RESULT result = CG_ERR_OK;
  
  tachart = new QTAChart (ui->tabWidget);
  foreach (tdc, datavector)
  {
	tablename = tdc.tablename;
	symbol = tdc.symbol;
	source = tdc.source;
	timeframe = tdc.timeframe;
	name = tdc.name;
	
	// load frames
    Frames.clear ();
    SQLCommand = "select * from " + tablename + ";";
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(),
                      sqlcb_dataframes, NULL, NULL);
    if (rc != SQLITE_OK)
    {
	  delete tachart;  
      result = CG_ERR_ACCESS_DATA;
      setGlobalError(result, __FILE__, __LINE__);
      showMessage ("Symbol " +symbol +": " + errorMessage (result));
      return result;
    }
    
    // if market is London and longbp = true;
    if ((tdc.market.trimmed ().toUpper ().left (3) == "LON" ||
         tdc.market.trimmed ().toUpper ().left (3) == "LSE")&&
        Application_Settings->options.longbp)
    {
	   QTAChartFrame frame;
	   
	   for (qint32 counter = 0; counter < Frames.count (); counter ++)
	   {
		 frame = Frames[counter];  
		 frame.High /= 100;
		 frame.Low /= 100;
		 frame.Open /= 100;
		 frame.Close /= 100;
		 frame.AdjClose /= 100;
		 Frames[counter] = frame;
       }	
    }    
      
    if (timeframe == "DAY")
      tachart->loadFrames (Frames, "D", timeframe);
    else
    if (timeframe == "WEEK")
      tachart->loadFrames (Frames, "W", timeframe);
    else
    if (timeframe == "MONTH")
      tachart->loadFrames (Frames, "M", timeframe);  
    else
    if (timeframe == "YEAR")
      tachart->loadFrames (Frames, "Y", timeframe);   
  }
  
  tachart->setSymbolKey (datavector[0].tablename);
  
  // load data
  SQLCommand = "select * from basedata where base = '" + datavector[0].base + "'";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(),
                      sqlcb_fundamentals, (void *) &Data, NULL);
  if (rc != SQLITE_OK)
  {
	delete tachart;  
    result = CG_ERR_ACCESS_DATA;
    setGlobalError(result, __FILE__, __LINE__);
    showMessage ("Symbol " +symbol +": " + errorMessage (result));
    return result;
  }
  
  tachart->loadData (Data);
  
  tachart->setSymbol (symbol);  
  tachart->setFeed (source);  
  title = symbol;
  subtitle = name;
  
  tachart->setAlwaysRedraw (true);
  tachart->setTitle (title, subtitle);
  ui->tabWidget->addTab (tachart, symbol + " " + (tdc.adjusted == "NO"?"RAW":"ADJ"));
  ui->tabWidget->setCurrentIndex (ui->tabWidget->count () - 1);
  
  // remove tooltip from close tab buttons
  allPButtons = ui->tabWidget->findChildren<QAbstractButton*>();	
  for (int ind = 0; ind < allPButtons.size(); ind++) 
    if (allPButtons.at(ind)->inherits("CloseButton"))
      allPButtons.at(ind)->setToolTip("");
  
  return result;
}

// expanded chart
bool
MainWindow::expandedChart ()
{
  return expandedChartFlag;
}

// expand/shrink chart
void
MainWindow::setExpandChart (bool expandflag)
{
  if (expandflag == expandedChartFlag)
    return;

  expandedChartFlag = expandflag;
  if (!expandflag)
  {
    int max = ui->tabWidget->count ();

    if (max == 0)
      return;
    ui->managerButton->show ();    
    ui->screenshotButton->show ();  
    ui->optionsButton->show (); 
    ui->infoButton->show ();  
    ui->exitButton->show ();
    ui->homeButton->show ();
    ui->forumButton->show ();
    ui->tabWidget->move (0, 55);
    ui->tabWidget->resize (width () - 2, height () - 60);

    for (qint32 counter = 0; counter < max; counter ++)
      ui->tabWidget->widget(counter)->resize (ui->tabWidget->width () - 2,
                                              ui->tabWidget->height () - 20);
  }
  else
  {
	ui->managerButton->hide ();  
	ui->screenshotButton->hide ();   
	ui->optionsButton->hide ();  
	ui->infoButton->hide ();     
    ui->exitButton->hide ();
    ui->homeButton->hide ();
    ui->forumButton->hide ();
    ui->tabWidget->move (1, 1);
    ui->tabWidget->resize (width () - 2, height ());
    int max = ui->tabWidget->count ();
    if (max == 0)
      return;

    for (qint32 counter = 0; counter < max; counter ++)
      ui->tabWidget->widget(counter)->resize (ui->tabWidget->width () - 2,
                                              ui->tabWidget->height () - 20);
  }
}

/// Signals
///

// update reply
void 
MainWindow::updateReplyFinished (QNetworkReply * updatereply)
{
  QVariant replyStatus;	
  QString redirectUrl;
  
  replyStatus = updatereply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  if (replyStatus.isNull () || (!replyStatus.isValid ()))
    return;
  
  if (replyStatus == 200) // Ok
  {
    QByteArray replybytes = updatereply->readAll (); 
    QString replystring = QString::fromLocal8Bit (replybytes.constData ());
    QStringList versiondigits =
      replystring.split (".", QString::SkipEmptyParts);
    
    newversion = false;
    if (versiondigits.length () == 3)	
    {
	  if (versiondigits[0].toInt () > VERSION_MAJOR)
	  {
	    newversion = true;
	    return;
	  }  
	 
	  if (versiondigits[0].toInt () < VERSION_MAJOR)
	    return;
	 	 
	  if (versiondigits[1].toInt () > VERSION_MINOR)
	  {
	    newversion = true;
	    return;
	  }
	 
	  if (versiondigits[1].toInt () < VERSION_MINOR)
	    return;
	 
	  if (versiondigits[2].toInt () > VERSION_PATCH)
	  {
	    newversion = true;
	    return;
	  }
    }
  }
  else if (replyStatus == 301) // redirect
  {  
	redirectUrl =
      updatereply->attribute (QNetworkRequest::RedirectionTargetAttribute).toString();
    request.setUrl(QUrl(redirectUrl));
    request.setRawHeader("User-Agent", nativeHttpHeader ().toAscii ());
    UpdateManager->get (request);  
  }
}

// managerButton_clicked ()
void
MainWindow::managerButton_clicked ()
{
  datamanagerdlg->show ();
}

// managerButton_clicked ()
void
MainWindow::screenshotButton_clicked ()
{
  QFileDialog *fileDialog;
  QTAChart *chart;
  QString fileName = "";
  QPixmap screenshot;
  
  if (ui->tabWidget->count () == 0)
  {
	showMessage ("Open a chart first please.");  
    return;
  }  
  
  fileDialog = new QFileDialog;  
  chart = reinterpret_cast <QTAChart *> (ui->tabWidget->widget(ui->tabWidget->currentIndex ()));
  chart->setCustomBottomText (APPWEBPAGE);
  screenshot = QPixmap::grabWidget (chart);
  fileName = fileDialog->getSaveFileName(this, "Save chart", "", "Image (*.png)"); 
  
  if (fileName == "")
    goto screenshotButton_clicked_end;  
  
  if (fileName.mid (fileName.size () - 4).toLower () != ".png")
    fileName += ".png";
  
  screenshot.save(fileName, "PNG");  
  showMessage ("Screenshot saved.");

screenshotButton_clicked_end:  
  chart->restoreBottomText ();
  delete fileDialog;
  
}

// exit_clicked ()
void
MainWindow::exitButton_clicked ()
{
  bool answer;

  answer = showOkCancel ("Quit " + QApplication::applicationName () + "?");
  if (!answer)
    return;
  
  waitdlg->show ();
  delay (1);
  
  qApp->exit (0);
}

// options_clicked ()
void
MainWindow::optionsButton_clicked ()
{
  /* this is a weird trick in order to keep os x running */
  optionsdlg->exec ();
  delete optionsdlg;
  optionsdlg = new OptionsDialog (this);
}

// info_clicked ()
void
MainWindow::infoButton_clicked ()
{
  infodlg->show ();
  
}

// home_clicked ()
void
MainWindow::homeButton_clicked ()
{
  QDesktopServices::openUrl(QUrl(APPWEBPAGE));
}

// forum_clicked ()
void
MainWindow::forumButton_clicked ()
{
  QDesktopServices::openUrl(QUrl(APPFORUM));
}

// closeTab_clicked ()
void
MainWindow::closeTab_clicked (int index)
{
  QWidget *chart;

  if (ui->tabWidget->count () == 1)
    setExpandChart (false);

  ui->tabWidget->widget (index)->hide ();
  chart = ui->tabWidget->widget (index);
  ui->tabWidget->removeTab (index);
  delete chart;
}

// currentTab_changed ()
void
MainWindow::currentTab_changed (int index)
{
  int max;

  max = ui->tabWidget->count ();
  if (max < 1)
    return;

  for (qint32 counter = 0; counter < max; counter ++)
    ui->tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"))->
    setTabTextColor (counter, QColor(Qt::black));

  ui->tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"))->
  setTabTextColor (index, QColor(Qt::blue));

  ui->tabWidget->widget (index)->setFocus (Qt::OtherFocusReason);
}

/// Events
///
// resize
void
MainWindow::resizeEvent (QResizeEvent * event)
{
  QSize newsize;	
  int max;
  
  if (event->oldSize () == event->size ())
    return;
  
  newsize = event->size ();
  ui->exitButton->move (newsize.width () - 50, ui->exitButton->y ());
  ui->forumButton->move (newsize.width () - 95, ui->exitButton->y ());
  ui->homeButton->move (newsize.width () - 140, ui->exitButton->y ());
  if (!expandedChartFlag )
    ui->tabWidget->resize (newsize.width () - 2, newsize.height () - 60);
  else
    ui->tabWidget->resize (newsize.width () - 2, newsize.height ());

  max = ui->tabWidget->count ();
  if (max == 0)
    return;

  for (qint32 counter = 0; counter < max; counter ++)
    ui->tabWidget->widget(counter)->resize (ui->tabWidget->width () - 2,
                                            ui->tabWidget->height () - 20);
}

// focus
void
MainWindow::focusInEvent (QFocusEvent * event)
{
  ui->tabWidget->setFocus (event->reason ());
}
