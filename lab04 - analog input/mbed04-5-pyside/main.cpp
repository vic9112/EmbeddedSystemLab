#include "mbed.h"

int sample = 10;

float ADCdata;

int main(){
  for (int j=0; j<10;j++){ //run for 10 loops of waveforms

    ADCdata=0.0; //initial value
    for(int i=0;i<sample;i++){
        //Generate samples directly
        if(i<5){ADCdata=1.0;}
        else{ADCdata=2.0;}

        printf("%f\r\n", ADCdata);
        ThisThread::sleep_for(1000ms/sample);
    }
  }
}