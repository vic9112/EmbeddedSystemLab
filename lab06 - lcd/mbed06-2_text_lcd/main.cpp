#include "mbed.h"
#include "TextLCD.h"

DigitalOut led(LED1);      //LED1 = PA_5
TextLCD lcd(D2, D3, D4, D5, D6, D7);
AnalogIn Ain(A0);

// EXAMPLE1
/*
int main()
{
      lcd.printf("HELLO\n");
      for (char x = 0x00; x <= 0x09; x++)
      { // display numbers 0-9
            lcd.printf("%u", x);
      }
}
*/
//EXAMPLE2
/*
int main()
{
      int x=0;
      lcd.printf("Hello World!\n");
      while(true)
      {
            led = !led;             // toggle led
            lcd.locate(5,1);
            lcd.printf("%5i",x);    //counter display
            ThisThread::sleep_for(1s);
            x++;
      }
}
*/

//EXAMPLE3

int main()
{
      float percentage;
      int D;
      while (1)
      {
            percentage = Ain * 100;
            D = int(percentage);
            float B = percentage - D;
            int C = B * 1000000;

            lcd.printf("%d.", D);
            lcd.printf("%d", C);
            ThisThread::sleep_for(250ms);
            lcd.cls();
      }
}