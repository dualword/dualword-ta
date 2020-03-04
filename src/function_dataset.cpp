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

#include <stdlib.h>
#include "ta_func.h"
#include "function_dataset.h"

// dummy: returns input 
DataSet 
DUMMY (DataSet dset, int period)
{
  PriceVector *result = NULL;	
  qint32 setsize;
  
  if (period > 0)
    period = 0;
      
  result = new PriceVector;
  setsize = dset->size ();
  
  for (qint32 counter = 0; counter < setsize; counter ++)
    result->append ((qreal) dset->at (counter));
  return result;
}

// simple moving average 
DataSet 
SMA (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_MA(0, setsize - 1, input, period, TA_MAType_SMA, 
                    &outBegIdx, &outNbElement, output);
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  
  free ((void *) input);
  free ((void *) output);                     
  return result;    	
}

// exponential moving average 
DataSet 
EMA (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_MA(0, setsize - 1, input, period, TA_MAType_EMA, 
                    &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  
  free ((void *) input);
  free ((void *) output);                     
  return result;    	
}

// parabolic SAR
DataSet 
PSAR (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high, *low;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    return NULL;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
  {
	free ((void *) high);    
    return NULL;
  }  
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) low);    
	free ((void *) high);  
    return NULL;  
  }  

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
  }  

  retcode = TA_S_SAR (0, setsize - 1, high, low, 0.1, 0.1, 
                    &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  
  free ((void *) high);
  free ((void *) low);
  free ((void *) output);                     
  return result;    	
}

// relative strength index
DataSet 
RSI (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_RSI (0, setsize - 1, input, period, 
                    &outBegIdx, &outNbElement, output);
  result = new PriceVector;
  
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  
  free ((void *) input);
  free ((void *) output);                     
  return result;    	
}

// money flow index
DataSet 
MFI (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL, *volume = NULL;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto MFI_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto MFI_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto MFI_end; 
  
  volume = (float *) calloc (setsize + 1, sizeof (float));
  if (volume == NULL)
    goto MFI_end;   
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
    goto MFI_end; 

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
    volume[setsize - counter] =  frame.Volume;
  }  

  retcode = TA_S_MFI (0, setsize - 1, high, low, close, volume, 
                    period, &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
    
MFI_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
  
  if (volume != NULL)  
    free ((void *) volume);
      
  if (output != NULL)
    free ((void *) output);                     
    
  return result;    	
}

// rate of change
DataSet 
ROC (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_ROC (0, setsize - 1, input, period, 
                    &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
  {
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  }    
  
  free ((void *) input);
  free ((void *) output);                     
  return result;    	
}

// Williams %R
DataSet 
WILLR (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto WILLR_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto WILLR_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto WILLR_end; 
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
    goto WILLR_end; 

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_WILLR (0, setsize - 1, high, low, close, 
                    period, &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      if ((qreal) output[counter] > 0 || (qreal) output[counter] < -100)
        result->append (0);
      else  
        result->append ((qreal) output[counter]);
    
WILLR_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (output != NULL)
    free ((void *) output);                     
    
  return result;    	
}

// slow stochastic %K
DataSet 
STOCHSLOWK (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *outputK = NULL, *outputD = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto STOCHSLOWK_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto STOCHSLOWK_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto STOCHSLOWK_end; 
  
  outputK = (double *) calloc (setsize + 1, sizeof (double));
  if (outputK == NULL)
    goto STOCHSLOWK_end; 
  
  outputD = (double *) calloc (setsize + 1, sizeof (double));
  if (outputD == NULL)
    goto STOCHSLOWK_end;   

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_STOCH (0, setsize - 1, high, low, close, 
                    period, 3, TA_MAType_SMA, 3, TA_MAType_SMA,
                    &outBegIdx, &outNbElement, outputK, outputD);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
    {
      if (qAbs ((qreal) outputK[counter]) > 10000)
        result->append (0);
      else  
        result->append ((qreal) outputK[counter]);
    }
STOCHSLOWK_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (outputK != NULL)
    free ((void *) outputK);       
  
  if (outputD != NULL)
    free ((void *) outputD);                
    
  return result;    	
}

// slow stochastic %D
DataSet 
STOCHSLOWD (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *outputK = NULL, *outputD = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto STOCHSLOWD_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto STOCHSLOWD_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto STOCHSLOWD_end; 
  
  outputK = (double *) calloc (setsize + 1, sizeof (double));
  if (outputK == NULL)
    goto STOCHSLOWD_end; 
  
  outputD = (double *) calloc (setsize + 1, sizeof (double));
  if (outputD == NULL)
    goto STOCHSLOWD_end;   

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_STOCH (0, setsize - 1, high, low, close, 
                    period, 3, TA_MAType_SMA, 3, TA_MAType_SMA,
                    &outBegIdx, &outNbElement, outputK, outputD);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) outputD[counter]);
    
STOCHSLOWD_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (outputK != NULL)
    free ((void *) outputK);       
  
  if (outputD != NULL)
    free ((void *) outputD);                
    
  return result;    	
}

