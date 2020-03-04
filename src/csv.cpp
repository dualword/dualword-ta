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

#include <QDate>
#include <QTextStream>
#include <QTemporaryFile>
#include "common.h"

// create csv file from raw YAHOO finance csv file
static CG_ERR_RESULT
yahoo2csv_old (const QString & namein, const QString & nameout)
{
  QFile rawcsv, csv;
  QString inputline;
  QStringList values;
  CG_ERR_RESULT retval = CG_ERR_OK;

  rawcsv.setFileName (namein);
  csv.setFileName (nameout);
  if (rawcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    if (!csv.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      rawcsv.close();
      return CG_ERR_OPEN_FILE;
    }
    QTextStream in (&rawcsv);
    QTextStream out (&csv);
    inputline = in.readLine (0);	// header
    if (!in.atEnd ())
      inputline = in.readLine (0);	// first line
    while (!in.atEnd ())
    {
      inputline += ",00:00.00";
      out << inputline << "\n";
      inputline = in.readLine (0);
    }
    if (inputline.size() > 0)
      out << inputline << "\n";
  }
  else
    return CG_ERR_OPEN_FILE;

  rawcsv.close ();
  csv.close ();
  return retval;
}


// create csv file from raw YAHOO finance csv file
static CG_ERR_RESULT
yahoo2csv (const QString & namein, const QString & nameout)
{
  QFile rawcsv, csv;
  QString inputline, outputline;
  QStringList values, column;
  CG_ERR_RESULT retval = CG_ERR_OK;

  inputline.reserve (512);
  rawcsv.setFileName (namein);
  csv.setFileName (nameout);
  if (rawcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    if (!csv.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      rawcsv.close();
      return CG_ERR_OPEN_FILE;
    }
    QTextStream in (&rawcsv);
    QTextStream out (&csv);
    in.seek (0);
    inputline = in.readLine (0);	// header
    if (!in.atEnd ())
      inputline = in.readLine (0);	// first line
    while (!in.atEnd ())
    {
      inputline += QStringLiteral (",00:00.00");
      column = inputline.split (QStringLiteral(","), QString::KeepEmptyParts);
      
      /*
      float r = column[5].toFloat () / column[4].toFloat ();
      column[1] = QString::number (column[1].toFloat () / r);
      column[3] = QString::number (column[3].toFloat () / r);
      column[2] = QString::number (column[2].toFloat () / r);
      */
      
      outputline = 
        column[0] % QStringLiteral (",") % // date
        column[1] % QStringLiteral (",") % // open 
        column[2] % QStringLiteral (",") % // high
        column[3] % QStringLiteral (",") % // low
        column[4] % QStringLiteral (",") % // close
        column[6] % QStringLiteral (",") % // volume
        column[5] % QStringLiteral (",") % // adj close
        column[7]; // time
      
      
      out << outputline << QStringLiteral ("\n");
      inputline = in.readLine (0);
    }
    if (inputline.size() > 0)
      out << inputline << QStringLiteral ("\n");
  }
  else
    return CG_ERR_OPEN_FILE;

  rawcsv.close ();
  csv.close ();
  return retval;
}


