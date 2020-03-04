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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QSysInfo>
#include <QtGui/QApplication>
#include <QTime>
#include <QIcon>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QTextStream>
#include <QAtomicInt>
#include "common.h"
#include "libjson.h"

#ifdef Q_OS_MAC
#include <CoreServices/CoreServices.h>
#endif

#ifdef Q_OS_LINUX
#include <sys/utsname.h>
#endif // Q_OS_LINUX

// delay nsecs
void 
delay(int secs)
{
  QTime dieTime= QTime::currentTime().addSecs(secs);
  while( QTime::currentTime() < dieTime )
  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);    
}

// show message box
void
showMessage (QString message)
{
  QMessageBox msgBox;
  QFont fnt;

  QApplication::restoreOverrideCursor();

  fnt = msgBox.font ();
  fnt.setPixelSize (14);
  fnt.setFamily (DEFAULT_FONT_FAMILY);
  fnt.setWeight (QFont::DemiBold);
  
  msgBox.setWindowTitle ("Message");
  msgBox.setWindowIcon (QIcon (QString::fromLocal8Bit (":/png/images/icons/PNG/cglogo.png")));
  msgBox.setFont (fnt);
  msgBox.setIcon (QMessageBox::Information);
  msgBox.setText(message + "           ");
  msgBox.setStandardButtons(QMessageBox::Close);
  msgBox.setDefaultButton(QMessageBox::Close);
  correctWidgetFonts (&msgBox);
  msgBox.exec ();
}

// show demo message box
bool
showDemoMessage ()
{
  QMessageBox msgBox;
  QPushButton *purchaseBtn, *closeBtn;
  QFont fnt;

  fnt = msgBox.font ();
  fnt.setPixelSize (14);
  fnt.setFamily (DEFAULT_FONT_FAMILY);
  fnt.setWeight (QFont::DemiBold);
  
  msgBox.setWindowTitle ("Purchase full version");
  msgBox.setWindowIcon (QIcon (QString::fromLocal8Bit (":/png/images/icons/PNG/cglogo.png")));
  msgBox.setFont (fnt);
  msgBox.setIcon (QMessageBox::Warning);
  msgBox.setText("Trial version permits function of Download Manager when Data Manager contains less than five distinct symbols. Click 'Purchase' button to remove this limit.");
  purchaseBtn = msgBox.addButton ("Purchase", QMessageBox::AcceptRole);
  closeBtn = msgBox.addButton ("Close", QMessageBox::RejectRole);
  correctWidgetFonts (&msgBox);
  msgBox.exec ();
  if (msgBox.clickedButton() == (QAbstractButton *) purchaseBtn)
    return true;
  else
  if (msgBox.clickedButton() == (QAbstractButton *) closeBtn)  
    return false;
  
  return false;  
}

// show download message box
bool
showDownloadMessage ()
{
  QMessageBox msgBox;
  QPushButton *downloadBtn, *closeBtn;
  QFont fnt;

  fnt = msgBox.font ();
  fnt.setPixelSize (14);
  fnt.setFamily (DEFAULT_FONT_FAMILY);
  fnt.setWeight (QFont::DemiBold);
  
  msgBox.setWindowTitle ("Download new version");
  msgBox.setWindowIcon (QIcon (QString::fromLocal8Bit (":/png/images/icons/PNG/cglogo.png")));
  msgBox.setFont (fnt);
  msgBox.setIcon (QMessageBox::Warning);
  msgBox.setText("There is a new version available for download");
  downloadBtn = msgBox.addButton ("Download", QMessageBox::AcceptRole);
  closeBtn = msgBox.addButton ("Close", QMessageBox::RejectRole);
  correctWidgetFonts (&msgBox);
  msgBox.exec ();
  if (msgBox.clickedButton() == (QAbstractButton *) downloadBtn)
    return true;
  else
  if (msgBox.clickedButton() == (QAbstractButton *) closeBtn)  
    return false;
      
  return false;
}

