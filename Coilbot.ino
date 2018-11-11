// Include necessary library here
#include <Servo.h>

// CHANGE THE FOLLOWING VARIABLES AS REQUIRED
const float wire_diameter = 0.5; // variable to store the diameter of the wire in millimeters
int revolution_required = 691; // variable to store the number of revolution required in the spool
bool moving_forward = false; // variable to store the direction of movement of the servo

// Include variables for servo motor
const float radius_servo_arm = 14.5; // variable to store the radius of the servo arm between the axis of rotation to the wire hole in millimeters
const int servo_pin = 6;  // variable to store the pin number of the servo
const int servo_pwm_min = 612; // variable to store the pulse width, in microseconds, corresponding to the minimum (0-degree) angle on the servo
const int servo_pwm_max = 2148; // variable to store the pulse width, in microseconds, corresponding to the maximum (180-degree) angle on the servo
int initial_deg;  // variable to store the initial position of the servo in degrees
int final_deg;  // variable to store the final position of the servo in degrees
int start_deg;  // variable to store the start position of the servo in degrees
float pos; // variable to store the current position of the servo in degrees (initial current position is start position)
Servo myServo;  // servo object to control a servo

// Include variables for stepper motor
const int pwm_a = 3;  // variable to store the pwm pin A position
const int pwm_b = 9;  // variable to store the pwm pin B position
const int dir_a = 2;  // variable to store the direction pin A position
const int dir_b = 8;  // variable to store the direction pin B position
const int STEPS = 2*200;  // variable to store the total count of halfsteps per full rotation (*2 for half steps!), e.g. 1.8deg stepper => 200 steps => 400 halfsteps
int currStep = 0;  // variable to store the current half step, corresponds to angular position,  limits: 0...STEPS
int sub = 0;  // variable to store the current halfstep within repeating sequence (8 halfsteps for each sequence), limits: 0...7
const int step_dir_per_revolution = -400; // variable to store the number of halfsteps (magnitude) and intended direction (sign) per revolution of the stepper motor
const int step_time = 4000;  // variable to store the wait time between steps in microseconds, bigger means smaller speed and vice versa
int revolution_count = 0; // variable to store the counter for number of revolution spun

void setup() {
  // put your setup code here, to run once:

  myServo.attach(servo_pin, servo_pwm_min, servo_pwm_max);  // attach the servo on the pin to the servo object
  pinMode(pwm_a,OUTPUT);  // initialize the pwm pin A as output
  pinMode(pwm_b,OUTPUT);  // initialize the pwm pin B as output
  pinMode(dir_a,OUTPUT);  // initialize the direction pin A as output
  pinMode(dir_b,OUTPUT);  // initialize the direction pin B as output
  Serial.begin(9600); // set the data rate in bits per second (baud) for serial data transmission
  calibration(); // allow user to set the initial position, final position and start position of the servo
}

void loop() {
  // put your main code here, to run repeatedly:

  myServo.write(pos);  // tell servo to go to position in variable 'pos'
  subStep(step_dir_per_revolution, step_time);  // spin stepper for one revolution
  delay(10);  // wait for both the motors to reach the position
  revolution_count += 1; // increment revolution counter
  Serial.print("Revolution spun:\t"); // display information
  Serial.print(revolution_count); 
  Serial.print("\tRevolution left:\t"); 
  Serial.print(revolution_required - revolution_count); 
  Serial.print("\tPosition:\t"); 
  Serial.println(pos); 

  if (revolution_count >= revolution_required){ // check whether the required number of revolution of wire has been spun onto the spool
    Serial.print("Solenoid wiring has been completed. Total number of revolution:"); // display feedback for completion
    Serial.println(revolution_count); 
    while (true){ // go into infinite loop
    turnOffMotors; // turn off the motors
    }
  }

  // increment or decrement degree interval based on intended movement of the servo
  if (moving_forward && pos < final_deg){ // if servo is moving from initial position to final position
    float deg_interval = (acos(cos(pos*3.14159/180)-(wire_diameter/radius_servo_arm))-pos*3.14159/180)*180/3.14159;   // calculate degree interval when the servo is moving from initial position to final position
    pos += deg_interval; // increment degree interval for next position of the servo
    if (isnan(pos)){ // servo will exceed 180 degrees in the current position
      myServo.write(pos);  // tell servo to go to 180 degrees position
      pos = 180; // set position of servo to 180 degrees
    }
    if (pos >= final_deg){ // if position of the servo is equal or more than final position
      pos = final_deg; // set position of servo to final degrees
      moving_forward = false; // make servo moves from final position to initial position
    }
  }
  else if (!moving_forward && pos > initial_deg){ // if servo is moving from final position to initial position
    float deg_interval = (pos*3.14159/180-acos(cos(pos*3.14159/180)+(wire_diameter/radius_servo_arm)))*180/3.14159;   // calculate degree interval when the servo is moving from final position to initial position
    pos -= deg_interval; // decrement degree interval for next position of the servo
    if (isnan(pos)){ // servo will subceed 0 degrees in the current position
      myServo.write(pos);  // tell servo to go to 0 degrees position
      pos = 0; // set position of servo to 0 degrees
    }
    if (pos <= initial_deg){ // if position of the servo is equal or less than initial position
      pos  = initial_deg; // set position of servo to initial degrees
      moving_forward = true; // make servo moves from initial position to final position
    }
  }
}

