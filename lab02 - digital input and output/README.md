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

### Seven-Segment Display

![7-segment display](https://github.com/vic9112/Embedded_System_Lab/assets/137171415/571961d8-e42b-46a4-99a9-3f641227d4e6)

``` cpp=
#include "mbed.h"

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(){
  while(1){
    for (int i = 0; i<10; i = i+1){
      display = table[i];
      ThisThread::sleep_for(1s);
    }
  }
}
```

- The type **`BusOut`** is an object used for setting the state of a collection of pins.
``` cpp=
`BusOut(PinName0, PinName1, ...);`
```

### Switch Button

``` cpp=
#include "mbed.h"

DigitalIn mypin(BUTTON1);
DigitalOut myled(LED1);

int main()
{
   // check mypin object is initialized and connected to a pin
   if (mypin.is_connected())
   {
      printf("mypin is connected and initialized! \n\r");
   }

   // Optional: set mode as PullUp/PullDown/PullNone/OpenDrain
   mypin.mode(PullNone);

   // press the button and see the console / led change
   while (1)
   {
      printf("mypin has value : %d \n\r", mypin.read());
      myled = mypin; // toggle led based on value of button
      ThisThread::sleep_for(250ms);
   }
}
```

- **`DigitalIn`** is a class which used for reading the state of a pin. **BUTTON1** is wired to ground, we will het "0" when the button is pushed.
``` cpp=
DigitalIn mypin(BUTTON1);
```
