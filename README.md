# arduino-rotary
Rotary encoder Arduino source code. Great example how to handle rotary encoders and buttons.

This is an example for rotary encoders and buttons in Arduino. It uses no library because I try to keep it short and simple. The example is using Timer0 to handle the timing. Any of the other timers can be used if Timer0 is already occupied.

The timer is used to create a non-blocking timed updates. Rotary encoders and buttons are relatively slow compared to other events that can happen in the microcontroller. Just assume a 16MHz MCU instruction time compared to rotary encoder 10ms change interval or the 300+ ms needed for button update and deglitch. It is clear that the non-blocking approach will release a lot of processor time which can be used for handling other tasks.

Encoders are deglitched with 2ms delay (timer driven events), position update is done every 100ms.

Output is done via serial port. Both encoder positions are stored as int, unsigned int or char can be used instead.

The source code ocupies 3.77kB (12%) of the flash and 280 bytes (13%) od SRAM when running on Arduino Nano (ATmega328P).

 HW pins assignment 
 
 Encoder A pin  CLK  to Arduino  D2 
 
                DT   to          D9 
                
                SW   to          D8 
                
 Encoder B pin  CLK  to Arduino  D3 
 
                DT   to          D11 
                
                SW   to          D10 
                
 Both enc. pin  +    to          +5V (VCC) 
 
                GND  to          GND 
                
 