// This function is called in order to make the stepper motor make a number of sub steps (depending on your wiring).
// Variable steps is for number of steps (forwards = positive, backwards = negative)
// stepDelay is for waiting time between steps in microseconds => bigger means lower speed, smaller means higher speed

void subStep(long steps, int stepDelay){

  // The function will run for the amount of times called in the method.
  // This is accomplished by a while loop, where it will subtract 1 from the amount after every run (forwards).
  // In case of backward rotation it will add 1 to the negative number of steps until 0 is reached.

  while(steps!=0)
    {

    if(steps>0){currStep++;}       //increment current halfstep (forward)
    if(steps<0){currStep--;}       //decrement current halfstep (backward)

    if(currStep>STEPS){currStep= currStep-STEPS;}         //position >360deg is reached => set position one turn back
    if(currStep<0){currStep= currStep+STEPS;}             //position <0deg   is reached => set position one turn forward

    sub = currStep%8;           //determine the next halfstep

    switch(sub)
    {
           case 0: 
    // Starting position (if repeated, ful step (4))
    // EXPLINATION: in this case, both our power are high.
    // Therefore both coils are activated, with their standard polarities for their magnetic fields.
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,HIGH);
    digitalWrite(dir_a,HIGH);
    digitalWrite(dir_b,HIGH);
            break;

            case 1:
    //Half step (½)
    // EXPLINATION: In this case, only out b-coil is active, still with it's stand polarity.
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,LOW);
    digitalWrite(dir_a,HIGH);
    digitalWrite(dir_b,LOW);
            break;

    case 2:
            //Full step (1)
    // EXPLINATION: In this case, the b-coil is activated as in previous cases.
    // But the a-coil now has it's direction turned on. So now it's active, but with the reversered polarity.
    // By continuing this pattern (for reference: http://www.8051projects.net/stepper-motor-interfacing/full-step.gif) , you'll get the axis to turn.
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,HIGH);
    digitalWrite(dir_a,HIGH);
    digitalWrite(dir_b,LOW);
            break;

    case 3:
    // Half step (1½)
    digitalWrite(pwm_a,LOW);
    digitalWrite(pwm_b,HIGH);
    digitalWrite(dir_a,LOW);
    digitalWrite(dir_b,LOW);
            break;

    case 4:
    // Full step (2)
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,HIGH);
    digitalWrite(dir_a,LOW);
    digitalWrite(dir_b,LOW);
            break;

    case 5:
    // Half step (2½)
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,LOW);
    digitalWrite(dir_a,LOW);
    digitalWrite(dir_b,LOW);
            break;

    case 6:
    // Full step (3)
    digitalWrite(pwm_a,HIGH);
    digitalWrite(pwm_b,HIGH);
    digitalWrite(dir_a,LOW);
    digitalWrite(dir_b,HIGH);
            break;

    case 7:
    // Half step (3½)
    digitalWrite(pwm_a,LOW);
    digitalWrite(pwm_b,HIGH);
    digitalWrite(dir_a,LOW);
    digitalWrite(dir_b,HIGH);
            break;
    }

    delayMicroseconds(stepDelay);        //Waiting time to next halfstep

    if(steps>0){steps--;}      //decrement of remaining halfsteps of forward rotation
    if(steps<0){steps++;}      //increment of remaining halfsteps of backward rotation
  }
}


