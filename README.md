This library was written with the purpose of controlling total power intput to large inertia power consumers
such as water heaters, furnaces, chillers, etc. 
The idea was to recreate a common industrial PLC functionality where most digital outputs can be configured
as low frequency PWM and can be controlled only by setting the duty cycle. Everything else should happen 
"under the hood", so to speak.

Version 1 (current) of this library implements the "under the hood" part, by the user calling a pwmLoop function
inside the void loop() of Arduino. If necessary, next versions will be based on FreeRTOS, to really achieve perfect
hidden pin manipulation.
