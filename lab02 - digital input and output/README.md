# Lab 2

## Introduction 

## Equipment List

- B-L4S5I-IOT01A * 1
- USB Type-A male to USB Micro-B male * 1 (compatible with B-L4S5I-IOT01A)
- Breadboard * 1 (with at least 64 rows of pins)
- Seven segment display * 1
- Wire * 20
- 220 Ohm Resistor * 2

## Lab Description

### Blinky LED ane DigitalOut

``` cpp=
#include "mbed.h"

DigitalOut myled(LED1);

int main()
{
   // check that myled object is initialized and connected to a pin
   if (myled.is_connected()) {
      printf("myled is initialized and connected!\n\r");
   }

   // Blink LED
   while (1) {
      myled = 1;          // set LED1 pin to high
      printf("myled = %d \n\r", (uint8_t)myled);
      ThisThread::sleep_for(500ms);

      myled.write(0);     // set LED1 pin to low
      printf("myled = %d \n\r", myled.read());
      ThisThread::sleep_for(500ms);
   }
}
```

- DigitalOut is a class which is used for setting the state of a pin like LED1 and so on. Take a look at the specific pin that we can utilize on the B_L4S5I_IOT01A.
- **`DigitalOut myled(LED1);`**

| Function | Usage |
| -------- | ----- |
| DigitalOut | Create a DigitalOut connected to the specified pin |
| `.write()` | Set the output, specified as 0 or 1 (int) |
| `.read()` | Return the output setting, represented as 0 or 1 (int) |
| (operator) = | A shorthand for write (overload operator) |
| (operator) int() | A shorthand for read (overload operator) |
| `.is_connected()` | Return non-zero if connected to a GPIO pin |
