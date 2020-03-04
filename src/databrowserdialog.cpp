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
#include <QScrollBar>
#include "ui_databrowserdialog.h"
#include "databrowserdialog.h"
#include "common.h"

struct Quote
{
  QString date;
  QString open;
  QString high;
  QString low;
  QString close;
  QString volume;
};

Q_DECLARE_TYPEINFO (Quote, Q_MOVABLE_TYPE);
typedef QVector < Quote > QuoteVector;

static QuoteVector qvector;
static const int NCOLUMNS = 6;

// constructor
DataBrowserDialog::DataBrowserDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::DataBrowserDialog)
{
  QString stylesheet = "background: transparent; background-color: white;",
          buttonstylesheet = "background: transparent; background-color: white; color:black";
  int colwidth = 0;

  cheadersList << " Date "
               << " Open "
               << " High "
               << " Low  "
               << " Close "
               << " Volume ";
               

  ui->setupUi (this);
  this->setStyleSheet (stylesheet);
  ui->tableWidget->setColumnCount (NCOLUMNS);
  ui->tableWidget->setHorizontalHeaderLabels (cheadersList);
  ui->tableWidget->sortByColumn (0, Qt::AscendingOrder);
  ui->tableWidget->setStyleSheet (stylesheet);
  ui->tableWidget->verticalScrollBar ()->setStyleSheet ("background: transparent; background-color:lightgray;");
  ui->tableWidget->horizontalScrollBar ()->setStyleSheet ("background: transparent; background-color:lightgray;");
  ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
  
  for (qint32 counter = 0; counter < NCOLUMNS - 1; counter ++)
    colwidth += ui->tableWidget->columnWidth (counter);

  this->resize (colwidth + 40, height ());

  // connect to signals
  connect (ui->closeButton, SIGNAL (clicked ()), this,
           SLOT (closeButton_clicked ()));
  
  correctWidgetFonts (this); 
  if (parent != NULL)
    setParent (parent);       
}

// destructor
DataBrowserDialog::~DataBrowserDialog ()
{
  qvector.clear ();
  delete ui;
}

/// Events
///
// resize
void
DataBrowserDialog::resizeEvent (QResizeEvent * event)
{
  QSize newsize;
  
  if (event->oldSize () == event->size ())
    return;
  
  newsize = event->size ();	
  ui->tableWidget->resize (newsize.width () - 10, newsize.height () - 60);
  ui->closeButton->move ((width () - 90) / 2, height () - 42);
}

// callback for sqlite3_exec
static int
sqlcb_table_data (void *dummy, int argc, char **argv, char **column)
{ 
  Quote q;
  float f;
  bool ok;
  
  if (dummy != NULL)
    return 1;
    
  for (qint32 counter = 0; counter < argc; counter ++)
  {
	QString colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();  
    
    if (colname == "DATE")
      q.date = QString::fromUtf8 (argv[counter]);
    else
    if (colname == "OPEN")
    {  	
      f = QString::fromUtf8 (argv[counter]).toFloat (&ok);
      q.open =   QString::number (f, 'f', 2);
    }
    else
    if (colname == "HIGH")
    {  
      f = QString::fromUtf8 (argv[counter]).toFloat (&ok);
      q.high =   QString::number (f, 'f', 2);
    }  
    else
    if (colname == "LOW")
    {
	  f = QString::fromUtf8 (argv[counter]).toFloat (&ok);
      q.low =    QString::number (f, 'f', 2);
    }
    else
    if (colname == "CLOSE")
    {  
      f = QString::fromUtf8 (argv[counter]).toFloat (&ok);
      q.close =  QString::number (f, 'f', 2);
    } 
    else
    if (colname == "VOLUME") 
      q.volume = QString::fromUtf8 (argv[counter]);	
  }
  
  qvector += q;
  return 0;	
}

// show
void
DataBrowserDialog::showEvent (QShowEvent * event)
{
  QString query;
  int rc;
  
  if (event->spontaneous ())	
    return;
    
  ui->tableWidget->clearContents ();      
  qvector.clear ();
  query = "SELECT DATE, OPEN, HIGH, LOW, CLOSE, VOLUME FROM ";
  query += tablename + " ORDER BY DATE DESC;";
  
  rc = sqlite3_exec(Application_Settings->db, query.toUtf8(), 
                    sqlcb_table_data, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
    this->hide ();
    return;
  } 
  
  ui->tableWidget->setSortingEnabled (false);
  ui->tableWidget->setRowCount (qvector.size ());
  for (qint32 counter = 0; counter < qvector.size (); counter++)
  {
	QTableWidgetItem *item;
	  
    item = new QTableWidgetItem;
    item->setText (qvector[counter].date);
    ui->tableWidget->setItem(counter,0,item);
    
    item = new QTableWidgetItem;
    item->setText (qvector[counter].open);
    ui->tableWidget->setItem(counter,1,item);
    
    item = new QTableWidgetItem;
    item->setText (qvector[counter].high);
    ui->tableWidget->setItem(counter,2,item);
    
    item = new QTableWidgetItem;
    item->setText (qvector[counter].low);
    ui->tableWidget->setItem(counter,3,item);
    
    item = new QTableWidgetItem;
    item->setText (qvector[counter].close);
    ui->tableWidget->setItem(counter,4,item);
    
    item = new QTableWidgetItem;
    item->setText (qvector[counter].volume);
    ui->tableWidget->setItem(counter,5,item);
  }

  ui->tableWidget->viewport()->update();
}

// public slots
void
DataBrowserDialog::closeButton_clicked ()
{
  this->hide ();
}

