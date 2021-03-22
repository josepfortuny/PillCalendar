  /***********************
 * Name: Servo
 * Function: Controls the electrovalve with a servomotor with open, close and stop functions
 * Parameters: int pin_servo, int aux_tap
 * Return: boolean busy, int tap_position
 ***********************/
 
 /***************
 * Includes
 **************/
#include <Servo.h> //Servo library
/**********************
 *  Defines
*********************/
#define OPEN_TAP_VEL 0
#define CLOSE_TAP_VEL 180
#define STOP_TAP_VEL 90
#define TIME_TAP_MILLIS 500
#define STATE_STOP_SERVO 0
#define STATE_OPEN_SERVO 1
#define STATE_CLOSE_SERVO 2
#define STATE_SHUTDOWN_SERVO 3

int time_servo = 0;
int pin_servov = 0;
int state_servo = 0;
int tap_position = 0;
int tap_difference_position = 0;
boolean busy = false;

Servo servo;//creates the object to include its functions

/*Return indicators to the App*/
int get_tapPositionServo(){
  return tap_position;
}
boolean is_servo_busy(){
  return busy;
}

/*Function to open the tap by pressing once the "+" tap*/
void increment_water(int aux_tap){//Introduces the value to increment from the main project "AABrain" linked with the APP
  servo.write(OPEN_TAP_VEL);//rotates the servo to the specified 0º angle "OPEN_TAP_VEL"
  tap_difference_position = aux_tap;
  busy = true;
  time_servo = millis();
  state_servo = STATE_OPEN_SERVO;
}
/*Function to close the tap by pressing once the "-" tap*/
void decrement_water(int aux_tap){//Introduces the value to increment from the main project "AABrain" linked with the APP
  servo.write(CLOSE_TAP_VEL);//rotates the servo to the specified 180º angle "OPEN_TAP_VEL"
  tap_difference_position = aux_tap;
  busy = true;
  time_servo = millis();
  state_servo = STATE_CLOSE_SERVO;//Stops the servo
  
}
/*Function to fully close the tap*/
void shutdown_water(){
  state_servo = STATE_SHUTDOWN_SERVO;//Fully close tap state
  busy = true;
  time_servo = millis();
  servo.write(CLOSE_TAP_VEL);//rotates the servo to the specified 180º angle "OPEN_TAP_VEL"
}
/*Function to configure the servo motor inital state*/
void Setup_Servo(int pin_servo){//Introduces the ESP8266 pin from the main project "Grupo_8"
  pin_servov = pin_servo;
  state_servo = STATE_STOP_SERVO;//First state of the servo config
  servo.attach(pin_servov); //Functions that initializes the servo pin 
  servo.write(90);//servo positioning
}
/*State machine of servo actuator*/
void Process_Servo(){
  switch(state_servo){
      /*Stop electrovalve State*/        
    case STATE_STOP_SERVO:
      busy = false;
      servo.write(90);//servo positioning
      break;

      /*Opening electrovalve State*/    
    case STATE_OPEN_SERVO:
      if (( millis() - time_servo )  > (TIME_TAP_MILLIS * tap_difference_position)){//Evaluates the state time 
        tap_position += tap_difference_position;
        state_servo = STATE_STOP_SERVO;//Stops the servo
      }
      break;

      /*Closing electrovalve State*/     
    case STATE_CLOSE_SERVO:
      if (( millis() - time_servo )  > (TIME_TAP_MILLIS * tap_difference_position)){//Evaluates the state time 
        tap_position -= tap_difference_position;
        //MyOwnDebug("Tap position of Servo after less" + String(tap_position));
        state_servo = STATE_STOP_SERVO;//Stops the servo
      }    
      break;

     /*Fully close electrovalve State*/ 
    case STATE_SHUTDOWN_SERVO:
      if (( millis() - time_servo )  > ((TIME_TAP_MILLIS -30) * tap_position )){//Evaluates the state time 
        tap_position = 0;//Redefines the tap start position
        state_servo = STATE_STOP_SERVO;//Stops the servo
      }
    break;
  }
}
