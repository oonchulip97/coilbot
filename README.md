# Overview
This solenoid winder is able to produce solenoids which are suitable to be used as actuators.

## Boards and Motors
 - Arduino Uno R3
 - Arduino Motor Shield L298P
 - Servo Motor Tower Pro 9G 180
 - Stepper Motor Nema17 12V
   
## Software
 - Arduino IDE (tested using version 1.8.5)
 
## Photos
 <img src="/Images/eye_holes.png"  width="480">
 <img src="/Images/spool_supporter.png"  width="480">
 <img src="/Images/servo_position.png"  width="480">
 
# Setup
 1. Design and manufacture solenoid spool supporter to connect the solenoid spool to the shaft of the stepper motor.
 2. Assemble the solenoid spool and solenoid spool supporter to the shaft of the stepper motor.
 3. Connect the Arduino Uno to computer via the Arduino USB port.
 4. Connect the Arduino Motor Shield to a power source via the 12V power cable.
 5. Place the wire spool and thread the wire through the suitable eye holes and servo arm hole onto the solenoid spool.
 * NOTES: Green eye holes are used for high-tension wire. Red eye holes and wire tensioner are used for low-tension wire.
 
# Calibration
 1. Measure and change the diameter of the wire in the Arduino Sketch.
 2. Adjust the number of revolutions required on the Solenoid Spool in the Arduino Sketch.
 3. Adjust the initial direction of the servo motor as needed in the Arduino Sketch.
 4. Open Serial Monitor.
 5. Type in the initial position (in degrees) of the servo motor so the servo arm hole align with the end of the solenoid spool.
 6. Type in the final position (in degrees) of the servo motor so the servo arm hole align with another end of the solenoid spool.
 7. Type in the start position (in degrees) of the servo motor. Typically, start position is the same as the final position.
 8. Number of revolutions spun, number of revolutions remaining and position of the servo (degrees) will be shown in the Serial Monitor.
 * NOTES: Press the reset button on the Arduino Uno to stop and reset the operation.
 
# Results
