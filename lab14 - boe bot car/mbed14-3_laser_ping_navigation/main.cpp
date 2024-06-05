#include "mbed.h"
#include "bbcar.h"

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

int main() {
   // parallax_ping  ping1(pin8);
   parallax_laserping  ping1(pin8);

    float pin1 = (float)ping1;
    car.goCertainDistance(20.0);

    while(car.checkDistance(1)){
     ThisThread::sleep_for(500ms);
    }
    car.stop();
    float pin2 = (float)ping1;
    
    ThisThread::sleep_for(3s);
    printf("error distance = %f\n", (car.servo0.targetAngle - car.servo0.angle)*6.5*3.14/360);
    printf("\nPing distance: %5f\n", (pin2 - pin1));
}