// This function simply just turn off the motors, called when ever we don't need the motors anymore.
// In this way, we won't fray the circuit or coils.
// Note 1: Motor has no torque anymore
// Note 2: If current position is between full steps, motor will move for a half step!
void turnOffMotors(){
  digitalWrite(pwm_a,LOW);
  digitalWrite(pwm_b,LOW);
  digitalWrite(dir_a,LOW);
  digitalWrite(dir_b,LOW);
}

// This function allows user to enter a specified position of the servo.
bool userInput(String servo_pos, int &degree){

  String readString;
  bool input_from_user = true;

  Serial.print("Type the "); // display information
  Serial.print(servo_pos);
  Serial.println(" position of the servo (degrees) into the serial port. Click send.");
  while (Serial.available() <= 0){}; // wait for user input
  while (Serial.available()){; // begin after user input
    char c = Serial.read(); // get one character from serial buffer
    readString += c; // add character into the string
    delay(2); // slow looping to allow buffer to fill with next character
  }
    
  degree = readString.toInt(); // convert string into number
  myServo.write(degree); // move servo to the specified position
  readString = ""; // clear string
  Serial.print("Servo is moving to "); // display information
  Serial.print(servo_pos); 
  Serial.print(" position (degrees):\t"); 
  Serial.println(degree);

  while(input_from_user){ // loop until user enters valid input
    Serial.println("Type Y if the position of the servo is correct. Type N to recalibrate the position of the servo. Click Send."); // display user insctructions
    while (Serial.available() <= 0){}; // wait for user input
    char c = Serial.read(); // get the character from the serial buffer
    if(c == 'Y'){ // user type Y
      input_from_user = false; // do not need further user input
      Serial.print("Calibration of "); // display information
      Serial.print(servo_pos); 
      Serial.print(" position (degrees) is completed:\t");
      Serial.println(degree);
      unsigned long now = millis (); // read and discard any input within 200 milliseconds
      while (millis () - now < 200){
        Serial.read ();
      }
      return false; // calibration of the specified position is completed
    }
    else if(c == 'N'){ // user type N
      input_from_user = false; // do not need further user input
      Serial.print("Recalibrate "); // display information
      Serial.print(servo_pos); 
      Serial.println(" position of the servo.");
      unsigned long now = millis (); // read and discard any input within 200 milliseconds
      while (millis () - now < 200){
        Serial.read ();
      }
    }
    else{ // user do not type Y
      Serial.println("Invalid input. Press retry."); // display information
      unsigned long now = millis (); // read and discard any input within 200 milliseconds
      while (millis () - now < 200){
        Serial.read ();
      }
    }
  }
  return true; // calibration of the specified position is incomplete
}

// This function allows user to set initial position, final position and start position of the servo.
void calibration() {

  bool calibrate_initial_deg = true;
  bool calibrate_final_deg = true;
  bool calibrate_start_deg = true;

  Serial.println("Begin calibration for the initial position, final position and start position of the servo.");

  while(calibrate_initial_deg){ // calibrate for initial position of the servo
    calibrate_initial_deg = userInput("initial",initial_deg);
  }

  while(calibrate_final_deg){ // calibrate for final position of the servo
    calibrate_final_deg = userInput("final",final_deg);
  }

  while(calibrate_start_deg){ // calibrate for start position of the servo
    calibrate_start_deg = userInput("start",start_deg);
  }
  
  Serial.println("Calibration for initial position, final position and start position of the servo is completed."); // display information
  Serial.print("Initial position (degrees):\t");
  Serial.print(initial_deg);
  Serial.print("\t");
  Serial.print("Final position (degrees):\t");
  Serial.print(final_deg);
  Serial.print("\t");
  Serial.print("Start position (degrees):\t");
  Serial.println(start_deg);
  pos = start_deg;
}
