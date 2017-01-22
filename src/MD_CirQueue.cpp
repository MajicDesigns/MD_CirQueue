/*
MD_CirQueue - Library to provide a FIFO queue implemented as a Ring Buffer
  
See header file for comments

This file contains class related methods.
  
Copyright (C) 2017 Marco Colli. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <Arduino.h>
#include <MD_CirQueue.h>

/**
 * \file
 * \brief Implements class definition and general methods
 */

#define DEBUG 0

#if DEBUG
#define PRINTS(s)   { Serial.print(F(s)); }
#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }
#else
#define PRINTS(s)
#define PRINT(s, v)
#endif

MD_CirQueue::MD_CirQueue(uint8_t itmQty, uint16_t itmSize) :
_itmQty(itmQty), _itmSize(itmSize), _itmCount(0), _overwrite(false)
{
  uint16_t size = sizeof(uint8_t) * _itmQty * _itmSize;

  PRINT("\nAllocating ", size);
  PRINTS("bytes");
  _itmData = (uint8_t *)malloc(size); 
  clear();
}

MD_CirQueue::~MD_CirQueue()
{
  free(_itmData);
}

void MD_CirQueue::clear()
{
  _head = _tail = _itmCount = 0;
}

bool MD_CirQueue::push(uint8_t *itm)
{
  if (isFull())
  {
    if (_overwrite)
    {
      PRINTS("\nOverwriting Q");
      pop(_itmData + (_tail*_itmSize));	// pop into itself ...
    }
    else
      return(false);
  }
  
  // Save item and adjust the tail pointer
  PRINT("\nPush @", _head);
  memcpy(_itmData+(_itmSize*_head), itm, _itmSize);
  _head++;
  _itmCount++;
  if (_head == _itmQty) _head = 0;
  
  return(true);
}

uint8_t *MD_CirQueue::pop(uint8_t *itm)
{
    if (isEmpty()) return(NULL); 

    // Copy data from the buffer
    PRINT("\nPop @", _tail);
    memcpy(itm, _itmData+(_itmSize*_tail), _itmSize);
	_tail++;
  _itmCount--;

    // If head has reached last item, wrap it back around to the start
    if (_tail == _itmQty) _tail = 0;
    
	return (itm);
}
