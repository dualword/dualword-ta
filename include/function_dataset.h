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

#ifndef FUNCTION_DATASET_H
#define FUNCTION_DATASET_H

#include <QVector>
#include "qtachart.h"

typedef QVector < qreal >PriceVector;
typedef PriceVector* DataSet;

// dummy: returns input 
DataSet DUMMY (DataSet dset, int period);

// simple moving average 
DataSet SMA (DataSet dset, int period);

// exponential moving average 
DataSet EMA (DataSet dset, int period);

// parabolic SAR
DataSet PSAR (FrameVector *HLOC, int period);

// relative strength index
DataSet RSI (DataSet dset, int period);

// money flow index
DataSet MFI (FrameVector *HLOC, int period);

// rate of change
DataSet ROC (DataSet dset, int period);

// Williams %R
DataSet WILLR (FrameVector *HLOC, int period);

// slow stochastic %K
DataSet STOCHSLOWK (FrameVector *HLOC, int period);

// slow stochastic %D
DataSet STOCHSLOWD (FrameVector *HLOC, int period);

// fast stochastic %K
DataSet STOCHFASTK (FrameVector *HLOC, int period);

// fast stochastic %D
DataSet STOCHFASTD (FrameVector *HLOC, int period);

// moving average convergence/divergence 
DataSet MACD (DataSet dset, int period);

// moving average convergence/divergence signal
DataSet MACDSIGNAL (DataSet dset, int period);

// bollinger bands upper
DataSet BBANDSUPPER (DataSet dset, int period);

// bollinger bands middle
DataSet BBANDSMIDDLE (DataSet dset, int period);

// bollinger bands lower
DataSet BBANDSLOWER (DataSet dset, int period);

// average directional movement index
DataSet ADX (FrameVector *HLOC, int period);

// aroon up
DataSet AROONUP (FrameVector *HLOC, int period);

// aroon down
DataSet AROONDOWN (FrameVector *HLOC, int period);

// commodity channel index
DataSet CCI (FrameVector *HLOC, int period);

// standard deviation
DataSet STDDEV (DataSet dset, int period);

// momentum
DataSet MOMENTUM (DataSet dset, int period);

// directional movement index
DataSet DMI (FrameVector *HLOC, int period);

// average true range
DataSet ATR (FrameVector *HLOC, int period);

#endif // FUNCTION_DATASET_H