// create csv file from raw GOOGLE finance csv file
static CG_ERR_RESULT
google2csv (const QString & namein, const QString & nameout)
{
  QFile rawcsv, csv;
  QString inputline;
  QStringList values;
  CG_ERR_RESULT retval = CG_ERR_OK;

  rawcsv.setFileName (namein);
  csv.setFileName (nameout);
  if (rawcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    if (!csv.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      rawcsv.close();
      return CG_ERR_OPEN_FILE;
    }
    QTextStream in (&rawcsv);
    QTextStream out (&csv);
    inputline = in.readLine (0);	// header
    if (!in.atEnd ())
      inputline = in.readLine (0);	// first line
    while (!in.atEnd ())
    {
	  QStringList column, datetok;
	  QString outputline, datestr;
      inputline += ",00:00.00";
      column = inputline.split (",", QString::KeepEmptyParts);
      datetok = column[0].split ("-", QString::KeepEmptyParts);
      if (datetok.size () < 3)
      {
		retval = CG_ERR_INVALID_DATA;  
        goto google2csv_end;
      }
      if (datetok[2].toInt () < 100)
      {
        datestr = (datetok[2].toInt () > 50?
                   QString::number(datetok[2].toInt () + 1900):
                   QString::number(datetok[2].toInt () + 2000));
      }
      else
        datestr = QString::number(datetok[2].toInt ());
      datestr += "-";           
      if (datetok[1] == "Jan") datestr += "01"; 
      if (datetok[1] == "Feb") datestr += "02"; 
      if (datetok[1] == "Mar") datestr += "03"; 
      if (datetok[1] == "Apr") datestr += "04"; 
      if (datetok[1] == "May") datestr += "05"; 
      if (datetok[1] == "Jun") datestr += "06"; 
      if (datetok[1] == "Jul") datestr += "07"; 
      if (datetok[1] == "Aug") datestr += "08"; 
      if (datetok[1] == "Sep") datestr += "09"; 
      if (datetok[1] == "Oct") datestr += "10"; 
      if (datetok[1] == "Nov") datestr += "11"; 
      if (datetok[1] == "Dec") datestr += "12";
      datestr += "-"; 
      datestr += QString().sprintf("%02d", datetok[0].toInt ());
      outputline = datestr + ",";
      
      if (column[4] == "-") column[4] = "0";
      if (column[1] == "-" || column[1].toFloat () == 0) column[1] = column[4];
      if (column[2] == "-" || column[2].toFloat () == 0) column[2] = column[4];
      if (column[3] == "-" || column[3].toFloat () == 0) column[3] = column[4];
      if (column[5] == "-") column[5] = "0";
      outputline += column[1] + ",";
      outputline += column[2] + ",";
      outputline += column[3] + ",";
      outputline += column[4] + ",";
      outputline += column[5] + ",";
      outputline += column[4] + ",";
      outputline += column[6];
      out << outputline << "\n";
      inputline = in.readLine (0);
    }
    if (inputline.size() > 0)
      out << inputline << "\n";
  }
  else
    return CG_ERR_OPEN_FILE;

google2csv_end:
  rawcsv.close ();
  csv.close ();
  return retval;
}

// create csv file from raw STANDARD csv file
static CG_ERR_RESULT
standard2csv (const QString & namein, const QString & nameout, QString & symbolname)
{
  QFile rawcsv, csv;
  QString inputline;
  QStringList values;
  CG_ERR_RESULT retval = CG_ERR_OK;

  rawcsv.setFileName (namein);
  csv.setFileName (nameout);
  if (rawcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    if (!csv.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      rawcsv.close();
      return CG_ERR_OPEN_FILE;
    }
    QTextStream in (&rawcsv);
    QTextStream out (&csv);
    if (!in.atEnd ())
      inputline = in.readLine (0);	// first line
    while (!in.atEnd ())
    {
	  QStringList column, datetok;
	  QString outputline, datestr;
      inputline += ",00:00.00";
      column = inputline.split (",", QString::KeepEmptyParts);
      symbolname = column[0];
      datetok = column[1].split ("-", QString::KeepEmptyParts);
      datestr =  QString::number(datetok[2].toInt ());
      datestr += "-";           
      if (datetok[1] == "Jan") datestr += "01"; 
      if (datetok[1] == "Feb") datestr += "02"; 
      if (datetok[1] == "Mar") datestr += "03"; 
      if (datetok[1] == "Apr") datestr += "04"; 
      if (datetok[1] == "May") datestr += "05"; 
      if (datetok[1] == "Jun") datestr += "06"; 
      if (datetok[1] == "Jul") datestr += "07"; 
      if (datetok[1] == "Aug") datestr += "08"; 
      if (datetok[1] == "Sep") datestr += "09"; 
      if (datetok[1] == "Oct") datestr += "10"; 
      if (datetok[1] == "Nov") datestr += "11"; 
      if (datetok[1] == "Dec") datestr += "12";
      datestr += "-"; 
      datestr += QString().sprintf("%02d", datetok[0].toInt ());
      outputline = datestr + ",";
      outputline += column[2] + ",";
      outputline += column[3] + ",";
      outputline += column[4] + ",";
      outputline += column[5] + ",";
      outputline += column[6] + ",";
      outputline += column[5] + ",";
      outputline += column[7];
      out << outputline << "\n";
      inputline = in.readLine (0);
    }
    if (inputline.size() > 0)
      out << inputline << "\n";
  }
  else
    return CG_ERR_OPEN_FILE;

  rawcsv.close ();
  csv.close ();
  return retval;
}