// show Ok/Cancel message box
bool
showOkCancel (QString message)
{
  QMessageBox msgBox;
  QFont fnt;

  QApplication::restoreOverrideCursor();
  fnt = msgBox.font ();
  fnt.setPixelSize (14);
  fnt.setFamily (DEFAULT_FONT_FAMILY);
  fnt.setWeight (QFont::DemiBold);
  
  msgBox.setWindowTitle ("Question");
  msgBox.setWindowIcon (QIcon (QString::fromLocal8Bit (":/png/images/icons/PNG/cglogo.png")));
  msgBox.setFont (fnt);
  msgBox.setIcon (QMessageBox::Question);
  msgBox.setText(message);
  msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  correctWidgetFonts (&msgBox);
  msgBox.exec ();
  if (msgBox.clickedButton() == msgBox.button (QMessageBox::Ok))
    return true;

  return false;
}

// error messages
QString
errorMessage (CG_ERR_RESULT err)
{
  QStringList ErrorMessage;
  ErrorMessage  <<
                QStringLiteral ("No error") <<
                QStringLiteral ("Cannot open file") <<
                QStringLiteral ("Cannot create temporary table") <<
                QStringLiteral ("Cannot create table") <<
                QStringLiteral ("Cannot insert data") <<
                QStringLiteral ("Cannot delete data") <<
                QStringLiteral ("Cannot access database") <<
                QStringLiteral ("Invalid reply or network error") <<
                QStringLiteral ("Cannot create temporary file") <<
                QStringLiteral ("Cannot write to file") <<
                QStringLiteral ("Transaction error") <<
                QStringLiteral ("Not enough memory") <<
                QStringLiteral ("Symbol does not exist") <<
                QStringLiteral ("Cannot access data") <<
                QStringLiteral ("Network timeout") <<
                QStringLiteral ("Invalid data") <<
                QStringLiteral ("Request pending") <<
                QStringLiteral ("Buffer not found") <<
                QStringLiteral ("No quotes for symbol") <<
                QStringLiteral ("Operation failed") <<
                QStringLiteral ("Compiler not found") <<
                QStringLiteral ("Compilation failed") <<
                QStringLiteral ("No data");

  GlobalError = CG_ERR_OK;

  return ErrorMessage[err];
}

// full operating system description
extern QString
fullOperatingSystemVersion ()
{
  QString full = "", os = "", ver = "", desc = "";

#ifdef Q_OS_WIN32
  os = "Microsoft Windows ";
  switch (QSysInfo::WindowsVersion) 
  {
	case QSysInfo::WV_NT:
	  ver = "NT ";
	  break;
	case QSysInfo::WV_2000:
	  ver = "2000 ";
	  break;
	case QSysInfo::WV_XP:
	  ver = "XP ";
	  break;
	case QSysInfo::WV_2003:
	  ver = "2003 ";
	  break;
	case QSysInfo::WV_VISTA:
	  ver = "Vista ";
	  break;
	case QSysInfo::WV_WINDOWS7:
	  ver = "7 ";
	  break;
	default:
	  ver = ">7 ";
  }
#endif // Q_OS_WIN32

#ifdef Q_OS_MAC
  SInt32 majorVersion,minorVersion,bugFixVersion;
  
  os = "Mac OS X ";
  switch (QSysInfo::MacintoshVersion)
  {
	case QSysInfo::MV_CHEETAH:
	  ver = "10.0 Cheetah ";
	  break;
	case QSysInfo::MV_PUMA:
	  ver = "10.1 Puma ";
	  break;
	case QSysInfo::MV_JAGUAR:
	  ver = "10.2 Jaguar ";
	  break;
	case QSysInfo::MV_PANTHER:
	  ver = "10.3 Panther ";
	  break;
	case QSysInfo::MV_TIGER:
	  ver = "10.4 Tiger ";
	  break;
	case QSysInfo::MV_LEOPARD:
	  ver = "10.5 Leopard ";
	  break;
	case QSysInfo::MV_SNOWLEOPARD:
	  ver = "10.6 Snow Leopard ";
	  break;
	case QSysInfo::MV_LION:
	  ver = "10.7 Lion ";
	  break;
	case QSysInfo::MV_MOUNTAINLION:
	  ver = "10.8 Mountain Lion ";
	  break;  
	default:
	  Gestalt(gestaltSystemVersionMajor, &majorVersion);
      Gestalt(gestaltSystemVersionMinor, &minorVersion);
      Gestalt(gestaltSystemVersionBugFix, &bugFixVersion);
      ver = QString::number ((int) majorVersion) + "." +
            QString::number ((int) minorVersion) + " ";
  }  
#endif // Q_OS_MAC

#ifdef Q_OS_LINUX
  struct utsname unixname;
  if (uname (&unixname) >= 0)
  {
	os = QString (unixname.sysname) + " ";
	ver = QString (unixname.release) + " ";
  }
#endif // Q_OS_LINUX
  
  full = os + ver + desc;
  return full;
}

