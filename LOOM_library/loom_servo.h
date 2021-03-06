// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <Adafruit_PWMServoDriver.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define SERVOMIN  150     // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600     // This is the 'maximum' pulse length count (out of 4096)


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_servo_t {
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

  #if (num_servos==1)
    int predeg[1] = {0};
    double pre_pulselength[1] = {0.0};
  #elif (num_servos==2)
    int predeg[2] = {0, 0};
    double pre_pulselength[2] = {0.0, 0.0};
  #elif (num_servos==3)
    int predeg[3] = {0, 0, 0};
    double pre_pulselength[3] = {0.0, 0.0, 0.0};
  #elif (num_servos==4)
    int predeg[4] = {0, 0, 0, 0};
    double pre_pulselength[4] = {0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==5)
    int predeg[5] = {0, 0, 0, 0, 0};
    double pre_pulselength[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==6)
    int predeg[6] = {0, 0, 0, 0, 0, 0};
    double pre_pulselength[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==7)
    int predeg[7] = {0, 0, 0, 0, 0, 0, 0};
    double pre_pulselength[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==8)
    int predeg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    double pre_pulselength[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  #endif // of if (num_servos==1) 
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_servo_t state_servo;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_servo();
void set_servo_degree(int set_degree, int servo_choice);
void set_servo(OSCMessage &msg);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================

// -- SERVO SETUP --
// Called by main setup
// Arguments: none
// Return:    none
void setup_servo() {
  state_servo.pwm.begin();
  state_servo.pwm.setPWMFreq(60);
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

// -- SET SERVO DEGREE --
// Changes specified servo (number) to provided position (degree)
// Arguments: set_degree (angle to set servo to), servo_choice (which servo to set)
// Return:    none
void set_servo_degree(int set_degree, int servo_choice) 
{
  uint16_t pulselength = map(set_degree, 0, 180, SERVOMIN, SERVOMAX);
  
  if (set_degree < state_servo.predeg[servo_choice]) {
    for (double pulselen = state_servo.pre_pulselength[servo_choice]; pulselen >= pulselength; pulselen--) {
      state_servo.pwm.setPWM(servo_choice, 0, pulselen);
    }
  }
  
  //When input degree is greater than previous degree, it increases
  if (set_degree > state_servo.predeg[servo_choice]) {
    for (double pulselen = state_servo.pre_pulselength[servo_choice]; pulselen < pulselength; pulselen++) {
      state_servo.pwm.setPWM(servo_choice, 0, pulselen);
    }
  }
  
  state_servo.predeg[servo_choice] = set_degree;
  state_servo.pre_pulselength[servo_choice] = pulselength;
}


// -- SET SERVO --
// Parses OSC message for which servo and position to call set_servo_degree() on
// Arguments: msg (OSC message holding a servo number and angle)
// Return:    none
void set_servo(OSCMessage &msg) 
{
  int servo_num  = msg.getInt(0);
  int set_degree = msg.getInt(1);
  
  #if LOOM_DEBUG == 1
    Serial.print("received message for servo ");
    Serial.print(servo_num);
    Serial.print(" with degree ");
    Serial.println(set_degree);
  #endif
  
  set_servo_degree(set_degree, servo_num);
}


