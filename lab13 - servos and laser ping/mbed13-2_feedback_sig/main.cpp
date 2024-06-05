  #include "mbed.h"
  #include "PwmIn.h"
  #include <iostream>
  #define CENTER_BASE 1500
  #define unitsFC 360                          // Units in a full circle
  #define dutyScale 1000                       // Scale duty cycle to 1/000ths
  #define dcMin 29                             // Minimum duty cycle
  #define dcMax 971                            // Maximum duty cycle
  #define q2min unitsFC/4                      // For checking if in 1st uadrant
  #define q3max q2min * 3                      // For checking if in 4th uadrant

  PwmOut servo_c(D11);
  PwmIn servo_f(D10);                               //servo feedback signal

  Ticker feedback_ticker;
  Ticker control_ticker;

  volatile int angle, targetAngle = 0;              // Global shared ariables
  volatile int Kp = 1;                          // Proportional constant
  volatile float tCycle;
  volatile int theta;
  volatile int thetaP;
  volatile int turns = 0;

  void servo_control(int speed) {
     if (speed > 200)       speed = 200;
     else if (speed < -200) speed = -200;

     servo_c = (CENTER_BASE + speed)/20000.0f;
  }

  void feedback360(){                           // Position monitoring
     tCycle = servo_f.period();
     int dc = dutyScale * servo_f.dutycycle();
     theta = (unitsFC - 1) -                   // Calculate angle
              ((dc - dcMin) * unitsFC)
              / (dcMax - dcMin + 1);
     if(theta < 0)                             // Keep theta valid
        theta = 0;
     else if(theta > (unitsFC - 1))
        theta = unitsFC - 1;

     // If transition from quadrant 4 to
     // quadrant 1, increase turns count.
     if((theta < q2min) && (thetaP > q3max))
        turns++;
     // If transition from quadrant 1 to
     // quadrant 4, decrease turns count.
     else if((thetaP < q2min) && (theta > q3max))
        turns --;

     // Construct the angle measurement from the turns count and
     // current theta value.
     if(turns >= 0)
        angle = (turns * unitsFC) + theta;
     else if(turns <  0)
        angle = ((turns + 1) * unitsFC) - (unitsFC - theta);

     thetaP = theta;                           // Theta previous for next rep
  }

void control_target() {
    int err = targetAngle - angle;
    int offset_cnst = 20;
    int offset;
    
    if (err >  200) err = 200;
    if (err < -200) err = -200;
    if (abs(targetAngle - angle) > 3) {
        if (err > 0) {
            offset = offset_cnst;
        } else if (err < 0) {
            offset = -offset_cnst;
        } else {
            offset = 0;
        }
        servo_control(offset + err);
    } else { servo_control(0);
    }
}

int main() {


    printf("---start---\n");

    servo_c.period_ms(20);
    feedback_ticker.attach(&feedback360, 5ms);
    control_ticker.attach(&control_target, 50ms);

    targetAngle = 120;
    while (1) {
        printf("turns = %d, angle = %d\n", turns, angle);
        ThisThread::sleep_for(2s);
    }

}