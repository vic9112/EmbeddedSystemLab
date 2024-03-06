// CheckPoint
// The program should monitor the state of the push-button switch. 
// When the switch's state is zero, the 7-segment display should cycle through the digits 0 
// through 9 in ascending order. Conversely, when the switch's state is one, the 7-segment 
// display should cycle through the digits 9 through 0 in descending order.
#include "mbed.h"

// 7-segment display
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
// push button
DigitalIn  mypin(BUTTON1);
DigitalOut myled(LED1);

int main() {
    // check mypin object is initialized and connected to a pin
    if (mypin.is_connected()) {
        printf("mypin is connected and initialized \n\r");
    }

    // Optional: set mode as PullUp/PullDown/PullNone/OpenDrain
    mypin.mode(PullNone);

    while (1) {
        printf("mypin value: %d \n\r", mypin.read());
        myled = mypin; // toggle led based on value of button
        if (mypin.read() == 0) {
            for (int i = 0; i < 10; i += 1) {
                display = table[i];
                ThisThread::sleep_for(1s);
                if (mypin.read() == 1) break;
            }
        } else {
            for (int i = 9; i >= 0; i -= 1) {
                display = table[i];
                ThisThread::sleep_for(1s);
                if (mypin.read() == 0) break;
            }
        }
    }
}
