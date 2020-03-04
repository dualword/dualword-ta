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

#ifndef MANUALDIALOG_H
#define MANUALDIALOG_H

#include <QUrl>
#include <QDialog>

namespace Ui
{
  class ManualDialog;
}

class ManualDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ManualDialog(QWidget *parent = 0);
  ~ManualDialog();

private:
  Ui::ManualDialog *ui;
  QString manualpath;

private slots:
  void close_clicked (void);
  void link_clicked (const QUrl &);

protected:
  virtual void resizeEvent (QResizeEvent * event);
  virtual void changeEvent(QEvent * event);
  
};

#endif // MANUALDIALOG_H
