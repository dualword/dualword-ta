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
#include <QPushButton>
#include <QDialogButtonBox>
#include <QShowEvent>
#include "appColorDialog.h"

// constructor
appColorDialog::appColorDialog (QWidget * parent)
{
  if (parent != NULL)
    setParent (parent);
  connect (this, SIGNAL (accepted ()), this, SLOT (color_accepted ()));	
}

// destructor
appColorDialog::~appColorDialog ()
{
}

//
QColor	
appColorDialog::appSelectedColor (bool *ok) const
{
  *ok = okflag;
  if (okflag)
    QApplication::setOverrideCursor (QCursor (Qt::PointingHandCursor)); 
  return selectedColor ();	
}	

// color accepted
void 
appColorDialog::color_accepted(void)
{
  okflag = true;	
}

// show event
void 
appColorDialog::showEvent (QShowEvent * event)
{
  if (event->spontaneous ())	
    return;
    	
  okflag = false;	
}