// set global error
void
setGlobalError(CG_ERR_RESULT err, const char *_file_, int _line_)
{
#ifdef QDEBUG_H
  if (err != 0)
    qDebug () << err << _file_ << _line_;
#endif     
  	
  if (GlobalError == CG_ERR_OK)
    GlobalError = err;
  
  return;
}

// formats' callback
int
sqlcb_formats(void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  
  if (dummy != NULL)
    return 1;
    
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "FORMAT")
      ComboItems->formatList << QString::fromUtf8 (argv[counter]);
  }
  return 0;
}

// timeframes' callback
int
sqlcb_timeframes(void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  
  if (dummy != NULL)
    return 1;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "TIMEFRAME")
      ComboItems->timeframeList << QString::fromUtf8 (argv[counter]);
  }
  return 0;
}

// currencies' callback
int
sqlcb_currencies(void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  
  if (dummy != NULL)
    return 1;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "SYMBOL")
      ComboItems->currencyList << QString::fromUtf8 (argv[counter]);
  }
  return 0;
}

// currencies' callback
int
sqlcb_markets(void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  
  if (dummy != NULL)
    return 1;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "MARKET")
      ComboItems->marketList << QString::fromUtf8 (argv[counter]);
  }
  return 0;
}

// datafeeds' callback
int
sqlcb_datafeeds (void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  
  if (dummy != NULL)
    return 1;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "FEEDNAME")
      ComboItems->datafeedsList << QString::fromUtf8 (argv[counter]);
  }
  return 0;
}

// symbol's data frames callback
int
sqlcb_dataframes (void *dummy, int argc, char **argv, char **column)
{
  QTAChartFrame Frame;
  QString colname;
  
  if (dummy != NULL)
    return 1;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "OPEN")
      Frame.Open = QString::fromUtf8(argv[counter]).toFloat ();
    else
    if (colname == "HIGH")
      Frame.High = QString::fromUtf8(argv[counter]).toFloat ();
    else
    if (colname == "LOW")
      Frame.Low = QString::fromUtf8(argv[counter]).toFloat ();
    else
    if (colname == "CLOSE")
      Frame.Close = QString::fromUtf8(argv[counter]).toFloat ();
    else
    if (colname == "ADJCLOSE")
      Frame.AdjClose = QString::fromUtf8(argv[counter]).toFloat ();  
    else
    if (colname == "VOLUME")
      Frame.Volume = QString::fromUtf8(argv[counter]).toFloat ();
    else
    if (colname == "DATE")
      strcpy (Frame.Date, argv[counter]);
    else
    if (colname == "TIME")
      strcpy (Frame.Time, argv[counter]);
  }

  Frames << Frame;
  return 0;
}

