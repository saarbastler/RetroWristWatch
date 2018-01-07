# The PCB
The PCB is using a ATMega328P (Arduino Uno type). 

![Schematic](/schematic.png)

The Bubble display is a common cathode type, so four BC 848 transistors are the digit drivers. The segments are connected to Atmega 328 portpins. The RTC is a DS1337 conneced by I2C.

![PCB](/pcb.png)

I build the PCB using my Stepcraft mill, so there is a RetroUhrCNC.plt file using HPGL code for milling.   