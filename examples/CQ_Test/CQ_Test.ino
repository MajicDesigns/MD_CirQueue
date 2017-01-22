#include <MD_CirQueue.h>

const uint8_t	QUEUE_SIZE = 6;

// Define a queue that 4 unsif=gned 32 bit values.
MD_CirQueue Q(QUEUE_SIZE, sizeof(uint32_t));

// Display full/empty status of ring buffer.
void showStatus()
{
  if (Q.isEmpty()) Serial.print("\n->Q EMPTY");
  if (Q.isFull()) Serial.print("\n->Q FULL");
  
}

void setup() 
{
  Serial.begin(57600);
  Serial.print("\n[CQ_Test]");

  Q.begin();
  //Q.setFullOverwrite(true);
  
  // Show status of (empty) ring buffer.
  showStatus();
  
  // Try pushing 1 more item than can be queued.
  for (uint32_t i = 0; i < QUEUE_SIZE+3; i++) 
  {
    bool b;

    b = Q.push((uint8_t *)&i);
    Serial.print("\nPush ");
    Serial.print(i);
    Serial.print(b ? " ok" : " fail");
    showStatus();
  }
  
  // Start popping bytes until buffer is empty again.
  while (!Q.isEmpty()) 
  {
	  uint32_t	n;
	  Q.pop((uint8_t *)&n);
    Serial.print("\nPopped ");
    Serial.print(n);
    showStatus();
  }
}

void loop()
{
}
