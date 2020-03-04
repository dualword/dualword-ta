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

#ifndef COMMON_H
#define COMMON_H

// #include <QDebug>
#include <limits>
#include <QMessageBox>
#include "defs.h"

// extern classes
#include "loadcsvdialog.h"
#include "datamanagerdialog.h"
#include "templatemanagerdialog.h"
#include "downloaddatadialog.h"
#include "progressdialog.h"
#include "waitdialog.h"
#include "qtachart.h"

#define QREAL_MAX 	(std::numeric_limits<qreal>::max())
#define QREAL_MIN	(std::numeric_limits<qreal>::min())

extern QTAChart *chartwidget;					// widget of the chart
extern LoadCSVDialog *loadcsvdialog;			// dialog to load CSV file
extern DownloadDataDialog *downloaddatadialog;	// dialog to download data
extern ProgressDialog *progressdialog;			// dialog to show progress
extern TemplateManagerDialog *templatemanager; // template manager dialog
extern SQLists *ComboItems;				// QStringLists used as combo box items
extern FrameVector Frames;				// vector of symbol's data frames
extern QStringList UserAgents;			// Http user agents

// extern functions
// show a message box
extern void
showMessage (QString message);

// show an Ok/Cancel question box
extern bool
showOkCancel (QString message);

// show demo message box
extern bool
showDemoMessage ();

// show download message box
extern bool
showDownloadMessage ();

// delay nsecs
void delay(int secs);

// returns description of an error code
extern QString
errorMessage (CG_ERR_RESULT err);

// returns full operating system description
extern QString
fullOperatingSystemVersion ();

// sqlite3_exec callback for retrieving supported data formats
extern int
sqlcb_formats(void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving supported timeframes
extern int
sqlcb_timeframes(void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving supported currencies
extern int
sqlcb_currencies(void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving markets
extern int
sqlcb_markets(void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving symbol table
extern int
sqlcb_symbol_table(void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving datafeeds
extern int
sqlcb_datafeeds(void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving symbol's data frames
extern int
sqlcb_dataframes (void *dummy, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving symbol's fundamental data 
extern int 
sqlcb_fundamentals (void *data, int argc, char **argv, char **column);

// sqlite3_exec callback for retrieving nsymbols
extern int
sqlcb_nsymbols(void *nsymptr, int argc, char **argv, char **column);

// set font size
extern void
correctFontSize (QWidget *widget);

// correct font size for widget and children
extern void
correctWidgetFonts (QWidget *widget);

void
correctWidgetFonts (QDialog *widget);

void
correctWidgetFonts (QMessageBox *widget);

// native http header
QString nativeHttpHeader ();

// random http header
QString httpHeader ();

// json parse
extern bool
json_parse (QString jsonstr, QStringList *node, QStringList *value, void *n1);
#endif 

// COMMON_H