// create csv file from AMI BROKER or METASTOCK7 csv file
static CG_ERR_RESULT
ami2csv (const QString & namein, const QString & nameout, QString & symbolname)
{
  QFile rawcsv, csv;
  QString inputline;
  QStringList values;
  CG_ERR_RESULT retval = CG_ERR_OK;
  
  inputline.reserve (512);
  rawcsv.setFileName (namein);
  csv.setFileName (nameout);
  if (rawcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    if (!csv.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      rawcsv.close();
      return CG_ERR_OPEN_FILE;
    }
    QTextStream in (&rawcsv);
    QTextStream out (&csv);
    in.seek (0);
    do 
    {
	  QStringList column, datetok;
	  QString outputline, datestr;
	  
	  datestr.reserve (32);
	  outputline.reserve (512);
	  
	  inputline = in.readLine (0);
	  if (inputline.size() > 0)
	  {
        inputline += ",00:00.00";
        column = inputline.split (",", QString::KeepEmptyParts);
        symbolname = column[0];
        datetok << column[1].right (2);
        datetok << column[1].mid (4,2);
        datetok << column[1].left (4);
        datestr =  datetok[2] % "-" % datetok[1] % "-" % datetok[0];
        outputline = datestr   % "," %
                     column[2] % "," %
                     column[3] % "," %
                     column[4] % "," %
                     column[5] % "," %
                     column[6] % "," %
                     column[5] % "," %
                     column[7];
        out << outputline << "\n";
      }  
    } while (!in.atEnd ());
  }
  else
    return CG_ERR_OPEN_FILE;
  
  rawcsv.close ();
  csv.close ();
  return retval;
}

// create csv file from AMI BROKER or METASTOCK8 csv file
static CG_ERR_RESULT
metastock8csv (const QString & namein, const QString & nameout, QString & symbolname)
{
  QFile rawcsv, csv;
  QString inputline;
  QStringList values;
  CG_ERR_RESULT retval = CG_ERR_OK;

  rawcsv.setFileName (namein);
  csv.setFileName (nameout);
  if (rawcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    if (!csv.open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      rawcsv.close();
      return CG_ERR_OPEN_FILE;
    }
    QTextStream in (&rawcsv);
    QTextStream out (&csv);
    if (!in.atEnd ())
      inputline = in.readLine (0);	// first line
    while (!in.atEnd ())
    {
	  QStringList column, datetok;
	  QString outputline, datestr;
      inputline += ",00:00.00";
      column = inputline.split (",", QString::KeepEmptyParts);
      symbolname = column[0];
      datetok << column[2].right (2);
      datetok << column[2].mid (4,2);
      datetok << column[2].left (4);
      datestr =  datetok[2];
      datestr += "-";
      datestr += datetok[1];
      datestr += "-";
      datestr += datetok[0];
      outputline = datestr + ",";
      outputline += column[3] + ",";
      outputline += column[4] + ",";
      outputline += column[5] + ",";
      outputline += column[6] + ",";
      outputline += column[7] + ",";
      outputline += column[6] + ",";
      outputline += column[8];
      out << outputline << "\n";
      inputline = in.readLine (0);
    }
    if (inputline.size() > 0)
      out << inputline << "\n";
  }
  else
    return CG_ERR_OPEN_FILE;

  rawcsv.close ();
  csv.close ();
  return retval;
}

/// General purpose csv functions
// normalize table names
static void
tablename_normal (QString & tname)
{
  QStringList column, oldc, substc;

  oldc   <<"."<<"="<<"/"<<"!"<<"@"<<"#"<<"$"<<"%"<<"^"<<"&"<<"*"<<"("<<")"<<
         "+"<<"-"<<","<< " "<<":" << "[" << "]" << "{" << "}" << "|"<<"<"<<">";
  substc <<"_"<<"a"<<"b"<<"c"<<"d"<<"e"<<"f"<<"g"<<"h"<<"i"<<"j"<<"k"<<"l"<<
         "m"<<"n" << "o"<<"p"<<"q"<<"r"<<"s"<<"t"<<"u"<<"v"<<"w"<<"x"<<"y"<<"z";

  for (qint32 counter = 0, maxcounter = oldc.size (); 
       counter < maxcounter; counter ++)
    tname.replace (oldc[counter], substc[counter]);

  if (tname.indexOf(QRegExp("[0-9]"), 0) == 0)
    tname.insert(0, QString("_"));
}

// Return and SQL statement to drop a view if exists
static QString
dropview (QString viewname)
{
  return "DROP VIEW IF EXISTS " + viewname + ";"; 	
}

