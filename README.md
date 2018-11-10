# Overview
This solenoid winder is able to produce solenoids which are suitable to be used as actuators.

## Boards and Motors
 - Arduino Uno R3
 - Arduino Motor Shield L298P
 - Servo Motor Tower Pro 9G 180
 - Stepper Motor Nema17 12V
   
## Software
 - Arduino IDE (tested using version 1.8.5)
 
# Setup
 1. Design and manufacture solenoid spool supporter to connect the solenoid spool to the shaft of the stepper motor.
 2. Assemble the solenoid spool and solenoid spool supporter to the shaft of the stepper motor.
 3. Connect the Arduino Uno to computer via the Arduino USB port.
 4. Connect the Arduino Motor Shield to a power source via the 12V power cable.
 5. Place the wire spool and thread the wire through the suitable eye holes * and servo arm onto the solenoid spool.
 * Green eye holes are used for high-tension wire. Red eye holes and wire tensioner are used for low-tension wire.
 
# Calibration
 1. Connect to computer and long press the pushbutton to initiate and reset the map in EEPROM.
 2. Place arduino car near a boundary. 
 3. Switch on the Arduino board and the motors. 
 4. Long press the pushbutton to begin mapping.
 5. Press pushbutton when arduino car is moving in a straight line after mapping is completed.
 6. Connect to computer to obtain map via EEPROM.
 
 Below is a video of the robot in action.
 
 
# Results
 '0' indicates free space while '1' indicates boundaries.
 

# Mechanism
## Moving
 This robot is following the boundaries using left-hand rule.
 - Is obstacle in front?
   - Yes: Turn right
   - No: Proceed checking
 - Is there free space at the left?
   - Yes: Turn left
   - No: Move forward at a distance from the boundaries at the left

## Mapping
 - Look up the direction the robot is facing via dead reckoning (North, South, East, West)
   - If robot turn left: change internal direction towards left (i.e. North to West) 
   - If robot turn right: change internal direction towards right (i.e. North to East)
   - If robot move forward: change '0' to '1' in previous position
