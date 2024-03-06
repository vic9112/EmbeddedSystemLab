#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_RATE 1000ms

int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);

    for (int i = 0; i < 10; i++)
    {
        led = !led;
        printf("%1.5f\n", 3.14159); // If we didn't override the parameter printf_lib with the value "std", it can't successfully print out
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