// Returns an SQL statement that creates the timeframe views
static QString
tfview (QString tablename, QString tf, SymbolEntry *data, QString operation, bool adjust)
{
  QString SQLCommand = "",	
          vname,
          viewclose, viewopen, viewhigh, viewlow, viewmindate, 
          viewmaxdate, viewvolume;
  
  vname = tablename + "_" + tf;
    
  if (operation == "UPDATE")
    goto tfview_end;	
  
  viewclose = vname + "_" + "_CLOSE";
  viewopen = vname + "_" + "_OPEN";
  viewhigh = vname + "_" + "_HIGH";
  viewlow = vname + "_" + "_LOW";
  viewmindate = vname + "_" + "_MINDATE";
  viewmaxdate = vname + "_" + "_MAXDATE";
  viewvolume = vname + "_" + "_VOLUME";
  
  // drop all views 
  SQLCommand += dropview (vname) +
                dropview (viewclose) +
                dropview (viewopen) +
                dropview (viewhigh) +
                dropview (viewlow) +
                dropview (viewmindate) +
                dropview (viewmaxdate) +
                dropview (viewvolume);
  SQLCommand.append ('\n');
  
  // maxdate 
  SQLCommand += "CREATE VIEW " + viewmaxdate + 
  " AS SELECT MAX (DATE) AS DATE, " + tf + " FROM " + 
  tablename + " GROUP BY " + tf + ";";
  SQLCommand.append ('\n');
  
  // mindate
  SQLCommand += "CREATE VIEW " + viewmindate + 
  " AS SELECT MIN (DATE) AS DATE, " + tf + " FROM " + 
  tablename + " GROUP BY " + tf + ";";
  SQLCommand.append ('\n');
  
  // tf low
  SQLCommand += "CREATE VIEW " + viewlow + 
  " AS SELECT MIN (LOW) AS LOW, " + tf + " FROM " + 
  tablename + " GROUP BY " + tf + ";";
  SQLCommand.append ('\n');
  
  // tf high
  SQLCommand += "CREATE VIEW " + viewhigh + 
  " AS SELECT MAX (HIGH) AS HIGH, " + tf + " FROM " + 
  tablename + " GROUP BY " + tf + ";";
  SQLCommand.append ('\n');
  
  // tf open
   SQLCommand += "CREATE VIEW " + viewopen + 
   " AS SELECT " + tablename + ".OPEN, " +
   tablename + "." + tf + ", "+
   tablename + ".DATE, " +
   tablename + ".TIME FROM " +
   tablename + ", " + viewmindate + " WHERE " +
   tablename + ".DATE = " + viewmindate + ".DATE AND " + 
   tablename + "." + tf + " =  " + viewmindate + "." + tf + ";";
   SQLCommand.append ('\n');
  
  // tf close
   SQLCommand += "CREATE VIEW " + viewclose +
   " AS SELECT " + tablename + ".CLOSE, " +
   tablename + ".CLOSE AS ADJCLOSE, " +
   tablename + "." + tf + " FROM " + tablename + 
   ", " + viewmaxdate + " WHERE " + 
   tablename + ".DATE = " + viewmaxdate + ".DATE AND " + 
   tablename + "." + tf + " =  " + viewmaxdate + "." + tf + ";";
   SQLCommand.append ('\n');
   
   // tf volume
   SQLCommand += "CREATE VIEW " + viewvolume +
   " AS SELECT SUM (" + tablename + ".VOLUME ) AS VOLUME, " +
   tablename + "." + tf + " FROM " + 
   tablename + " GROUP BY " + tf + ";";
   SQLCommand.append ('\n');
   
   // final tf view
   SQLCommand += "CREATE VIEW " + vname +
   " AS SELECT " + 
   viewhigh + ".HIGH AS HIGH, " +
   viewlow + ".LOW AS LOW, " + 
   viewopen + ".OPEN AS OPEN, " + 
   viewclose + ".CLOSE AS CLOSE, " +
   viewvolume + ".VOLUME AS VOLUME, " +
   viewopen + ".DATE AS DATE, " + 
   viewopen + ".TIME AS TIME, " + 
   viewclose + ".ADJCLOSE AS ADJCLOSE FROM " +
   viewhigh + ", " + viewlow + ", " + viewopen + ", " + 
   viewclose + ", " + viewvolume + " WHERE " +
   viewhigh + "." + tf + " = " + viewlow + "." + tf + " AND " + 
   viewlow + "." + tf + " = " + viewopen + "." + tf + " AND " +
   viewopen + "." + tf + " = " + viewclose + "." + tf + " AND " +
   viewclose + "." + tf + " = " + viewvolume + "." + tf + ";";
   SQLCommand.append ('\n');
   
   // remove symbol from symbols table
  SQLCommand += "delete from SYMBOLS where KEY=\""+ vname +"\";";
  SQLCommand.append ('\n');

  // insert symbol into symbols table
  SQLCommand += "INSERT INTO SYMBOLS VALUES ('" +
                data->symbol + "','" +
                data->name + "','" +
                data->market + "','" +
                data->source + "','" +
                tf + "','" +
                " " + "','" +
                " " + "','" +
                vname + "','" +
                data->currency + "',0,'" +
                data->dnlstring + "',";
  if (adjust)
    SQLCommand += "'YES', ";
  else  
    SQLCommand += "'NO', ";
  SQLCommand += "'" + data->tablename + "');";      
  SQLCommand.append ('\n');
  
  SQLCommand.append ('\n');
    
tfview_end:
  // update DATEFROM, DATETO and TFRESOLUTION of symbol entry
  
  SQLCommand += "update SYMBOLS set DATEFROM = (select min(DATE) from " +
                vname + ") where KEY=\"" + vname + "\";";
  SQLCommand.append ('\n');

  SQLCommand += "update SYMBOLS set DATETO = (select max(DATE) from " +
                vname + ") where KEY=\""+ vname + "\";";
  SQLCommand.append ('\n');

  SQLCommand += "update SYMBOLS set TFRESOLUTION = (select MINUTE_RESOLUTION ";
  SQLCommand += "from TIMEFRAMES where SYMBOLS.TIMEFRAME = TIMEFRAMES.TIMEFRAME);";
  
 
  return SQLCommand;  
}