// fast stochastic %K
DataSet 
STOCHFASTK (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *outputK = NULL, *outputD = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto STOCHFASTK_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto STOCHFASTK_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto STOCHFASTK_end; 
  
  outputK = (double *) calloc (setsize + 1, sizeof (double));
  if (outputK == NULL)
    goto STOCHFASTK_end; 
  
  outputD = (double *) calloc (setsize + 1, sizeof (double));
  if (outputD == NULL)
    goto STOCHFASTK_end;   

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_STOCHF (0, setsize - 1, high, low, close, 
                    period, 3, TA_MAType_SMA, 
                    &outBegIdx, &outNbElement, outputK, outputD);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) outputK[counter]);
    
STOCHFASTK_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (outputK != NULL)
    free ((void *) outputK);       
  
  if (outputD != NULL)
    free ((void *) outputD);                
    
  return result;    	
}

// fast stochastic %D
DataSet 
STOCHFASTD (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *outputK = NULL, *outputD = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto STOCHFASTD_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto STOCHFASTD_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto STOCHFASTD_end; 
  
  outputK = (double *) calloc (setsize + 1, sizeof (double));
  if (outputK == NULL)
    goto STOCHFASTD_end; 
  
  outputD = (double *) calloc (setsize + 1, sizeof (double));
  if (outputD == NULL)
    goto STOCHFASTD_end;   

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_STOCHF (0, setsize - 1, high, low, close, 
                    period, 3, TA_MAType_SMA, 
                    &outBegIdx, &outNbElement, outputK, outputD);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) outputD[counter]);
    
STOCHFASTD_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (outputK != NULL)
    free ((void *) outputK);       
  
  if (outputD != NULL)
    free ((void *) outputD);                
    
  return result;    	
}

// moving average convergence/divergence 
DataSet 
MACD (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *signal = NULL, *hist = NULL, *macd = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  signal = (double *) calloc (setsize + 1, sizeof (double));
  if (signal == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  hist = (double *) calloc (setsize + 1, sizeof (double));
  if (hist == NULL)
  {
	free ((void *) input);  
	free ((void *) signal);  
    return NULL;  
  }
  
  macd = (double *) calloc (setsize + 1, sizeof (double));
  if (macd == NULL)
  {
	free ((void *) input);  
	free ((void *) signal);  
	free ((void *) hist);  
    return NULL;  
  }
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_MACDFIX (0, setsize - 1, input, period, 
                    &outBegIdx, &outNbElement, macd, signal, hist);
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
  {
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      if (qAbs ((qreal) macd[counter]) > 100000)
        result->append (0);
      else  
        result->append ((qreal) macd[counter]);
  }    
  
  free ((void *) input);
  free ((void *) signal);    
  free ((void *) hist);    
  free ((void *) macd);                      
  return result;    	
}

// moving average convergence/divergence signal
DataSet 
MACDSIGNAL (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *signal = NULL, *hist = NULL, *macd = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  signal = (double *) calloc (setsize + 1, sizeof (double));
  if (signal == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  hist = (double *) calloc (setsize + 1, sizeof (double));
  if (hist == NULL)
  {
	free ((void *) input);  
	free ((void *) signal);  
    return NULL;  
  }
  
  macd = (double *) calloc (setsize + 1, sizeof (double));
  if (macd == NULL)
  {
	free ((void *) input);  
	free ((void *) signal);  
	free ((void *) hist);  
    return NULL;  
  }
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_MACDFIX (0, setsize - 1, input, period, 
                    &outBegIdx, &outNbElement, macd, signal, hist);
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) signal[counter]);
  
  free ((void *) input);
  free ((void *) signal);    
  free ((void *) hist);    
  free ((void *) macd);                      
  return result;    	
}

