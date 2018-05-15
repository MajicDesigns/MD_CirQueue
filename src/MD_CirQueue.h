#pragma once
/**
\mainpage Circular Queue Library
This library implements a FIFO queue for generalized items, implemented as a
circular queue. The number and size of the items that are enqueued is defined
in the constructor, after which the calling program can push and pop items
in FIFO order from the queue. When the queue is full the library accommodates
both overwriting the oldest item in the queue or failing the current push()
attempt.

This mechanism is useful for holding data that needs to be asynchronously
transferred between different parts of an application (eg. multiple data
streams queued up for one 'consumer' task).

- \subpage pageRevisionHistory
- \subpage pageCopyright

\page pageCopyright Copyright
Copyright
---------
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

\page pageRevisionHistory Revision History
Revision History
----------------
May 2018 version 1.0.2
- Maintenance release, no major changes

Jan 2017 version 1.0
- First implementation
*/
#pragma once

#include <Arduino.h>

/**
 * \file
 * \brief Main header file and class definition for the MD_CirQueue library
 */

#define CQ_DEBUG 0

#if CQ_DEBUG
#define CQ_PRINTS(s)   { Serial.print(F(s)); }
#define CQ_PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }
#else
#define CQ_PRINTS(s)
#define CQ_PRINT(s, v)
#endif

/**
 * Core object for the MD_CirQueue library
 */
class MD_CirQueue
{
public:
  /**
   * Class Constructor.
   *
   * Instantiate a new instance of the class. The parameters passed are used to
   * configure the quantity and size of queue objects.
   *
   * \param itmQty    number of items allowed in the queue.
   * \param itmSize   size of each item in bytes.
   */
  MD_CirQueue(uint8_t itmQty, uint16_t itmSize) :
    _itmQty(itmQty), _itmSize(itmSize),
    _itmCount(0), _overwrite(false)
  {
    uint16_t size = sizeof(uint8_t) * _itmQty * _itmSize;

    CQ_PRINT("\nAllocating ", size);
    CQ_PRINTS("bytes");
    _itmData = (uint8_t *)malloc(size);
    clear();
  }

  /**
   * Class Destructor.
   *
   * Released allocated memory and does the necessary to clean up once the queue is
   * no longer required.
   */
  ~MD_CirQueue()
  {
    free(_itmData);
  }

  /**
   * Initialize the object.
   *
   * Initialize the object data. This needs to be called during setup() to initialize new
   * data for the class that cannot be done during the object creation.
   */
   void begin(void) {};

 /**
  * Clear contents of buffer
  *
  * Clears the buffer by resetting the head and tail pointers. Does not zero out delete
  * data in the buffer.
  */
   inline void clear() { _idxPut = _idxTake = _itmCount = 0; };

 /**
  * Push an item into the queue
  *
  * Place the item passed into the end of the queue. The item will be returned
  * to the calling program, in FIFO order, using pop().
  * If the buffer is already full before the push(), the behavior will depend on the
  * the setting controlled by the setFullOverwrite() method.
  *
  * @param itm    a pointer to data buffer of the item to be saved. Data size must be size specified in the constructor.
  * @return true  if the item was successfully placed in the queue, false otherwise
  */
  bool push(uint8_t* itm)
    {
  if (isFull())
  {
    if (_overwrite)
    {
    CQ_PRINTS("\nOverwriting Q");
    pop(_itmData + (_idxTake * _itmSize));  // pop it into itself ...
    }
    else
      return(false);
    }

    // Save item and adjust the tail pointer
    CQ_PRINT("\nPush @", _idxPut);
    memcpy(_itmData + (_itmSize * _idxPut), itm, _itmSize);
    _idxPut++;
    _itmCount++;
    if (_idxPut == _itmQty) _idxPut = 0;

    return(true);
  }

 /**
  * Pop an item from the queue
  *
  * Return the first available item in the queue, copied into the buffer specified,
  * returning a pointer to the copied item. If no items are available (queue is
  * empty), then no data is copied and the method returns a NULL pointer.
  *
  * @param itm  a pointer to data buffer for the retrieved item to be saved. Data size must be size specified in the constructor.
  * @return pointer to the memory buffer or NULL if the queue is empty
  */
  uint8_t *pop(uint8_t* itm)
    {
    if (isEmpty()) return(NULL);

    // Copy data from the buffer
    CQ_PRINT("\nPop @", _idxTake);
    memcpy(itm, _itmData + (_itmSize * _idxTake), _itmSize);
    _idxTake++;
    _itmCount--;

    // If head has reached last item, wrap it back around to the start
    if (_idxTake == _itmQty) _idxTake = 0;

    return (itm);
  }

 /**
   * Peek at the next item in the queue
   *
   * Return a copy of the first item in the queue, copied into the buffer specified,
   * returning a pointer to the copied item. If no items are available (queue is
   * empty), then no data is copied and the method returns a NULL pointer. This does
   * not remove the item in the queue but gives a 'llok-ahead' capability in
   * processing the queue.
   *
   * @param itm a pointer to data buffer for the copied item to be saved. Data size must be size specified in the constructor.
   * @return pointer to the memory buffer or NULL if the queue is empty
   */
   uint8_t *peek(uint8_t* itm)
   {
     if (isEmpty()) return(NULL);

     // Copy data from the buffer
     CQ_PRINT("\nPeek @", _idxTake);
     memcpy(itm, _itmData + (_itmSize * _idxTake), _itmSize);

     return (itm);
   }

 /**
  * Set queue full behavior
  *
  * If the setting is set true, then push() with a full queue will overwrite the
  * oldest item in the queue. Default behavior is not to overwrite the oldest item
  * and fail the push() attempt.
  *
  * @param b  true to overwrite oldest item, false (default) to fail the push() call
  */
  inline void setFullOverwrite(bool b) { _overwrite = b; };

 /**
  * Check if the buffer is empty
  *
  * @return true if empty, false otherwise
  */
  inline bool isEmpty(void) { return(_itmCount == 0); };

 /**
  * Check if the buffer is full
  *
  * @return true if full, false otherwise
  */
  inline bool isFull() { return (_itmCount != 0 && _itmCount == _itmQty); };

private:
  uint8_t   _itmQty;    /// number of items in the queue
  uint16_t  _itmSize;   /// size in bytes for each item
  uint8_t*  _itmData;   /// pointer to allocated memory buffer

  uint8_t   _itmCount;  /// number of items in the queue
  uint8_t   _idxPut;    /// array index where the next push will occur
  uint8_t   _idxTake;   /// array index where next pop will occur
  bool      _overwrite; /// when true, overwrite oldest object if push() and isFull()
};
