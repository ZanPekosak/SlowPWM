#include "Arduino.h"
#include "SlowPWM.h"
#include "SlowPWM_Config.h"

//init function -> sets registers, pins and initial state
S_PWM::S_PWM(uint8_t pin, uint16_t period){
  _periodTime = period;
  _pin = pin; //  set pin number from constructor
  /*If period time which was intput is less
  than 100ms, set it to 100 and */
  if(_periodTime > MIN_PERIOD_TIME){
    _periodTime = period;
  }
  else{
    _periodTime = MIN_PERIOD_TIME;
    _thowConstructorValueWarn = 1;
  }
}

void S_PWM::begin(){
  pinMode(_pin, OUTPUT);    //  set the pinmode as output
  digitalWrite(_pin, 0);    //  set the pin to false, to be sure
  _elapOnTime = millis();   //  intialization of tracking timer for non blocking switching
  _elapOffTime = millis();  //  intialization of tracking timer for non blocking switching

  /*Check if value which was input into the class constructor was too low and set it to the lowest bound*/
  if(_thowConstructorValueWarn){
    _thowConstructorValueWarn = 0; //  reset the flag
    #ifdef ENABLE_DEBUG
    Serial.print("Period time which was input into the class constructor S_PWM() is too low. Setting to 100ms...");
    Serial.print("\tDONE\n");
    #endif
  }

  #ifdef ENABLE_DEBUG
  Serial.print("Selected pin: ");
  Serial.print(_pin);
  Serial.print(", selected period: ");
  Serial.print(_periodTime);
  Serial.print("ms\n");
  #endif
}

void S_PWM::setDuty(uint8_t dutyPercent){
  static bool doonce = 1; //  create flag to track if the waring is issued once
  static bool doonce2 = 1; //  create flag to track if the waring is issued once

  /*If dutyPercent is between 0-1, than calculate and set the values*/
  if(dutyPercent >= MIN_DUTY && dutyPercent <= MAX_DUTY){
    _onTime = ((float)dutyPercent/100)*_periodTime; //  calculate on time
    if(_onTime < MIN_ON_TIME){ //  check if duty time is less than 20ms and prevent it from being less. Thow warning on serial
      if(doonce){
        doonce = 0; //  reset flag
        /*Print warning on Serial, use non formatting functions*/
        #ifdef ENABLE_DEBUG
        Serial.print("Error in function setDuty() - calculated on time value: ");
        Serial.print(_onTime);
        Serial.print(" out of bounds, line: ");
        Serial.print(__LINE__);
        Serial.print("\n");
        #endif
      }
      _onTime = MIN_ON_TIME; //  set the on cycle time to the specified minimum on time
    }
    else{
      doonce = 1; //  reset the flag so we can show a value of of bounds if it happens again
      #ifdef ENABLE_DEBUG
        Serial.print("Calculated ON time: ");
        Serial.print(_onTime);
        Serial.print("\n");
      #endif
    }

    _offTime = (uint16_t)(_periodTime - _onTime); //  calculate off time
    if(_offTime < MIN_OFF_TIME){ //  check if duty time is less than 20ms and prevent it from being less. Thow warning on serial
      if(doonce2){
        doonce2 = 0; //  reset flag
        /*Print warning on Serial, use non formatting functions - Arduino go brrrt*/
        #ifdef ENABLE_DEBUG
          Serial.print("Error in function setDuty() - calculated off time value: ");
          Serial.print(_offTime);
          Serial.print(" out of bounds, line: ");
          Serial.print(__LINE__);
          Serial.print("\n");
        #endif
      }
      _offTime = MIN_OFF_TIME; //  set the duty cycle to 20
    }
    else{
      doonce2 = 1;  //  reset the flag to get next warning
      #ifdef ENABLE_DEBUG
      Serial.print("Calculated OFF time: ");
      Serial.print(_offTime);
      Serial.print("\n");
      #endif
    }
  }
  else{
    #ifdef ENABLE_DEBUG
    Serial.print("Error in function setDuty() - duty parameter out of bounds, line: ");
    Serial.print(__LINE__);
    Serial.print("\n");
    #endif
  }  
}

void S_PWM::pwmLoop(){
  static bool isOn = 0; //  flag to track if the pin register has been written to, prevents rapidly writing to registers
  static bool isOff = 0; //  flag to track if the pin register has been written to, prevents rapidly writing to registers
  /*If bitwise or between onTime and offTime results in 0 ->
  both values are zero and setDuty() has not been called yet,
  thus we don't do anything to the pins*/
  if(_onTime | _offTime){
    /*If pin should be off for less than the minimum tolerance, turn it on fully
    WARNING: reverse logic. Print ON time, but decide based on OFF time*/
    if(_offTime <= ((uint16_t)_periodTime*(TIME_TOLERANCE_FULL_VAL*0.01))){
      if(!isOn){
        #ifdef ENABLE_DEBUG
          Serial.print("Pin should be on. ON time in pwmLoop(): ");
          Serial.print(_onTime);
          Serial.print("\n");
        #endif
        isOn = 1;               //  set flag
        isOff = 0;              //  reset flag
        digitalWrite(_pin, 1);  //  set pin
        _elapOnTime = millis(); //  start counting how long the pin has been off
      }
    }
    else if(_onTime <= ((uint16_t)_periodTime*(TIME_TOLERANCE_FULL_VAL*0.01))){
      if(!isOff){
        #ifdef ENABLE_DEBUG
          Serial.print("Pin should be off. OFF time in pwmLoop(): ");
          Serial.print(_offTime);
          Serial.print("\n");
        #endif
        isOff = 1;                //  set flag
        isOn = 0;                 //  reset flag
        digitalWrite(_pin, 0);    //  turn off pin
        _elapOffTime = millis();  //  start counting how long the pin has been off
      }
    }
    else{
      isOn = 0;  //  reset the flag
      isOff = 0;  //  reset the flag
      /*If the required time for the pin to be on has passed
      and the pin was on, toggle pin to low*/
      if(((millis() - _elapOnTime) >= _onTime) && _state){
        _state = 0;                     //  set state to false
        digitalWrite(_pin, _state);     //  set pin to false
        _elapOffTime = millis();        //  reset time tracker
      }
      /*If the required time for the pin to be off has passed
      and the pin was off, toggle pin to high*/
      if(((millis() - _elapOffTime) >= _offTime) && !_state){
        _state = 1;                     //  set state to true
        digitalWrite(_pin, _state);     //  set pin to true
        _elapOnTime = millis();         //  reset time tracker
      }
    }
  }
}