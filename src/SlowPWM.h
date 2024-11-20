#ifndef SlowPWM_h
#define SlowPWM_h

#include "Arduino.h"

//#define ENABLE_DEBUG

class S_PWM
{
  public:
    S_PWM(uint8_t pin, uint16_t period);
    void begin();
    void setDuty(uint8_t dutyPercent);
    void pwmLoop();

  private:
    bool _thowConstructorValueWarn = 0;
    bool _state = 0;
    uint8_t _pin = 0;
    uint16_t _periodTime = 0;
    uint16_t _onTime = 0;
    uint16_t _offTime = 0;
    uint32_t _elapOnTime = 0;
    uint32_t _elapOffTime = 0;
};

#endif