// bollinger bands upper
DataSet 
BBANDSUPPER (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *upper = NULL, *middle = NULL, *lower = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  upper = (double *) calloc (setsize + 1, sizeof (double));
  if (upper == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  middle = (double *) calloc (setsize + 1, sizeof (double));
  if (middle == NULL)
  {
	free ((void *) input);  
	free ((void *) upper);  
    return NULL;  
  }
  
  lower = (double *) calloc (setsize + 1, sizeof (double));
  if (lower == NULL)
  {
	free ((void *) input);  
	free ((void *) upper);  
	free ((void *) middle);  
    return NULL;  
  }
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_BBANDS (0, setsize - 1, input, period, 2.0, 2.0, 
            TA_MAType_SMA, &outBegIdx, &outNbElement, upper, middle, lower);
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) upper[counter]);
  
  free ((void *) input);
  free ((void *) upper);    
  free ((void *) middle);    
  free ((void *) lower);                      
  return result;    	
}

// bollinger bands middle
DataSet 
BBANDSMIDDLE (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *upper = NULL, *middle = NULL, *lower = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  upper = (double *) calloc (setsize + 1, sizeof (double));
  if (upper == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  middle = (double *) calloc (setsize + 1, sizeof (double));
  if (middle == NULL)
  {
	free ((void *) input);  
	free ((void *) upper);  
    return NULL;  
  }
  
  lower = (double *) calloc (setsize + 1, sizeof (double));
  if (lower == NULL)
  {
	free ((void *) input);  
	free ((void *) upper);  
	free ((void *) middle);  
    return NULL;  
  }
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_BBANDS (0, setsize - 1, input, period, 2.0, 2.0, 
            TA_MAType_SMA, &outBegIdx, &outNbElement, upper, middle, lower);
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) middle[counter]);
  
  free ((void *) input);
  free ((void *) upper);    
  free ((void *) middle);    
  free ((void *) lower);                      
  return result;    	
}

// bollinger bands lower
DataSet 
BBANDSLOWER (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *upper = NULL, *middle = NULL, *lower = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  upper = (double *) calloc (setsize + 1, sizeof (double));
  if (upper == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  middle = (double *) calloc (setsize + 1, sizeof (double));
  if (middle == NULL)
  {
	free ((void *) input);  
	free ((void *) upper);  
    return NULL;  
  }
  
  lower = (double *) calloc (setsize + 1, sizeof (double));
  if (lower == NULL)
  {
	free ((void *) input);  
	free ((void *) upper);  
	free ((void *) middle);  
    return NULL;  
  }
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_BBANDS (0, setsize - 1, input, period, 2.0, 2.0, 
            TA_MAType_SMA, &outBegIdx, &outNbElement, upper, middle, lower);
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) lower[counter]);
  
  free ((void *) input);
  free ((void *) upper);    
  free ((void *) middle);    
  free ((void *) lower);                      
  return result;    	
}

// average directional movement index
DataSet 
ADX (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto ADX_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto ADX_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto ADX_end; 
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
    goto ADX_end; 

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_ADX (0, setsize - 1, high, low, close, 
                    period, &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
    
ADX_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (output != NULL)
    free ((void *) output);                     
    
  return result;    	
}

// aroon up
DataSet 
AROONUP (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high, *low;
  double *up = NULL, *down = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    return NULL;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
  {
	free ((void *) high);    
    return NULL;
  }  
  
  up = (double *) calloc (setsize + 1, sizeof (double));
  if (up == NULL)
  {
	free ((void *) low);    
	free ((void *) high);  
    return NULL;  
  }  
  
  down = (double *) calloc (setsize + 1, sizeof (double));
  if (down == NULL)
  {
	free ((void *) up);  
	free ((void *) low);    
	free ((void *) high);  
    return NULL;  
  }

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
  }  

  retcode = TA_S_AROON (0, setsize - 1, high, low, period, 
                    &outBegIdx, &outNbElement, down, up);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) up[counter]);
  
  free ((void *) high);
  free ((void *) low);
  free ((void *) up);                     
  free ((void *) down);                     
  return result;    	
}

