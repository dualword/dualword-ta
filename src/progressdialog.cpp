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

#include "ui_progressdialog.h"
#include "common.h"

// constructor
ProgressDialog::ProgressDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::ProgressDialog)
{
  ui->setupUi (this);
  setWindowTitle ("Please Wait");
  setWindowFlags (((Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint) & ~Qt::WindowMinimizeButtonHint));
  ui->progressBar->setValue (0);
  correctWidgetFonts (this);
}

// destructor
ProgressDialog::~ProgressDialog ()
{
  delete ui;
}


// set message
void
ProgressDialog::setMessage (QString message)
{
  ui->messageLabel->setText (message);
}


QProgressBar *
ProgressDialog::getProgressBar ()
{

  return ui->progressBar;
}

