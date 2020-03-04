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

#ifndef FEEDGOOGLE_H
#define FEEDGOOGLE_H

#include <QObject>
#include "defs.h"

namespace Ui
{
  class GoogleFeed;
}

class GoogleFeed: public QObject
{
  Q_OBJECT
public:
  explicit GoogleFeed (QObject *parent = 0); // constructor
  ~GoogleFeed (void);	     // destructor

  // functions
  bool symbolExistence (QString & symbol,
                        QString & name,
                        QString & market); // check if symbol exists

  CG_ERR_RESULT downloadData (QString symbol,    // download
                              QString timeframe, // historical
                              QString currency,  // data
                              QString task,
                              bool    adjust);
  CG_ERR_RESULT getRealTimePrice (QString symbol, 
                                   RTPrice & rtprice); // get real time price                             

  bool validSymbol (QString symbol);	// validate google symbol


private:
  // variables and classes
  RTPrice realtimeprice;// real time price
  QString Symbol;		// symbol
  QString symbolName; 	// symbolname
  QString Market;		// market
  SymbolEntry entry;						// symbol entry

  // functions
  QString symbolURL (QString symbol); // returns symbol check URL
  QString downloadURL (QString symbol); // download URL
  QString updateURL (QString symbol); // update URL
  QString realTimePriceURL (QString symbol); // real time price URL
};

#endif // FEEDGOOGLE_H
