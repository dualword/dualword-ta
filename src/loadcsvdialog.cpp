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

#include "ui_loadcsvdialog.h"
#include "common.h"
#include "feedyahoo.h"

// constructor
LoadCSVDialog::LoadCSVDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::LoadCSVDialog)
{
  QPushButton *btn;
  QFont font;		
  QPalette pal;
  QString stylesheet = "background: transparent;",
          stylesheet2 = "background: transparent; background-color: white;";
  int rc;

  ui->setupUi (this);
  fileDialog = new QFileDialog (this);
  
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
  ui->CSVFileLbl->setStyleSheet (stylesheet);
  ui->formatLbl->setStyleSheet (stylesheet);
  ui->symbolLbl->setStyleSheet (stylesheet);
  ui->nameLbl->setStyleSheet (stylesheet);
  ui->marketLbl->setStyleSheet (stylesheet);
  ui->currencyLbl->setStyleSheet (stylesheet);
  ui->timeframeLbl->setStyleSheet (stylesheet);
  ui->filenameEdit->setStyleSheet (stylesheet2);
  ui->symbolEdit->setStyleSheet (stylesheet2);
  ui->marketEdit->setStyleSheet (stylesheet2);
  ui->nameEdit->setStyleSheet (stylesheet2);
  ui->marketComboBox->setStyleSheet (stylesheet2);
  ui->timeframesComboBox->setStyleSheet (stylesheet2);
  ui->formatComboBox->setStyleSheet (stylesheet2);
  ui->currencyComboBox->setStyleSheet (stylesheet2);
  ui->filechooserButton->setStyleSheet (stylesheet2);
  ui->progressBar->setValue (0);

  // format list
  ComboItems->formatList.clear ();
  rc = sqlite3_exec(Application_Settings->db, ComboItems->formats_query,
                    sqlcb_formats, NULL, NULL);

  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
    this->hide ();
  }
  ComboItems->formatList.sort ();
  ui->formatComboBox->addItems (ComboItems->formatList);

  // timeframes
  ComboItems->timeframeList.clear ();
  rc = sqlite3_exec(Application_Settings->db, ComboItems->timeframes_query,
                    sqlcb_timeframes, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
    this->hide ();
  }
  ui->timeframesComboBox->addItems (ComboItems->timeframeList);

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

  // market list
  ComboItems->marketList.clear ();
  rc = sqlite3_exec(Application_Settings->db, ComboItems->markets_query,
                    sqlcb_markets, NULL, NULL);

  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
    this->hide ();
  }
  ui->marketComboBox->addItems (ComboItems->marketList);

  // connect to signals
  connect (ui->filechooserButton, SIGNAL (clicked ()), this,
           SLOT (filechooserButton_clicked ()));

  connect (ui->marketComboBox, SIGNAL (currentIndexChanged (const QString &)), this,
           SLOT (marketComboBox_changed (const QString &)));
  
  correctWidgetFonts (this);   
  if (parent != NULL)
    setParent (parent);      
}

// destructor
LoadCSVDialog::~LoadCSVDialog ()
{
  delete ui;
}

/// Functions
// enable/disable widgets during download
void
LoadCSVDialog::widgetsSetEnabled (bool boolean)
{
  ui->filenameEdit->setEnabled (boolean);
  ui->filechooserButton->setEnabled (boolean);
  ui->formatComboBox->setEnabled (boolean);
  ui->symbolEdit->setEnabled (boolean);
  ui->nameEdit->setEnabled (boolean);
  ui->marketEdit->setEnabled (boolean);
  ui->marketComboBox->setEnabled (boolean);
  // ui->timeframesComboBox->setEnabled (boolean);
  ui->currencyComboBox->setEnabled (boolean);
  ui->buttonBox->setEnabled (boolean);
}

/// Signals
///
// filechooserButton_clicked ()
void
LoadCSVDialog::filechooserButton_clicked ()
{
  QString fileName, title, wildcards;

  title = QString::fromUtf8 ("Select CSV file");
  wildcards = QString::fromUtf8 ("All files (*)");
  fileName = fileDialog->getOpenFileName(this, title, QDir::homePath (), wildcards);
  if (fileName != "")
    ui->filenameEdit->setText (fileName);
}

// marketComboBox_changed (const QString &)
void
LoadCSVDialog::marketComboBox_changed (const QString & str)
{
  ui->marketEdit->setText (str);
}

// accept ()
void
LoadCSVDialog::accept ()
{
  SymbolEntry symboldata;
  QString market;
  CG_ERR_RESULT err;

  ui->symbolEdit->setText (ui->symbolEdit->text ().toUpper ());
  ui->symbolEdit->setText (ui->symbolEdit->text ().trimmed ());
  ui->nameEdit->setText (ui->nameEdit->text ().toUpper ());
  ui->nameEdit->setText (ui->nameEdit->text ().trimmed ());
  setGlobalError(CG_ERR_OK, __FILE__, __LINE__);
  GlobalProgressBar = ui->progressBar;
  widgetsSetEnabled (false);
  qApp->processEvents();

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  if (ui->filenameEdit->text () == "")
  {
    showMessage ("Please select a CSV file");
    widgetsSetEnabled (true);
    ui->progressBar->setValue (0);
    return;
  }

  if (ui->symbolEdit->text () == "")
  {
    showMessage ("No symbol");
    widgetsSetEnabled (true);
    ui->progressBar->setValue (0);
    return;
  }
  
  if ("YAHOO CSV" == ui->formatComboBox->currentText ())
    symboldata.adjust = true;
  else  
    symboldata.adjust = false;
    
  if (ui->marketEdit->text () == "")
    symboldata.market = "NONE";
  else
    symboldata.market = ui->marketEdit->text ();

  symboldata.csvfile =  ui->filenameEdit->text ();
  symboldata.symbol = ui->symbolEdit->text ();
  symboldata.name = ui->nameEdit->text ();
  symboldata.timeframe = ui->timeframesComboBox->currentText ();
  symboldata.currency = ui->currencyComboBox->currentText ();
  symboldata.source = "CSV";
  symboldata.format = ui->formatComboBox->currentText ();
  symboldata.tablename =  symboldata.symbol + "_" +
                          symboldata.market + "_" +
                          symboldata.source + "_";
  if (symboldata.adjust)
    symboldata.tablename += "ADJUSTED_";
  else
    symboldata.tablename += "UNADJUSTED_";
  
  symboldata.tablename += symboldata.timeframe;
  symboldata.tmptablename = "TMP_" + symboldata.tablename;
  symboldata.dnlstring = "";

  err = csv2sqlite (&symboldata, "CREATE");
  if (err != CG_ERR_OK)
  {
    setGlobalError(err, __FILE__, __LINE__);
    showMessage (errorMessage (err));
    widgetsSetEnabled (true);
    ui->progressBar->setValue (0);
    return;
  }
  else
    showMessage ("Import completed");

  QApplication::restoreOverrideCursor();
  widgetsSetEnabled (true);
  ui->progressBar->setValue (0);
}
