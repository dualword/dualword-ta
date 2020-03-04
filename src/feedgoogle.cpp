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

#include <QApplication>
#include <QTextStream>
#include <QTemporaryFile>
#include <QAtomicInt>
#include "common.h"
#include "libjson.h"
#include "netservice.h"
#include "feedgoogle.h"

// constructor
GoogleFeed::GoogleFeed (QObject *parent)
{
  if (parent != NULL)
    setParent (parent);
}

// destructor
GoogleFeed::~GoogleFeed ()
{
  
  return;
}

// validate symbol
bool
GoogleFeed::validSymbol (QString symbol)
{
  for (qint32 counter = 0, max = symbol.size ();
              counter < max; counter ++)
  {
    if (!((symbol[counter] >= 'A' && symbol[counter] <= 'Z') ||
          (symbol[counter] >= 'a' && symbol[counter] <= 'z') ||
          (symbol[counter] >= '0' && symbol[counter] <= '9') ||
           symbol[counter] == '.' || symbol[counter] == ':' ||
           symbol[counter] == '-'))
      return false;
  }

  return true;
}

// return symbol check URL
// eg: http://www.google.com/finance/info?infotype=infoquoteall&q=C
QString
GoogleFeed::symbolURL (QString symbol)
{
  QString urlstr = "";

  if (symbol.size () == 0)
    return urlstr;

  urlstr = "http://www.google.com/finance/info?infotype=infoquoteall&q=";
  urlstr += symbol;
  return urlstr;
}

QString
GoogleFeed::realTimePriceURL (QString symbol)
{
  return symbolURL (symbol);
}

// return historical download URL
// eg:
// http://www.google.com/finance/historical?q=c&startdate=Jan+1%2C+2000&output=csv
QString
GoogleFeed::downloadURL (QString symbol)
{
  QString downstr = "";

  if (symbol.size () == 0)
    return downstr;
  
  downstr = "http://www.google.com/finance/historical?q=";
  downstr += symbol;
  downstr += "&startdate=Jan+1,+2000&output=csv";
  
  return downstr;
}

// return update URL
QString
GoogleFeed::updateURL (QString symbol)
{
  return downloadURL (symbol);
}

// get real time price
CG_ERR_RESULT 
GoogleFeed::getRealTimePrice (QString symbol, RTPrice & rtprice)
{
  QTemporaryFile tempFile;		// temporary file
  RTPrice realtimeprice;        // real time price
  QTextStream in;		
  QString url, line;	
  NetService *netservice = NULL;
  CG_ERR_RESULT result = CG_ERR_OK;
  
  url = realTimePriceURL (symbol);
  
  // open temporary file
  if (!tempFile.open ())
  {
    result = CG_ERR_CREATE_TEMPFILE;
    goto getRealTimePrice_end;
  }
  tempFile.resize (0);
  
  netservice = new NetService (Application_Settings->options.nettimeout, 
                               httpHeader ().toLatin1 (), this);
  result = netservice->httpGET (url, tempFile, NULL);
  if (result != CG_ERR_OK)
    goto getRealTimePrice_end;
  
  in.setDevice (&tempFile);
  in.seek (0);
  line = in.readAll ();
  
  if (line.size () != 0)
  {
	QStringList node, value;
    
    if (json_parse (line, &node, &value, NULL))
    {
	  for (qint32 counter = 0; counter < node.size (); counter ++)
	  {
		if (node[counter].toLower () == "l_fix")
		  realtimeprice.price = value[counter]; 
		
		if (node[counter].toLower () == "c_fix")  
		  realtimeprice.change = value[counter]; 
		
		if (node[counter].toLower () == "cp_fix")    
		  realtimeprice.prcchange = value[counter].simplified() + QString ("%");
      }	
    }
    else
      result = CG_ERR_INVALID_DATA;    
  }	    

getRealTimePrice_end:
  setGlobalError(result, __FILE__, __LINE__);      
  realtimeprice.symbol = symbol;
  realtimeprice.feed = "GOOGLE";
  rtprice = realtimeprice;
  tempFile.close ();
  if (netservice != NULL)
    delete netservice;
  return result;
  
}

