/*
Define names are pretty self explanatory
Change the values if necessary, to adjust how the PWM behaves and prevent too short time values from being set
by other variables in your code.
*/

//#define ENABLE_DEBUG  //  uncomment this to enable Serial output (commented to save program space on Arduino Nano)

#define MIN_PERIOD_TIME 100         //  [ms]
#define MIN_ON_TIME 20              //  [ms]
#define MIN_OFF_TIME 20             //  [ms]
#define MIN_DUTY 0                  //  [%]
#define MAX_DUTY 100                //  [%]

/*
TIME_TOLERANCE_FULL_VAL - defines how much tolerance you want to have for min and max PWM values 
(at which point the pin should be fully on or fully off). Its implemeted mainly to turn the pin fully
off if the off time is lower than MIN_DUTY and to prevent unnecessary blinking at near 100% duty cycles
I recommend around 3% (~30ms) for 1 second period time. This also enables the library to use a low off time limit 
and at the same time stop blinks at a very low duty cycle (when the calculated off time is lower than MIN_OFF_TIME,
MIN_OFF_TIME is used instead. TIME_TOLERANCE_FULL_VAL will make sure anything below this results in PWM off).
*/
#define TIME_TOLERANCE_FULL_VAL 3   //  [%]