# Lab 3 Analog Output

## Lab Introduction

- Mbed has two interfaces to support non-digital outputs: AnalogOut and PwmOut. The AnalogOut Interface uses digital to analog converter (DAC) circuit on chip to generate desired voltage level at an analog output pin. The voltage output of AnalogOut Interface has the characteristic of the DAC: voltage range between VSS and VCC, driving current, conversion speed and accuracy.
- The PwmOut interface can be used to set the frequency and duty-cycle ratio (percentage of voltage high in a cycle) of a digital pulse train. PWM signals are often used to drive a motor servo. We will use PWM to control the Boe Bot Car at later lab.

## Equipment List
- B-L4S5I-IOT01A * 1
- USB Type-A male to USB Micro-B male * 1 (compatible with B-L4S5I-IOT01A)
- Breadboard * 1 (with at least 64 rows of pins)
- Picoscope * 1
- Jumper wire male to male

## How to Use PicoScope
- Please read through chapter 4 of [PicoScope 6 User's Guide](https://www.picotech.com/download/manuals/picoscope-6-users-guide.pdf) to get a general idea of picoscope's function. Even though we use the beta version of Picoscope 7, there is currently no document for this version