// check if symbol exists
bool
GoogleFeed::symbolExistence (QString & symbol, QString & name, QString & market)
{
  QTemporaryFile tempFile;		// temporary file
  QTextStream in;	
  QString urlstr, line;	
  QStringList token;
  NetService *netservice = NULL;
  CG_ERR_RESULT ioresult = CG_ERR_OK;
  bool result = false;
  
  symbol = symbol.trimmed ();
  if (!validSymbol (symbol))
    goto symbolExistence_end;
  
  urlstr = symbolURL (symbol);
  if (urlstr.size () == 0)
    goto symbolExistence_end;

  Symbol = symbol;
  symbolName = name;
  
  // open temporary file
  if (!tempFile.open ())
  {
    ioresult = CG_ERR_CREATE_TEMPFILE;
    goto symbolExistence_end;
  }
  tempFile.resize (0);
  
  netservice = new NetService (Application_Settings->options.nettimeout, 
                               httpHeader ().toLatin1 (), this);
  ioresult = netservice->httpGET (urlstr, tempFile, NULL);
  if (ioresult != CG_ERR_OK)
    goto symbolExistence_end;
  
  in.setDevice (&tempFile);
  in.seek (0);
  line = in.readAll ();
  
  if (line.size () != 0)
  {
	QStringList node, value;  
   
    if (json_parse (line, &node, &value, NULL))
    {
	  for (qint32 counter = 0; counter < node.size (); counter ++)
	  {
		if (node[counter].toLower () == "name")
		  symbolName = value[counter].simplified();
		
		if (node[counter].toLower () == "e")  
		  Market = value[counter].simplified();
      }
      name = symbolName;
      market = Market;
      result = true;
    }
    else
      ioresult = CG_ERR_INVALID_DATA; 
  }
  else   
    result = false;
  
symbolExistence_end:  
  setGlobalError(ioresult, __FILE__, __LINE__);
  
  if (result == false)
    Symbol = "";
    
  tempFile.close ();
  if (netservice != NULL)
    delete netservice;
    
  return result;
}

// download historical data
CG_ERR_RESULT
GoogleFeed::downloadData (QString symbol, QString timeframe, QString currency, 
                         QString task, bool adjust)
{
  QTemporaryFile tempFile;		// temporary file	
  QString url;	
  NetService *netservice = NULL;
  CG_ERR_RESULT result = CG_ERR_OK;
    
  if (GlobalProgressBar != NULL)
    GlobalProgressBar->setValue (0);
  
  // check symbol existence
  if (symbol != Symbol)
  {
    if (!symbolExistence (symbol, entry.name, entry.market))
    {
	  result = GlobalError.fetchAndAddAcquire (0);	
      if (result == CG_ERR_OK)
        result = CG_ERR_NOSYMBOL;
      return result;
    }
  }

  // open temporary file
  if (!tempFile.open ())
  {
    result = CG_ERR_CREATE_TEMPFILE;
    setGlobalError(result, __FILE__, __LINE__);
    return result;
  }
  tempFile.resize (0);
 
  // fill symbol entry
  entry.symbol = Symbol;
  entry.timeframe = timeframe;
  entry.csvfile = tempFile.fileName ();
  entry.source = "GOOGLE";
  entry.format = "GOOGLE CSV";
  entry.currency = currency;
  entry.name = symbolName;
  entry.market = Market;
  entry.adjust = adjust;
  entry.tablename = entry.symbol % "_" %
                    entry.market % "_" %
                    entry.source % "_";
  entry.tablename += entry.timeframe;
  entry.tmptablename = "TMP_" + entry.tablename;
  url = downloadURL (symbol);
  entry.dnlstring = url;
  
  netservice = new NetService (Application_Settings->options.nettimeout, 
                               httpHeader ().toLatin1 (), this);
  result = netservice->httpGET (url, tempFile, NULL);
  if (result != CG_ERR_OK)
    goto downloadData_end;
  
  if (GlobalProgressBar != NULL)
    GlobalProgressBar->setValue (50);
  qApp->processEvents(QEventLoop::ExcludeUserInputEvents, 10); 
   
  if (task == "DOWNLOAD")
    result = csv2sqlite (&entry, "CREATE");
  else
    result = csv2sqlite (&entry, "UPDATE");
  
  if (GlobalProgressBar != NULL)
    GlobalProgressBar->setValue (100);
  qApp->processEvents(QEventLoop::ExcludeUserInputEvents, 10); 

downloadData_end:  
  setGlobalError(result, __FILE__, __LINE__);
  tempFile.close ();
  if (netservice != NULL)
    delete netservice;
  return result;  
}
