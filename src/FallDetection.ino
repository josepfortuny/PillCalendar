/***********************
 * Name: FallDetection
 * Function: Detects fall from two sensors. Evaluates the time and the movements in each one separetly
 * Parameters: pin_precense_top, pin_precense_bottom
 * Return: -
 ***********************/

 /***************
 * Defines
 **************/
 #define TIME_NOPRECENSE 10000
/************
 * Variables 
************/
int pin_precense_topv = 0;
int pin_precense_bottomv = 0;
int signal_top = 0;
int signal_bottom = 0;
int counter_bottom = 0;
int last_signal_top_sensor = 0;
int last_signal_bottom_sensor = 0;
int time_precense = 0;
boolean no_precense_detected = false;
boolean precense_detected = false;
boolean fall_detected = false;
boolean is_no_precense_detected(){
  return no_precense_detected;
}
boolean is_precense_detected(){
  return precense_detected;
}
boolean is_fall_detected(){
  return fall_detected;
}

/*Processes and functions */

//Set the precence detected value to no presence detected
void set_precense_detected_false(){
  precense_detected = false;
}
//Set the fall detected value to no fall detected
void set_fall_detected_false(){
  fall_detected = false;
}
//Presence detection sensors
void Setup_FallDetection(int pin_precense_top , int pin_precense_bottom){
  pin_precense_topv = pin_precense_top;
  pin_precense_bottomv = pin_precense_bottom;
//Pin I/O configuration of the top and bottom sensors
  pinMode(pin_precense_topv, INPUT);
  pinMode(pin_precense_bottomv, INPUT);
  time_precense = millis();
}


 /*Function Falldetection*/
 /*******************************************************************
 *  Function to detect a fall from 2 presence sensors. 
 *  This function evaluates the order in which presence is detected, 
 *  in the event that a lower sensor signal is detected twice.
 *  
 ******************************************************************/

void Process_FallDetection(){
  signal_top = digitalRead(pin_precense_topv);//Data adquisition from top sensor
  signal_bottom = digitalRead(pin_precense_bottomv);//Data adquisition from bottom sensor
  if ( signal_top != last_signal_top_sensor ){//Sensor data evaluation of top sensor
    if(signal_top == HIGH){
      counter_bottom = 0;
      no_precense_detected = false;
      precense_detected = true;
      MyOwnDebug("TOP");
      time_precense = millis();
    }
  }
  if ( signal_bottom != last_signal_bottom_sensor ){//Sensor data evaluation of bottom sensor
    if (signal_bottom == HIGH){//presence detected once
      counter_bottom += 1;
      precense_detected = true;
      no_precense_detected = false;
      MyOwnDebug("Bottom");
      time_precense = millis();
      if (counter_bottom == 2 ){//presence detected twice, Fall!
        fall_detected = true;
        counter_bottom = 0;
        MyOwnDebug("Bottom caida"); 
      }
    }
  }
  if (!precense_detected){//Fall not detected, no presence detected
    if (( millis() - time_precense ) > TIME_NOPRECENSE ){
      no_precense_detected = true;
      fall_detected = false;
      precense_detected = false;
    } 
  }else{
    time_precense = millis();
  }
  last_signal_top_sensor = signal_top;
  last_signal_bottom_sensor = signal_bottom;
}