// symbol's fundamentals callback
int 
sqlcb_fundamentals (void *data, int argc, char **argv, char **column)
{
  QTAChartData *Data;	
  QString colname;
  
  Data = reinterpret_cast <QTAChartData *> (data);
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toLower ();
    
    if (colname == "bv")
      Data->bv = QString::fromUtf8(argv[counter]);
    else  
    if (colname == "mc")
      Data->mc = QString::fromUtf8(argv[counter]);  
    else  
    if (colname == "ebitda")
      Data->ebitda = QString::fromUtf8(argv[counter]);    
    else  
    if (colname == "pe")
      Data->pe = QString::fromUtf8(argv[counter]);      
    else  
    if (colname == "peg")
      Data->peg = QString::fromUtf8(argv[counter]);        
    else  
    if (colname == "dy")
      Data->dy = QString::fromUtf8(argv[counter]);          
    else  
    if (colname == "epscurrent")
      Data->epscurrent = QString::fromUtf8(argv[counter]);  
    else  
    if (colname == "epsnext")
      Data->epsnext = QString::fromUtf8(argv[counter]);  
    else  
    if (colname == "es")
      Data->es = QString::fromUtf8(argv[counter]);              
    else  
    if (colname == "ps")
      Data->ps = QString::fromUtf8(argv[counter]);                
    else  
    if (colname == "pbv")
      Data->pbv = QString::fromUtf8(argv[counter]);                  
  }
  
  return 0;    
  
}

// sqlite3_exec callback for retrieving application options
int
sqlcb_options (void *classptr, int argc, char **argv, char **column)
{
  AppOptions *options = reinterpret_cast <AppOptions *> (classptr);
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
	QString colname = QString::fromUtf8(column[counter]);
	colname = colname.toUpper ();
	
	if (colname == "PAK")
	{
	  options->pak = QString::fromUtf8 (argv[counter]); 
	}  
	else
	if (colname == "SHOWSPLASHSCREEN")
	{
	  if (atoi (argv[counter]) == 1)
	    options->showsplashscreen = true;
	  else
	    options->showsplashscreen = false;
	}
	else
	if (colname == "CHECKNEWVERSION")
	{
	  if (atoi (argv[counter]))
	    options->checknewversion = true;
	  else
	    options->checknewversion = false;
	}
	else
	if (colname == "LONGBP")
	{
	  if (atoi (argv[counter]))
	    options->longbp = true;
	  else
	    options->longbp = false;
	}
	else
	if (colname == "ENABLEPROXY")
	{
	  if (atoi (argv[counter]))
	    options->enableproxy = true;
	  else
	    options->enableproxy = false;
	}
	else
	if (colname == "PROXYHOST")
	{
	  options->proxyhost = QString::fromUtf8 (argv[counter]); 
	}
	else
	if (colname == "PROXYUSER")
	{
	  options->proxyuser = QString::fromUtf8 (argv[counter]);   
	}
	else
	if (colname == "PROXYPASS")
	{
	  options->proxypass = QString::fromUtf8 (argv[counter]);   
	}
	else
	if (colname == "PROXYPORT")
	{
	  options->proxyport = QString::fromUtf8(argv[counter]).toShort ();  
	}
	else
	if (colname == "NETTIMEOUT")
	  options->nettimeout = QString::fromUtf8(argv[counter]).toShort ();    
  }
  return 0;	
}

// nsymbols callback
int
sqlcb_nsymbols(void *nsymptr, int argc, char **argv, char **column)
{
  QString colname;
  int nsymbols;
  
  nsymbols = QString::fromUtf8(argv[0]).toInt ();
  *(int *) nsymptr = nsymbols; 
  
  return 0;
}

// load application options
CG_ERR_RESULT
loadAppOptions (AppOptions *options)
{
  QString query = "";
  int rc;
  
  query += "SELECT * FROM options WHERE recid = 1;";  
  rc = sqlite3_exec(Application_Settings->db, query.toUtf8(), 
                    sqlcb_options, options, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  } 
  
  FULL = true;
  
  return CG_ERR_OK; 	
}	