// CSV 2 SQLITE:
// Returns CG_ERR_OK on success, an error otherwise
CG_ERR_RESULT
csv2sqlite (SymbolEntry *data, QString operation)
{
  QDate datevar;	
  QFile tmpcsv;
  QTemporaryFile *tempfile;
  QString tempfilename, SQLCommand = "", inputline, 
          indexname, daynum, weeknum, adjustedviewname,
          symbol;
  CG_ERR_RESULT result = CG_ERR_OK;
  int rc;
  
  tablename_normal (data->tablename);
  tablename_normal (data->tmptablename);
  adjustedviewname = data->tablename + "_ADJUSTED";
  data->name.remove ("'");

  tempfile = new QTemporaryFile ;
  if (tempfile->open())
    tempfilename = tempfile->fileName ();
  else
  {
    result = CG_ERR_OPEN_FILE;
    setGlobalError(result, __FILE__, __LINE__);
  }
  delete tempfile;
  
  if (result == CG_ERR_OK)
  {
    if (data->format == "YAHOO CSV")
    {
	  data->adjust = true;	
      result = yahoo2csv (data->csvfile, tempfilename);
    } 
     
    if (data->format == "GOOGLE CSV")
    {
	  data->adjust = false;	
      result = google2csv (data->csvfile, tempfilename);
    }  
    
    if (data->format == "STANDARD CSV")
    {
	  data->adjust = false;	
      result = standard2csv (data->csvfile, tempfilename, symbol);
    }
    
    if (data->format == "AMI BROKER" ||
        data->format == "METASTOCK ASCII 7")
    {
	  data->adjust = false;	
      result = ami2csv (data->csvfile, tempfilename, symbol);
    }
    
    if (data->format == "METASTOCK ASCII 8")
    {
	  data->adjust = false;	
      result = metastock8csv (data->csvfile, tempfilename, symbol);
    }
  }
  
  if (result != CG_ERR_OK)
    goto csv2sqlite_end;
 
  // begin
   SQLCommand += "begin;";
   SQLCommand.append ('\n');

  // drop temporary table
  SQLCommand += "drop table if exists " + data->tmptablename + "; ";

  // create temporary table
  SQLCommand += "create table " + data->tmptablename +
                " as select * from DATAMODEL;";
  
  // fill temporary table
  tmpcsv.setFileName (tempfilename);
  if (tmpcsv.open (QIODevice::ReadOnly|QIODevice::Text))
  {
    QTextStream in (&tmpcsv);
    QString lastclose = "0.01";
    
    if (!in.atEnd ())
      inputline = in.readLine (0);
    while (!in.atEnd ())
    {
	  QStringList column, yyyymmdd;	
      if (GlobalProgressBar->value () < 101)
        GlobalProgressBar->setValue (GlobalProgressBar->value () + 1);
      column = inputline.split(",", QString::KeepEmptyParts);
      yyyymmdd = column[0].split("-", QString::KeepEmptyParts);
      
      for (qint32 counter = 0; counter < 8; counter ++)
      {
        if (column[counter] == "")
          column[counter] = "0";
      }
      
      // ATTENTION: Fix zero values. Possible FDIV error
      if (column[4].toFloat () <= 0.01)
        column[4] = lastclose;
      
      if (column[1].toFloat () <= 0.01)
        column[1] = column[4];
        
      if (column[2].toFloat () <= 0.01)
        column[2] = column[4];
        
      if (column[3].toFloat () <= 0.01)
        column[3] = column[4];
      
      if (column[5].toFloat () == 0.0)
        column[5] = "0";
      
      if (column[6].toFloat () <= 0.01)
        column[6] = column[4];
        
      lastclose = column[4];  
      // END OF FIX 
      
      datevar.setDate (yyyymmdd[0].toInt (), 
                       yyyymmdd[1].toInt (), 
                       yyyymmdd[2].toInt ());
      daynum = QString::number(datevar.dayOfYear ());
      if (daynum.size () == 1)
        daynum = "00" + daynum;
      if (daynum.size () == 2)
        daynum = "0" + daynum;
      daynum = yyyymmdd[0] + daynum;  
      
      if (datevar.weekNumber (NULL) == 1 &&  yyyymmdd[1].toInt () == 12)
      {
        weeknum = "01";
        weeknum = QString::number (yyyymmdd[0].toInt () + 1) + weeknum;
      }  
      else   
      {
        weeknum = QString::number(datevar.weekNumber (NULL));
        if (weeknum.size () == 1)
          weeknum = "0" + weeknum;
        weeknum = yyyymmdd[0] + weeknum;  
      }    
                                   
      SQLCommand.append ('\n');
      SQLCommand += "insert into " + data->tmptablename +
      " (OPEN, HIGH, LOW, CLOSE, VOLUME, ADJCLOSE, MONTH, YEAR, DAY, WEEK, DATE, TIME) values (" +
                    column[1] + "," +
                    column[2] + "," +
                    column[3] + "," +
                    column[4] + "," +
                    column[5] + "," +
                    column[6] + "," +
                    yyyymmdd[0] + yyyymmdd[1] + "," +
                    yyyymmdd[0] + "," +
                    daynum + "," +
                    weeknum + "," +
                    "'" + column[0] + "',"
                    "'" + column[7] + "');";
      inputline = in.readLine (0);
    }
    tmpcsv.close ();
  }
  SQLCommand.append ('\n');

  // drop data table
  SQLCommand += "drop table if exists " + data->tablename + "; ";
  SQLCommand.append ('\n'); 

  // create data table
  SQLCommand += "create table " + data->tablename +
                " (OPEN REAL NOT NULL, HIGH REAL  NOT NULL," +
                " LOW REAL NOT NULL, CLOSE REAL NOT NULL, VOLUME INTEGER," +
                " ADJCLOSE REAL NOT NULL, DATE TEXT NOT NULL, TIME TEXT," +
                " MONTH INTEGER, YEAR INTEGER, DAY INTEGER, WEEK INTEGER, " +
                " TICK INTEGER PRIMARY KEY AUTOINCREMENT);";
  SQLCommand.append ('\n');               

  // copy temporary table to data table
  SQLCommand += "insert into " + data->tablename +
                " (OPEN, HIGH, LOW, CLOSE, VOLUME, ADJCLOSE, MONTH, YEAR, DAY, WEEK, DATE, TIME)" +
                " select OPEN, HIGH, LOW, CLOSE, VOLUME, ADJCLOSE, MONTH, YEAR, DAY, WEEK, DATE, TIME from " +
                data->tmptablename + " GROUP BY DATE, TIME ORDER BY DATE DESC;";
  SQLCommand.append ('\n');                
  
  // correct ADJCLOSE if invalid
  SQLCommand += "update " + data->tablename + 
                " set ADJCLOSE = 0 where CLOSE = 0;";
  SQLCommand += "update " + data->tablename + 
                " set ADJCLOSE = CLOSE where ADJCLOSE < 0;";  
  SQLCommand.append ('\n');                                        
  
  // remove symbol from symbols table
  SQLCommand += "delete from SYMBOLS where KEY=\""+ data->tablename +"\";";
  SQLCommand.append ('\n');

  // insert symbol into symbols table
  SQLCommand += "insert into SYMBOLS values ('" +
                data->symbol + "','" +
                data->name + "','" +
                data->market + "','" +
                data->source + "','" +
                data->timeframe + "','" +
                " " + "','" +
                " " + "','" +
                data->tablename + "','" +
                data->currency + "',0,'" +
                data->dnlstring + "',";
  SQLCommand += "'NO', ";
  SQLCommand += "'" + data->tablename + "');";            
  SQLCommand.append ('\n');

  // update DATEFROM, DATETO and TFRESOLUTION of symbol entry
  SQLCommand += "update SYMBOLS set DATEFROM = (select min(DATE) from " +
                data->tablename + ") where KEY=\""+ data->tablename + "\";";
  SQLCommand.append ('\n');

  SQLCommand += "update SYMBOLS set DATETO = (select max(DATE) from " +
                data->tablename + ") where KEY=\""+ data->tablename + "\";";
  SQLCommand.append ('\n');

  SQLCommand += "update SYMBOLS set TFRESOLUTION = (select MINUTE_RESOLUTION ";
  SQLCommand += "from TIMEFRAMES where SYMBOLS.TIMEFRAME = TIMEFRAMES.TIMEFRAME);";

  // drop temporary table
  SQLCommand += "drop table if exists " + data->tmptablename + "; ";
  SQLCommand.append ('\n');
  
  // create indeces
  indexname = data->tablename + "_monthidx";
  SQLCommand += "CREATE INDEX \"" + indexname + "\" on " +
                 data->tablename + " (MONTH ASC);";
  SQLCommand.append ('\n');
                
  indexname = data->tablename + "_dateidx";
  SQLCommand += "CREATE INDEX \"" + indexname + "\" on " +
                 data->tablename + " (DATE ASC);";
  SQLCommand.append ('\n');
  
  indexname = data->tablename + "_weekidx";
  SQLCommand += "CREATE INDEX \"" + indexname + "\" on " +
                 data->tablename + " (WEEK ASC);";
  SQLCommand.append ('\n');
  
  indexname = data->tablename + "_yearidx";
  SQLCommand += "CREATE INDEX \"" + indexname + "\" on " +
                 data->tablename + " (YEAR ASC);";
  SQLCommand.append ('\n');
  
  // create week, month, year views for unadjusted data
  if (operation == "CREATE" /*&& data->source != "CSV"*/)
  {
    SQLCommand += tfview (data->tablename, "WEEK", data, operation, false);
    SQLCommand += tfview (data->tablename, "MONTH", data, operation, false);
    SQLCommand += tfview (data->tablename, "YEAR", data, operation, false);
  }
 
 // create adjusted view
  if (operation == "CREATE" && data->adjust)
  {
    SQLCommand += dropview (adjustedviewname);
    SQLCommand.append ('\n');
    SQLCommand += "CREATE VIEW " + adjustedviewname + " AS SELECT "; 
    SQLCommand += data->tablename + ".OPEN*(" + data->tablename + ".ADJCLOSE/" + data->tablename + ".CLOSE) AS OPEN,";
    SQLCommand += data->tablename + ".HIGH*(" + data->tablename + ".ADJCLOSE/" + data->tablename + ".CLOSE) AS HIGH,"; 
    SQLCommand += data->tablename + ".LOW*(" + data->tablename + ".ADJCLOSE/" + data->tablename + ".CLOSE) AS LOW,"; 
    SQLCommand += data->tablename + ".CLOSE*(" + data->tablename + ".ADJCLOSE/" + data->tablename + ".CLOSE) AS CLOSE,";
    SQLCommand += data->tablename + ".VOLUME as VOLUME," + data->tablename + ".ADJCLOSE as ADJCLOSE,";
    SQLCommand += data->tablename + ".DATE as DATE," + data->tablename + ".TIME as TIME," ; 
    SQLCommand += data->tablename + ".MONTH as MONTH," + data->tablename + ".YEAR as YEAR,"; 
    SQLCommand += data->tablename + ".DAY as DAY," + data->tablename + ".WEEK as WEEK," ;
    SQLCommand += data->tablename + ".TICK as TICK from " + data->tablename +";";  
    SQLCommand.append ('\n'); 
    SQLCommand += "UPDATE " +  data->tablename;
    SQLCommand += " SET ADJCLOSE = CLOSE WHERE ";
    SQLCommand += " (SELECT  MAX (high)/MIN(low) FROM " + adjustedviewname + ") > 5000;";
    SQLCommand.append ('\n'); 
    
    // remove symbol from symbols table
    SQLCommand += "delete from SYMBOLS where KEY=\""+ adjustedviewname +"\";";
    SQLCommand.append ('\n');
    
    // insert adjusted symbol into symbols table
    SQLCommand += "insert into SYMBOLS values ('" +
                data->symbol + "','" +
                data->name + "','" +
                data->market + "','" +
                data->source + "','" +
                data->timeframe + "','" +
                " " + "','" +
                " " + "','" +
                adjustedviewname + "','" +
                data->currency + "',0,'" +
                data->dnlstring + "',";
    SQLCommand += "'YES', ";         
    SQLCommand += "'" + data->tablename + "');";            
    SQLCommand.append ('\n'); 
    
    // update DATEFROM, DATETO and TFRESOLUTION of adjusted view entry
    SQLCommand += "update SYMBOLS set DATEFROM = (select min(DATE) from " +
              adjustedviewname + ") where KEY=\""+ adjustedviewname + "\";";
    SQLCommand.append ('\n');

    SQLCommand += "update SYMBOLS set DATETO = (select max(DATE) from " +
                adjustedviewname + ") where KEY=\""+ adjustedviewname + "\";";
    SQLCommand.append ('\n');

    SQLCommand += "update SYMBOLS set TFRESOLUTION = (select MINUTE_RESOLUTION ";
    SQLCommand += "from TIMEFRAMES where SYMBOLS.TIMEFRAME = TIMEFRAMES.TIMEFRAME);";
    SQLCommand.append ('\n');
    
    // create week, month, year views for adjusted data
    SQLCommand += tfview (adjustedviewname, "WEEK", data, operation, data->adjust);
    SQLCommand += tfview (adjustedviewname, "MONTH", data, operation, data->adjust);
    SQLCommand += tfview (adjustedviewname, "YEAR", data, operation, data->adjust);
    SQLCommand.append ('\n');
  } 
  
  // insert or updata statistics (basedata table)
  SQLCommand += "delete from basedata where base = '" + data->tablename + "';";
  SQLCommand.append ('\n');
  SQLCommand += "insert into basedata (base, bv, mc, ebitda, pe, peg, dy, epscurrent, epsnext, es, ps, pbv) ";
  SQLCommand += "values ('" + data->tablename + "','" + data->BookValue + "','"; 
  SQLCommand += data->MarketCap + "','" + data->EBITDA + "','" + data->PE +  "','";
  SQLCommand += data->PEG + "','" + data->Yield + "','" + data->EPScy +  "','";
  SQLCommand += data->EPSny +  "','" + data->ES +  "','";
  SQLCommand += data->PS +  "','" + data->PBv + "');";
  SQLCommand.append ('\n');              
  
  // commit
  SQLCommand += "commit;";
  SQLCommand.append ('\n');
    
  // execute sql
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    result = CG_ERR_TRANSACTION;
    setGlobalError(result, __FILE__, __LINE__);
    goto csv2sqlite_end;
  }
  
  // just update symbol table's date from and date to columns
  if (operation == "UPDATE")
  {
	// update DATEFROM, DATETO and TFRESOLUTION of adjusted view entry
	SQLCommand = "UPDATE " +  data->tablename;
    SQLCommand += " SET ADJCLOSE = CLOSE WHERE ";
    SQLCommand += " (SELECT  MAX (high)/MIN(low) FROM " + adjustedviewname + ") > 5000;";
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
	
    SQLCommand = "update SYMBOLS set DATEFROM = (select min(DATE) from " +
              adjustedviewname + ") where KEY=\""+ adjustedviewname + "\";";
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);

    SQLCommand = "update SYMBOLS set DATETO = (select max(DATE) from " +
                adjustedviewname + ") where KEY=\""+ adjustedviewname + "\";";
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);

    SQLCommand = "update SYMBOLS set TFRESOLUTION = (select MINUTE_RESOLUTION ";
    SQLCommand += "from TIMEFRAMES where SYMBOLS.TIMEFRAME = TIMEFRAMES.TIMEFRAME);";
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
    
    SQLCommand = tfview (data->tablename, "WEEK", data, operation, data->adjust);
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
    
    SQLCommand = tfview (data->tablename, "MONTH", data, operation, data->adjust);
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
    
    SQLCommand = tfview (data->tablename, "YEAR", data, operation, data->adjust);
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
	
    SQLCommand = tfview (adjustedviewname, "WEEK", data, operation, data->adjust);
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
    
    SQLCommand = tfview (adjustedviewname, "MONTH", data, operation, data->adjust);
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
    
    SQLCommand = tfview (adjustedviewname, "YEAR", data, operation, data->adjust); 
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, NULL, NULL);
  }
  
csv2sqlite_end:
  // remove temporary file
  QFile::remove(tempfilename);
  return result;
}


