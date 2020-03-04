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

#include "ui_downloaddatadialog.h"
#include "common.h"
#include "feedyahoo.h"
#include "feedgoogle.h"

// constructor
DownloadDataDialog::DownloadDataDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::DownloadDataDialog)
{
  QPushButton *btn;
  QFont font;		
  QPalette pal;
  QString stylesheet = "background: transparent;",
          stylesheet2 = "background: transparent; background-color: white;";
  int rc;

  ui->setupUi (this);
  
  btn = ui->buttonBox->button (QDialogButtonBox::Ok);
  font = btn->font ();
  font.setWeight (QFont::DemiBold);
  btn->setFont (font);
  btn->setFocusPolicy (Qt::NoFocus);
  
  btn = ui->buttonBox->button (QDialogButtonBox::Cancel);
  font = btn->font ();
  font.setWeight (QFont::DemiBold);
  btn->setFont (font);
  btn->setFocusPolicy (Qt::NoFocus);
  
  pal.setColor (backgroundRole (), Qt::white);
  setPalette (pal);

  ui->progressBar->setValue (0);
  ui->symbolLbl->setStyleSheet (stylesheet);
  ui->nameLbl->setStyleSheet (stylesheet);
  ui->datafeedLbl->setStyleSheet (stylesheet);
  ui->marketLbl->setStyleSheet (stylesheet);
  ui->currencyLbl->setStyleSheet (stylesheet);
  ui->symbolEdit->setStyleSheet (stylesheet2);
  ui->nameEdit->setStyleSheet (stylesheet2);
  ui->adjustLbl->setStyleSheet (stylesheet);
  ui->datafeedsComboBox->setStyleSheet (stylesheet2);
  ui->currencyComboBox->setStyleSheet (stylesheet2);

  // datafeeds list
  ComboItems->datafeedsList.clear ();
  rc = sqlite3_exec(Application_Settings->db, ComboItems->datafeeds_query,
                    sqlcb_datafeeds, NULL, NULL);

  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
    this->hide ();
  }
  ComboItems->datafeedsList.sort ();
  ui->datafeedsComboBox->addItems (ComboItems->datafeedsList);
  
  // currency list
  ComboItems->currencyList.clear ();
  rc = sqlite3_exec(Application_Settings->db, ComboItems->currencies_query,
                    sqlcb_currencies, NULL, NULL);

  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
    this->hide ();
  }
  ui->currencyComboBox->addItems (ComboItems->currencyList);

  // connect to signals
  connect (ui->checkButton, SIGNAL (clicked ()), this,
           SLOT (checkButton_clicked ()));
  
  correctWidgetFonts (this);   
  
  if (parent != NULL)
    setParent (parent);      

}

// destructor
DownloadDataDialog::~DownloadDataDialog ()
{
  delete ui;
}

/// Yahoo Finance related functions
// check if symbol exists

void
DownloadDataDialog::checkYahoosymbolExistence ()
{
  YahooFeed YF;
  QString name = "", market = "", symbol = "", currency = "";
  
  ui->nameEdit->setText ("");
  symbol = ui->symbolEdit->text ();
  if (symbol.size () == 0)
    return;

  widgetsSetEnabled (false);
  ui->checkButton->setText ("Checking");
  exists = YF.symbolExistence (symbol, name, market, currency);
  ui->symbolEdit->setText (symbol);
  ui->checkButton->setText ("Check");
  widgetsSetEnabled (true);

  if (exists)
  {
    ui->nameEdit->setText (name);
    ui->marketEdit->setText (market);
    return;
  }

  if (GlobalError != CG_ERR_OK)
    showMessage (errorMessage (GlobalError));
  else
    showMessage ("Symbol not found");
}

// download data from yahoo
void
DownloadDataDialog::downloadYahooControl ()
{
  YahooFeed YF;
  CG_ERR_RESULT result;
  
  result = YF.downloadData (ui->symbolEdit->text (),
                            "DAY",
                            ui->currencyComboBox->currentText (),
                            "DOWNLOAD",
                            ui->adjustCheckBox->isChecked ());
  
  if (result == CG_ERR_OK)
    showMessage ("Import completed");
  else
    showMessage (errorMessage (result));
}

/// Google Finance related functions
// check if symbol exists

void
DownloadDataDialog::checkGooglesymbolExistence ()
{
  GoogleFeed GF;
  QString name = "", market = "", symbol = "";
  
  GlobalError = CG_ERR_OK;
  ui->nameEdit->setText ("");
  symbol = ui->symbolEdit->text ();
  if (symbol.size () == 0)
    return;

  widgetsSetEnabled (false);
  ui->checkButton->setText ("Checking");
  exists = GF.symbolExistence (symbol, name, market);
  ui->symbolEdit->setText (symbol);
  ui->checkButton->setText ("Check");
  widgetsSetEnabled (true);

  if (exists)
  {
    ui->nameEdit->setText (name);
    ui->marketEdit->setText (market);
    return;
  }

  if (GlobalError != CG_ERR_OK)
    showMessage (errorMessage (GlobalError));
  else
    showMessage ("Symbol not found");
}

// download data from google
void
DownloadDataDialog::downloadGoogleControl ()
{
  GoogleFeed GF;
  CG_ERR_RESULT result;
  
  result = GF.downloadData (ui->symbolEdit->text (),
                            "DAY",
                            ui->currencyComboBox->currentText (),
                            "DOWNLOAD",
                            ui->adjustCheckBox->isChecked ());
  
  if (result == CG_ERR_OK)
    showMessage ("Import completed");
  else
    showMessage (errorMessage (result));
}

/// Common functions
// enable/disable widgets during download
void
DownloadDataDialog::widgetsSetEnabled (bool boolean)
{
  ui->datafeedsComboBox->setEnabled (boolean);
  ui->symbolEdit->setEnabled (boolean);
  ui->checkButton->setEnabled (boolean);
  ui->buttonBox->setEnabled (boolean);
  ui->currencyComboBox->setEnabled (boolean);
  qApp->processEvents();
}

/// Signals
///
// checkButton_clicked ()
void
DownloadDataDialog::checkButton_clicked ()
{
  // check if there is at least one data feed
  if (ComboItems->datafeedsList.size () == 0)
    return;

  setGlobalError(CG_ERR_OK, __FILE__, __LINE__);
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  ui->symbolEdit->setText (ui->symbolEdit->text ().toUpper ());
  ui->marketEdit->setText ("");
  if (ui->datafeedsComboBox->currentText () == "YAHOO")
    checkYahoosymbolExistence ();
  if (ui->datafeedsComboBox->currentText () == "GOOGLE")
    checkGooglesymbolExistence ();
  widgetsSetEnabled (true);
  QApplication::restoreOverrideCursor();
}

// accept ()
void
DownloadDataDialog::accept ()
{
  // check if there is at least one data feed
  if (ComboItems->datafeedsList.size () == 0)
    return;

  setGlobalError(CG_ERR_OK, __FILE__, __LINE__);
  GlobalProgressBar = ui->progressBar;

  checkButton_clicked ();
  if (!exists)
    return;

  widgetsSetEnabled (false);
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  if (ui->datafeedsComboBox->currentText () == "YAHOO")
    downloadYahooControl ();
  if (ui->datafeedsComboBox->currentText () == "GOOGLE")
    downloadGoogleControl ();
  
  QApplication::restoreOverrideCursor();
  widgetsSetEnabled (true);
  GlobalProgressBar->setValue (0);
}
