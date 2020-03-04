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

#ifndef DOWNLOADDATADIALOG_H
#define DOWNLOADDATADIALOG_H

#include <QDialog>
#include "defs.h"

namespace Ui
{
  class DownloadDataDialog;
}

class DownloadDataDialog : public QDialog
{
  Q_OBJECT

public:
  explicit DownloadDataDialog(QWidget *parent = 0);
  ~DownloadDataDialog();
  
private:
  Ui::DownloadDataDialog *ui;

  bool exists;						   // symbol exists: true, false

  /// Common functions
  void widgetsSetEnabled (bool boolean); // enable/disable widgets during download

  /// Yahoo finance related function
  void checkYahoosymbolExistence ();     // check if symbol exists
  void downloadYahooControl ();		   // control data download from yahoo
  
  /// Google finance related function
  void checkGooglesymbolExistence ();     // check if symbol exists
  void downloadGoogleControl ();		   // control data download from google

private slots:
  void accept ();
  void checkButton_clicked ();
};

#endif // DOWNLOADDATADIALOG_H
