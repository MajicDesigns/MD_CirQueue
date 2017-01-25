# MD_CirQueue

This library implements a FIFO queue for generalised items, implemented as a circular buffer. The number and size of the items that are enqueued is defined in the constructor, after which the calling program can push and pop items in FIFO order from the queue. When the queue is full the library accomodates both overwiting the oldest item in the queue or failing the current push() attempt.

This mechanism is useful for holding data that needs to be asynchronously transferred between different parts of an application (eg. multiple data streams queued up for one 'consumer' task).