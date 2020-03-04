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

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui
{
  class OptionsDialog;
}

class OptionsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit OptionsDialog(QWidget *parent = 0);
  ~OptionsDialog();
  
private:
  Ui::OptionsDialog *ui;
  
  void loadOptions (void);		// load application options
  void saveOptions (void);		// save application options
   
private slots:
  void ok_clicked (void);
  void cancel_clicked (void);  
  void newVersionBtn_clicked (void);  

protected:
  virtual void closeEvent (QCloseEvent * event);    
  virtual void showEvent (QShowEvent * event); 
};

#endif // OPTIONSDIALOG_H
