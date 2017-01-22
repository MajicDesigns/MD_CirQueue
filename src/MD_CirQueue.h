#pragma once
/**
\mainpage Circular Queue Library
This library implements a FIFO queue for generalised items, implemented as a 
circular queue. The number and size of the items that are enqueued is defined 
in the constructor, after which the calling program can push and pop items 
in FIFO order from the queue. When the queue is full the library accomodates 
both overwiting the oldest item in the queue or failing the current push() 
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
Jan 2017 version 1.0
- First implementation
*/

#include <Arduino.h>

 /**
 * \file
 * \brief Main header file for the MD_CirQueue library
 */

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
   * configure the quanity and size of queue objects.
   * 
   * \param itmQty		number of items allowed in the queue.
   * \param itmSize		size of each item in bytes.
   */
  MD_CirQueue(uint8_t itmQty, uint16_t itmSize);
        
  /** 
   * Class Destructor.
   *
   * Released allocated memory and does the necessary to clean up once the queue is
   * no longer required.
   */
  ~MD_CirQueue();
        
  /** 
   * Initialize the object.
   *
   * Initialise the object data. This needs to be called during setup() to initialise new 
   * data for the class that cannot be done during the object creation.
   */
   void begin(void) {};
   
   /**
	* Clear contents of buffer
	*
	* Clears the buffer by resetting the head and tail pointers. Does not zero out delete 
	* data in the buffer.
	*/
	void clear();
        
  /**
	* Push an item into the queue
	*
	* Place the item passed into the end of the queue. The item will be returned
	* to the calling proogram, in FIFO order, using pop().
	* If the buffer is already full before the push(), the behavior will depend on the 
	* the setting controlled by the setFullOverwrite() method.
	*
	* @param itm	a pointer to data buffer of the item to be saved. Data size must be size specified in the constructor.
	* @return true if the item was successfully placed in the queue, false otherwise
	*/
	bool push(uint8_t *itm);
        
  /**
	* Pop an item from the queue
	*
	* Return the first available item in the queue, copied into the buffer specified,
	* returning a pointer to the copied item. If no items are available (queue is
	* empty), then no data is copied and the method returns a NULL pointer.
	*
	* @param itm	a pointer to data buffer for the retrieved item to be saved. Data size must be size specified in the constructor.
	* @return pointer to the memory buffer or NULL if the queue is empty
	*/
	uint8_t *pop(uint8_t *itm);
	
  /**
    * Set queue full behaviour
	*
	* If the setting is set true, then push() with a full queue will overwrite the
	* oldest item in the queue. Default behaviour is not to overwrite the oldest item
	* and fail the push() attempt.
	*
	* @param b	true to ovewrite oldest item, false (default) to fail the push() call
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
    inline bool isFull() { return (_itmCount == _itmQty); };
	
  private:
    uint8_t	  _itmQty;	/// number of itenms in the queue
		uint16_t  _itmSize;	/// size in bytes for each item
    uint8_t	  *_itmData;/// pointer to allocated memory buffer
		
		uint8_t _itmCount;	/// number of items in the queue
    uint8_t _head;		  /// array index where the next push will occur
    uint8_t _tail;      /// array index where next pop will occur
		bool    _overwrite;	/// when true, overwrite oldest object if push() and isFull()
};