// save application options
CG_ERR_RESULT
saveAppOptions (AppOptions *options)
{
  QString query = "";
  int rc;
  
  query += "BEGIN; UPDATE options SET ";
  query.append ("\n");
  query += "pak = \"" + options->pak + "\"";
  
  if (options->showsplashscreen)
    query += ", showsplashscreen = 1 ";
  else  
    query += ", showsplashscreen = 0 ";
    
  if (options->checknewversion)
    query += ", checknewversion = 1 ";
  else  
    query += ", checknewversion = 0 ";
    
  if (options->enableproxy)
    query += ", enableproxy = 1 ";
  else  
    query += ", enableproxy = 0 ";  
    
  if (options->longbp)
    query += ", longbp = 1 ";
  else  
    query += ", longbp = 0 ";  
    
  query += ", proxyhost = \"" + options->proxyhost + "\"";
  query += ", proxyuser = \"" + options->proxyuser + "\"";
  query += ", proxypass = \"" + options->proxypass + "\"";
  query.append ("\n");
  query += ", proxyport = " + QString::number (options->proxyport);
  query += ", nettimeout = " + QString::number (options->nettimeout);
  query += " WHERE recid = 1; COMMIT;";
  
  rc = sqlite3_exec(Application_Settings->db, query.toUtf8(), 
                    NULL, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  } 
  
  FULL = true;
  return CG_ERR_OK; 	
}

// set font size
void
correctFontSize (QWidget *widget)
{
  QFont fnt;
  
  fnt = widget->font ();
  if (fnt.pointSize () == -1)
    return;
  	
  fnt.setPixelSize (fnt.pointSize () + FONT_SIZE_PAD);
  fnt.setFamily (DEFAULT_FONT_FAMILY);
  widget->setFont (fnt);
}

// correct font size for widget and children
void
correctWidgetFonts (QWidget *widget)
{
   QList<QWidget *> allWidgets = widget->findChildren<QWidget *>();	
   QWidget *wid;
   
   foreach (wid, allWidgets)
     correctFontSize (wid);
   
   correctFontSize (widget);  
}

void
correctWidgetFonts (QDialog *widget)
{
   QList<QWidget *> allWidgets = widget->findChildren<QWidget *>();	
   QWidget *wid;
   
   foreach (wid, allWidgets)
     correctFontSize (wid);

   correctFontSize (widget);  
}

void
correctWidgetFonts (QMessageBox *widget)
{
   QList<QWidget *> allWidgets = widget->findChildren<QWidget *>();	
   QWidget *wid;
   
   foreach (wid, allWidgets)
     correctFontSize (wid);
   
   correctFontSize (widget);  
}

// native http header
QString
nativeHttpHeader ()
{
  QString header = "";
  
  header += QString (APPNAME) + "/";
  header +=	QString::number (VERSION_MAJOR) + ".";
  header += QString::number (VERSION_MINOR) + ".";
  header += QString::number (VERSION_PATCH) + " ";
  header += fullOperatingSystemVersion () + " ";
  header += QString::number (sizeof (void *)*8) + " ";
  header += RunCounter + " " + UID + " ";
  
  return header;
}

// random http header 
QString
httpHeader ()
{
  QTime time = QTime::currentTime();
    
  qsrand((uint)time.msec());	
  return UserAgents[qrand() % UserAgents.size ()];
}

bool
json_parse (QString jsonstr, QStringList *node, QStringList *value, void *n1)
{
  JSONNODE *n = NULL;
  JSONNODE_ITERATOR i;
  bool result = true, allocated = false;

  if (n1 == NULL)
  {
    node->clear ();
    value->clear ();

    jsonstr.remove (13);
    jsonstr.remove (10);
    jsonstr.replace ('/', ' ');
    jsonstr.remove ('[');
    jsonstr.remove (']');
    jsonstr.replace ("\\x26", "&");
    jsonstr = jsonstr.trimmed ();
    n = json_parse (jsonstr.toStdString ().c_str());
    if (n == NULL)
    {
	  result = false;
	  goto json_parse_end;	
    }
    
    allocated = true;
  }
  else
    n = (JSONNODE *) n1;
 
  i = json_begin(n);
  while (i != json_end(n))
  {
    if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE)
    {
       if (!json_parse (jsonstr, node, value, *i))
       {
         result = false;
         goto json_parse_end;
       }
    }

    // get the node name and value as a string
    json_char *node_name = json_name(*i);
    json_char *node_value = json_as_string(*i);
    node->append (QString::fromLocal8Bit (node_name));
    value->append (QString::fromLocal8Bit (node_value));

    json_free(node_value);
    json_free(node_name);
    i++;
  }

json_parse_end:
  if (allocated)
  	json_delete(n);
  
  return result;
}
