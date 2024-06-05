#include "mbed.h"
#include "bbcar.h"

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);
BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

BusInOut qti_pin(D4,D5,D6,D7);
DigitalInOut pin8(D8);


int main() {

    parallax_qti qti1(qti_pin);
    int pattern;

    car.goStraight(40);

    while(1) {
        pattern = (int)qti1;
        parallax_laserping  ping1(pin8);
        // printf("%d\n",pattern);
        if ((float)ping1 < 40) {
            printf("%5f\n", (float)ping1);
        }
        switch (pattern) {
            case 0b1000: car.turn(50, 0.03); break;
            case 0b1100: car.turn(50, 0.05); break;
            case 0b0100: car.turn(40, 0.4); break;
            case 0b1110: car.turn(40, 0.2); break; // Branch
            case 0b0110: car.goStraight(38); break;
            case 0b0111: car.turn(40, -0.2); break; // Branch
            case 0b0010: car.turn(40, -0.4); break;
            case 0b0011: car.turn(50, -0.05); break;
            case 0b0001: car.turn(50, -0.03); break;
            case 0b1111: if (ping1 < 45) car.turn(40, 0.2); else car.stop(); break;
            default: car.goStraight(38);
        }
        ThisThread::sleep_for(10ms);
    }
}