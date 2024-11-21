/*
Written by: Zan Pekosak, on: 21. 11. 2024

This is very crude example on how to get a number from the Serial port of an Arduino and directly use it to set the
duty cycle of the onboard LED. The value input on the Serial interface must be between 0 and 100 [%]

No hardware wiring is necessary as the LED and Serial are both built in on most (if not all) Arduino boards
*/

#include <SlowPWM.h>

#define ONBOARD_LED 13
#define PERIOD 1000

char buffer[256];                           //  setup a buffer to store incoming chars
uint8_t bufferWriteCnt = 0;                 //  counter which tells us where we are in the buffer
uint32_t serialInactivityTimer = millis();  //  this tracks how long nothing came on the serial port and starts the msg processing

S_PWM pwm(ONBOARD_LED, PERIOD);  //  construct object on pin 13 with 1 Hz PWM frequency (f = 1/T, T = 1s = 1000ms)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //  initialize serial port
  pwm.begin();        //  initialize PWM
  /*NOTE: Nothing happens here, apart for the pin register being set. No PWM will be output before
  the first call to setDuty()*/
}

void loop() {
  // put your main code here, to run repeatedly:
  pwm.pwmLoop();  //  pin handler function

  while(Serial.available()){  //  while something is in serial buffer
    buffer[bufferWriteCnt++] = Serial.read(); //  write to our buffer and increment its counter
    serialInactivityTimer = millis(); //  reset the inactivity timer
  }

  if((bufferWriteCnt > 0) && ((millis() - serialInactivityTimer) >= 200)){  //  when 200ms have elapsed and something is in our buffer
    buffer[bufferWriteCnt] = '\0';  //  null terminate to make it a string
    bufferWriteCnt = 0; //  reset counter

    /*VERY crude error prevention*/
    if(isDigit(*buffer)){           //  if the first char in buffer is a number
      uint8_t duty = atoi(buffer);  //  convert to number from string
      pwm.setDuty(duty);            //  set the pwm's duty cycle
      /*Print out the set duty cycle*/
      Serial.print("Set duty: ");
      Serial.print(duty);
      Serial.print("\n");
    }
  }
}
