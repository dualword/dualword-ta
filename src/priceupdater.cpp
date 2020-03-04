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

#include <QGraphicsTextItem>
#include "defs.h"
#include "priceupdater.h"
#include "mainwindow.h"

/// PriceWorker
// constructor
PriceWorker::PriceWorker (QString symbol, QString feed)
{
  parentObject = NULL;
  state = 0;	
  runflag = 1;
  if (feed == "YAHOO")
    yfeed = new YahooFeed (this);
  else 
  if (feed == "GOOGLE")
    gfeed = new GoogleFeed (this);    
  else
    runflag = 0;
  datafeed = feed;  
  this->symbol = symbol;
  
}

// destructor
PriceWorker::~PriceWorker ()
{
  runflag = 0;
}

// process slot
void 
PriceWorker::process()
{
   CG_ERR_RESULT result = CG_ERR_OK;	
   qint32 counter = 0;
   
   state = 1;	
   while (runflag.fetchAndAddAcquire (0) == 1)
   {
	 if (counter == 0 && runflag.fetchAndAddAcquire (0))  
	 { 
	   if (datafeed.toUpper () == "YAHOO" && runflag.fetchAndAddAcquire (0))
	     result = yfeed->getRealTimePrice (symbol, rtprice, YahooFeed::HTTP);
	     
	   if (datafeed.toUpper () == "GOOGLE" && runflag.fetchAndAddAcquire (0))
	     result = gfeed->getRealTimePrice (symbol, rtprice);
	   
	   if (result == CG_ERR_OK && runflag.fetchAndAddAcquire (0))
	     parentObject->emitUpdateOnlinePrice (rtprice);
	 } 
	 
	 if (runflag.fetchAndAddAcquire (0) == 1)
	 {
	   Sleeper::msleep(100);
	   counter += 100;
	   if (counter >= (Application_Settings->options.nettimeout * 1100))
	     counter = 0; 
	 }    
   }	
   state = 0;
}

// terminate slot
void 
PriceWorker::terminate ()
{
  if (state.fetchAndAddAcquire (0) == 0)
    return;
    	
  runflag = 0;		
}

/// PriceUpdater
// constructor
PriceUpdater::PriceUpdater (QString symbol, QString feed, QTACObject *parent)
{
  if (parent != NULL)
    setParent (parent);
  	
  worker = new PriceWorker (symbol, feed);
  if (parent != NULL)
  {
    worker->setParentObject (parent);
    parent->onlineprice = true;
  }  
  worker->moveToThread(&thread);
  connect(&thread, SIGNAL(started()), worker, SLOT(process()));
  thread.start();
  thread.setPriority (QThread::LowestPriority);
}

// destructor
PriceUpdater::~PriceUpdater ()
{
  if (worker != NULL)	
  {
    worker->terminate ();
    thread.quit ();
    thread.wait ();
    delete worker;
  }  
}
