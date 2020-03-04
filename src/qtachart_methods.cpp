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

// member functions of QTAChart except
// constructor, destructors and event handlers

#include "qtachart_core.h"

/// Aa

/// Bb

/// Cc

/// Dd

/// Ee

/// Ff

/// Gg

/// Hh

/// Ii

/// Jj

/// Kk

/// Ll
/*
// populate chart's frame vector
static int
ascend (const QTAChartFrame & v1, const QTAChartFrame & v2)
{
  QString s1, s2;
  s1 = QString::fromLocal8Bit (v1.Date, -1);
  s2 = QString::fromLocal8Bit (v2.Date, -1);

  if (s1 != s2)
    return s1 < s2;

  s1 = QString::fromLocal8Bit (v1.Time, -1);
  s2 = QString::fromLocal8Bit (v2.Time, -1);

  return s1 < s2;
}
*/

static int
descend (const QTAChartFrame & v1, const QTAChartFrame & v2)
{
  QString s1, s2;
  s1 = QString::fromLocal8Bit (v1.Date, -1);
  s2 = QString::fromLocal8Bit (v2.Date, -1);

  if (s1 != s2)
    return s1 > s2;

  s1 = QString::fromLocal8Bit (v1.Time, -1);
  s2 = QString::fromLocal8Bit (v2.Time, -1);

  return s1 > s2;
}

// load the frames
void
QTAChart::loadFrames (FrameVector & frames, QString symbol, QString name)
{
  TFClass TF;	
  QString btext;
  QStringList ymd;
  QTAChartFrame frame, haframe, prevframe;	
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  qint32 nframes;
  bool ok;
  
  TF.HLOC.clear ();
  TF.HEIKINASHI.clear ();
  TF.TFSymbol = symbol;
  TF.TFName = name;
  TF.TFStartBar = 0;
  TF.TFExcess_Drag_Width = 0;
  nframes = frames.size ();
  
  for (qint32 counter = 0; counter < nframes; counter ++)
  {
    frame = frames.at (counter);
 
    ymd = QString::fromLocal8Bit (frame.Date, -1).split("-", QString::KeepEmptyParts);
    frame.year = frame.month = frame.day = 0;
    if (ymd.size () > 2)
    {
      frame.year = ymd[0].toUShort(&ok, 10);
      frame.month = ymd[1].toUShort(&ok, 10);
      frame.day = ymd[2].toUShort(&ok, 10);
    }  
    
    btext =
      "D: " +
      QString::fromLocal8Bit (frame.Date, -1) +
      " " +
      QString::fromLocal8Bit (frame.Time, -1) +
      "  O: " +
      QString::number (frame.Open, 'f', 2) +
      "  C: " +
      QString::number (frame.Close, 'f', 2) +
      "  H: " +
      QString::number (frame.High, 'f', 2) +
      "  L: " +
      QString::number (frame.Low, 'f', 2);
      
    frame.Text = btext;
    frames[counter] = frame;
  }    
  
  core->reloaded = true;
  TF.HLOC = frames;
  qSort(TF.HLOC.begin(), TF.HLOC.end(), descend);
  
  if (nframes < 2)
  {
	core->TIMEFRAME += TF;  
    return;
  }  

  TF.HEIKINASHI.prepend (TF.HLOC[nframes - 1]);
  prevframe = TF.HLOC[nframes - 1];

  for (qint32 counter = nframes - 2; counter > -1; counter--)
  {
    haframe = TF.HLOC[counter];
    frame = TF.HLOC[counter];
    haframe.Close = (frame.Open + frame.High + frame.Close + frame.Low) / 4;
    haframe.Open = (prevframe.Open + prevframe.Close) / 2;
    haframe.High = qMax (qMax(haframe.Close, haframe.Open), frame.High);
    haframe.Low =  qMin (qMin(haframe.Close, haframe.Open), frame.Low);
    TF.HEIKINASHI.prepend (haframe);
    prevframe  = haframe;
  }
  core->TIMEFRAME += TF;  
}

// load data
void 
QTAChart::loadData (QTAChartData data)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);		
  QString textdata = "";
  
  textdata += "Book Value:  " + data.bv + "\n\n";
  textdata += "Market Cap:  " + data.mc + "\n\n";
  textdata += "EBITDA:  " + data.ebitda + "\n\n";
  textdata += "Price/Earnings:  " + data.pe + "\n\n";
  textdata += "PEG Ratio:  " + data.peg + "\n\n";
  textdata += "Dividend Yield:  " + data.dy + "\n\n";
  textdata += "EPS Current:  " + data.epscurrent + "\n\n";
  textdata += "EPS Next:  " + data.epsnext + "\n\n";
  textdata += "Earnings/Share:  " + data.es + "\n\n";
  textdata += "Price/Sales:  " + data.ps + "\n\n";
  textdata += "Price/Book:  " + data.pbv + "\n\n";
  core->dataScr->setData (textdata);
}

/// Mm

/// Nn

/// Oo

/// Pp

/// Qq

/// Rr
// restore bottom text 
void 
QTAChart::restoreBottomText ()
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);	
  core->restoreBottomText ();
}

/// Ss

// always redraw chart on/off
void
QTAChart::setAlwaysRedraw (bool boolean)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  core->always_redraw = boolean;
}

// set chart's symbol
void
QTAChart::setSymbol (QString symbol)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  
  core->Symbol = symbol;	
}

// set chart's symbol key
void
QTAChart::setSymbolKey (QString key)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  
  core->SymbolKey = key;	
  
  // load setting
  core->loadSettings (); 
}

// set symbol's feed
void
QTAChart::setFeed (QString feed)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  
  core->Feed = feed;	
}

// set chart's title
void
QTAChart::setTitle (QString title, QString subtitle)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);
  core->setTitle (title, subtitle);
}

// set the bottom text to custom string
void 
QTAChart::setCustomBottomText (QString string)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);	
  core->setCustomBottomText (string);
}

// set linear chart on/off
void
QTAChart::setLinear (bool boolean)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  core->setLinearScale (boolean);
}

// set chart volumes on/off
void
QTAChart::showVolumes (bool boolean)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  core->show_volumes = boolean;
}

// set chart grid on/off
void
QTAChart::showGrid (bool boolean)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  core->show_grid = boolean;
}

// set online price on/off
void
QTAChart::showOnlinePrice (bool boolean)
{
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> ((void *) chartdata);

  core->show_onlineprice = boolean;
}

/// Tt

/// Uu

/// Vv

/// Ww

/// Xx

/// Wy

/// Zz
