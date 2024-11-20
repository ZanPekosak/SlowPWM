/*
Created by: Zan Pekosak, on: 21. 11. 2024

This library was written with the purpose of controlling total power intput to large inertia power consumers
such as water heaters, furnaces, chillers, etc. 
The idea was to recreate a common industrial PLC functionality where most digital outputs can be configured
as low frequency PWM and can be controlled only by setting the duty cycle. Everything else should happen 
"under the hood", so to speak.

Version 1 (current) of this library implements the "under the hood" part, by the user calling a pwmLoop function
inside the void loop() of Arduino. If necessary, next versions will be based on FreeRTOS, to really achieve perfect
hidden pin manipulation.*/

#ifndef SlowPWM_h
#define SlowPWM_h

#include "Arduino.h"

//#define ENABLE_DEBUG  //  uncomment this to enable Serial output (commented to save program space on Arduino Nano)

class S_PWM
{
  public:
    S_PWM(uint8_t pin, uint16_t period);
    void begin();
    void setDuty(uint8_t dutyPercent);
    void pwmLoop();

  private:
    bool _thowConstructorValueWarn = 0; //  if too low of a period value is input into the class constructor, we set this flag and notify the user via Serial
    bool _state = 0;                    //  variable to track the pin state - minimizes registry reads
    uint8_t _pin = 0;                   //  variable to store the pin number
    uint16_t _periodTime = 0;           //  period time [ms], used to calculate the on and off times
    uint16_t _onTime = 0;               //  how long the pin is high [ms]
    uint16_t _offTime = 0;              //  how long the pin is low  [ms]
    uint32_t _elapOnTime = 0;           //  timer for on state
    uint32_t _elapOffTime = 0;          //  timer for off state
};

#endif