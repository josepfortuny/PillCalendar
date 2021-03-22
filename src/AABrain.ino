/***********************
 * Name: AABrain
 * Function: Connects the sensors, servo and buttons to with  Firebase
 * Parameters: 
 * Return: -
 ***********************/
 
/**********************
 *  Defines
*********************/
#define PATH_TEMPERATURE "Sensors/Hardware_Grupo08/temperature"
#define PATH_PRECENSE "Sensors/Hardware_Grupo08/precense"
#define PATH_TAPPOSITION "Sensors/Hardware_Grupo08/tapPosition"
#define PATH_TOKENS "Tokens/Hardware_Grupo08"
#define PRESENCE_DETECTED "Hay movimiento"
#define NO_PRESENCE_DETECTED "No Hay movimiento"
#define FALL_DETECTED "Hay caida"
#define FCM_TITLE "CAIDA!!!"
#define FCM_BODY "Se ha detectado una caida!!"
#define TIME_BETWEEN_FALL 10000
#define TIME_ASK_FIREBASE 1000
#define MAX_TAP_POSITION 10
#define MIN_TAP_POSITION 0
#define PULL_FIREBASE 0
#define PULL_BUTTONS 1
#define PULL_TEMPERATURE 2
#define PULL_FALLDETECTION 3

/************
 * Variables 
 ************/
int time_fall_detected = 0;
int brain_state = 0;
int tap_position_firebase = 0;
int tap_position_servo = 0;
int time_firebase = 0;
boolean no_precense = false;

/*Processes and functions */

//Firebase initail configuration of  the tap position
void Setup_Brain(){
  set_tapPositionFirebase(PATH_TAPPOSITION,MIN_TAP_POSITION);
  time_firebase = millis();
}

//Proces structure control of the IOT sensors and control buttons with the firebase conections
void Process_Brain(){
  switch (brain_state){
    
    case PULL_FIREBASE:
      if(!is_servo_busy()){
        if (( millis() - time_firebase )  > (TIME_ASK_FIREBASE)){
          tap_position_firebase = get_tapPostitionFirebase(PATH_TAPPOSITION);
          tap_position_servo= get_tapPositionServo();
          time_firebase = millis();
          if (tap_position_firebase != tap_position_servo){
            MyOwnDebug("Tap position of Servo " + String(tap_position_servo));
            MyOwnDebug("Tap position from firebase " + String(tap_position_firebase));
            if (tap_position_firebase > tap_position_servo ){
              increment_water(tap_position_firebase - tap_position_servo );
              MyOwnDebug("ABRIR EL SERVO " + String((tap_position_firebase - tap_position_servo))+ " POSICIONES");
            }else if (tap_position_firebase < tap_position_servo){
              decrement_water((tap_position_servo - tap_position_firebase));
              MyOwnDebug("CERRAR EL SERVO " + String((tap_position_servo - tap_position_firebase)) + " POSICIONES");
            }
          }
        }
      }
      brain_state = PULL_BUTTONS;
    break;
    //Updates to firebase the tap position and the control buttons of the HW device 
    case PULL_BUTTONS:
      tap_position_servo = get_tapPositionServo();
      if(get_stop_water_pressed()){
        shutdown_water();
        post_FCM(FCM_TITLE, FCM_BODY, PATH_TOKENS);
        MyOwnDebug("Botón stop pulsado");
        set_tapPositionFirebase(PATH_TAPPOSITION,MIN_TAP_POSITION);
        
      }else if(get_add_water_pressed()){
        if( tap_position_servo < MAX_TAP_POSITION ){
          MyOwnDebug("Incrementando 1 posición el grifo del agua");
          increment_water(1);
          set_tapPositionFirebase(PATH_TAPPOSITION,(tap_position_servo +1));
        }
      }else if(get_substract_water_pressed()){
        if( tap_position_servo > MIN_TAP_POSITION ){
          MyOwnDebug("Decrementando 1 posición el grifo del agua");
          decrement_water(1);
          set_tapPositionFirebase(PATH_TAPPOSITION,(tap_position_servo -1));
        }
      }
      reset_buttons();
      brain_state = PULL_TEMPERATURE; 
    break;
    //Temperature output  and evaluation of max temperature reached updated to firebase to notify the pills app user
    case PULL_TEMPERATURE:
      if (data_temperature_available()){
        set_TemperatureFirebase(PATH_TEMPERATURE,String(get_temperature()));
        MyOwnDebug(get_temperature() + String("ºC"));
        if (max_temperature_reached()){
          MyOwnDebug("Se ha superado la temperatura máxima del agua");
          shutdown_water();
          post_FCM(FCM_TITLE, FCM_BODY, PATH_TOKENS);
          set_tapPositionFirebase(PATH_TAPPOSITION,MIN_TAP_POSITION);
        }
        reset_temperature_data();
      }
      brain_state = PULL_FALLDETECTION;
    break;
    //Fall detection pull to firebase. This function also update if the are any presence or not
    case PULL_FALLDETECTION:
      if (is_precense_detected()){
        if (is_fall_detected()){//Send activity to firebase when a fall is detected. Updated to the firebase for be displayed on the app
          set_PrecenseFirebase(PATH_PRECENSE,FALL_DETECTED);
          post_FCM(FCM_TITLE, FCM_BODY, PATH_TOKENS);
          MyOwnDebug("Se ha detectado una caida!!");
          shutdown_water();
          set_tapPositionFirebase(PATH_TAPPOSITION,MIN_TAP_POSITION);
          no_precense = false;
          set_precense_detected_false();
          set_fall_detected_false();
          time_fall_detected = millis();
          
        }else{//When isn't a fall , is a presence detection. Update the status to Firebase to be updated in the phone app Pills
          set_PrecenseFirebase(PATH_PRECENSE,PRESENCE_DETECTED);
          MyOwnDebug("Se detectó presencia");
          no_precense = false;
          set_precense_detected_false();
        }
      }else if(is_no_precense_detected() && !no_precense){//If there aren't any presence detection, there are no presence. Updates that no presence detected to Firebase
        if ((millis() - time_fall_detected)> TIME_BETWEEN_FALL){
          no_precense = true;
          set_PrecenseFirebase(PATH_PRECENSE,NO_PRESENCE_DETECTED);
          MyOwnDebug("Se dejó de detectar presencia");
        }
      }
      brain_state = PULL_FIREBASE;
    break;
  }
}