// aroon down
DataSet 
AROONDOWN (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high, *low;
  double *up = NULL, *down = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    return NULL;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
  {
	free ((void *) high);    
    return NULL;
  }  
  
  up = (double *) calloc (setsize + 1, sizeof (double));
  if (up == NULL)
  {
	free ((void *) low);    
	free ((void *) high);  
    return NULL;  
  }  
  
  down = (double *) calloc (setsize + 1, sizeof (double));
  if (down == NULL)
  {
	free ((void *) up);  
	free ((void *) low);    
	free ((void *) high);  
    return NULL;  
  }

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
  }  

  retcode = TA_S_AROON (0, setsize - 1, high, low, period, 
                    &outBegIdx, &outNbElement, down, up);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) down[counter]);
  
  free ((void *) high);
  free ((void *) low);
  free ((void *) up);                     
  free ((void *) down);                     
  return result;    	
}

// commodity channel index
DataSet 
CCI (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto CCI_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto CCI_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto CCI_end; 
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
    goto CCI_end; 

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_CCI (0, setsize - 1, high, low, close, 
                    period, &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
    
CCI_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (output != NULL)
    free ((void *) output);                     
    
  return result;    	
}

// standard deviation
DataSet 
STDDEV (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_STDDEV (0, setsize - 1, input, period, period,
                    &outBegIdx, &outNbElement, output);
  result = new PriceVector;
  
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  
  free ((void *) input);
  free ((void *) output);                     
  return result;    	
}

// momentum
DataSet 
MOMENTUM (DataSet dset, int period)
{
  PriceVector *result = NULL;
  float *input;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;

  if (period < 2)
    period = 2;
  
  setsize = dset->size ();
  input = (float *) calloc (setsize + 1, sizeof (float));
  if (input == NULL)
    return NULL;
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
  {
	free ((void *) input);  
    return NULL;  
  }  
  
  for (qint32 counter = setsize; counter > 0; counter --)
    input[setsize - counter] = dset->at (counter - 1);
  
  retcode = TA_S_MOM (0, setsize - 1, input, period,
                    &outBegIdx, &outNbElement, output);
  result = new PriceVector;
  
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
  
  free ((void *) input);
  free ((void *) output);                     
  return result;    	
}

// directional movement index
DataSet 
DMI (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto DMX_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto DMX_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto DMX_end; 
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
    goto DMX_end; 

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_DX (0, setsize - 1, high, low, close, 
                    period, &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
    
DMX_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (output != NULL)
    free ((void *) output);                     
    
  return result;    	
}

// average true range
DataSet 
ATR (FrameVector *HLOC, int period)
{
  PriceVector *result = NULL;
  QTAChartFrame frame;
  float *high = NULL, *low = NULL, *close = NULL;
  double *output = NULL;
  TA_RetCode retcode;
  int outBegIdx, outNbElement;
  qint32 setsize;
  
  if (period < 2)
    period = 2;
  
  setsize = HLOC->size ();
  high = (float *) calloc (setsize + 1, sizeof (float));
  if (high == NULL)
    goto DMX_end;
  
  low = (float *) calloc (setsize + 1, sizeof (float));
  if (low == NULL)
    goto DMX_end; 
  
  close = (float *) calloc (setsize + 1, sizeof (float));
  if (close == NULL)
    goto DMX_end; 
  
  output = (double *) calloc (setsize + 1, sizeof (double));
  if (output == NULL)
    goto DMX_end; 

  for (qint32 counter = setsize; counter > 0; counter --)
  {
	frame = HLOC->at (counter - 1);
    high[setsize - counter] = frame.High;
    low[setsize - counter] =  frame.Low;
    close[setsize - counter] =  frame.Close;
  }  

  retcode = TA_S_ATR (0, setsize - 1, high, low, close, 
                    period, &outBegIdx, &outNbElement, output);
  
  result = new PriceVector;
  if (retcode == TA_SUCCESS)
    for (qint32 counter = outNbElement - 1; counter >=0; counter --)
      result->append ((qreal) output[counter]);
    
DMX_end:
  if (high != NULL)  
    free ((void *) high);
  
  if (low != NULL)  
    free ((void *) low);
  
  if (close != NULL)  
    free ((void *) close);
      
  if (output != NULL)
    free ((void *) output);                     
    
  return result;    	
}
