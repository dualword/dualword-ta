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

#ifndef DATAMANAGERDIALOG_H
#define DATAMANAGERDIALOG_H

#include <QDialog>
#include "defs.h"
#include "databrowserdialog.h"

namespace Ui
{
  class DataManagerDialog;
}

class DataManagerDialog : public QDialog
{
  Q_OBJECT

public:
  explicit DataManagerDialog(QWidget *parent = 0);
  ~DataManagerDialog();

  QStringList cheadersList; // list of columns' headers
  QStringList symbolList;	  // symbols column content
  QStringList descList;     // name column content
  QStringList marketList;   // markets column content
  QStringList sourceList;   // source column content
  QStringList timeframeList;// timeframe column content
  QStringList datefromList;  // from column content
  QStringList datetoList;    // to column content
  QStringList currencyList; // currency column content
  QStringList keyList; // key column content
  QStringList adjustedList; // adjusted column content
  QStringList baseList; // base column content
  
  QStringList sqlite_master_name; // name column of sqlite_master table
  QStringList sqlite_master_type; // type columnt of sqlite_master table
  TableDataVector TDVector; // vector of TableDataClass

private:
  Ui::DataManagerDialog * ui;
  
  DataBrowserDialog *browser; // data browser
  void reloadSymbols ();	// reload all symbols
  void fillcolumn (QStringList list, int col); // fill table column
  CG_ERR_RESULT fillTableDataVector (QString key, // fill TableDataVector
                                     QString adjusted); 
  void cleartable (); // clear table
  QString formSQLDropSentence (QString table, qint32 *nentries); // form a drop sql sentence 

private slots:
  void importButton_clicked ();
  void refreshButton_clicked ();
  void exitButton_clicked ();
  void trashButton_clicked ();
  void downloadButton_clicked ();
  void updateButton_clicked ();
  void browserButton_clicked ();
  void chartButton_clicked ();
  void symbol_double_clicked ();

protected:
  virtual void resizeEvent (QResizeEvent * event);
  virtual void showEvent (QShowEvent * event);
  virtual void changeEvent (QEvent * event);
  virtual void keyPressEvent (QKeyEvent * event);
};

#endif // DATAMANAGERDIALOG_H
