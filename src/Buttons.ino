/***********************
 * Name: Buttons
 * Function: The electrovalve control with the buttons and the information process
 * Parameters: more_watter, less_watter, stop_watter
 * Return: stop_water, add_tapPostion, substract_tapPostion
 ***********************/

/**********************
 *  Defines
*********************/
#define FILTER_BUTTON_TIME 50
#define WAITING_BUTTON_PRESSED 0
#define FILTER_BUTTON_STOP 1
#define FILTER_BUTTON_MORE 2
#define FILTER_BUTTON_LESS 3

/************
 * Variables 
 ************/
int state_buttons = 0;
int last_bounce_time = 0;
int buttonState_more_water = 0;         
int buttonState_less_water = 0;
int buttonState_stop_water = 0;
int lastButtonState_stop_water = 0;  
int lastButtonState_less_water = 0;    
int lastButtonState_more_water = 0;
int pin_more_water = 0;
int pin_less_water = 0;
int pin_stop_water = 0;
boolean stop_water = false;
boolean add_tapPostion = false;
boolean substract_tapPostion = false;

/*Processes and functions */

//Return AABrain the boolean value of the stop water status
boolean get_stop_water_pressed(){
  return stop_water;
}
//Return AABrain the boolean value of the +tap press status
boolean get_add_water_pressed(){
  return add_tapPostion;
}
//Return AABrain the boolean value of the -tap press status
boolean get_substract_water_pressed(){
  return substract_tapPostion;
}
//Function to reset the buttons
void reset_buttons(){
  stop_water=false;
  add_tapPostion = false;
  substract_tapPostion = false;
}

//Configurates all buttons from the input pins
void Setup_Buttons(int more_watter, int less_watter , int stop_watter){//
  pin_more_water = more_watter;
  pin_less_water = less_watter;
  pin_stop_water = stop_watter;
  pinMode(pin_more_water,INPUT);
  pinMode(pin_less_water,INPUT);
  pinMode(pin_stop_water,INPUT);
}

//Process manage the buttons that controls the electrovalve
void Process_Buttons(){
  switch(state_buttons){
     case WAITING_BUTTON_PRESSED://data adquisition from each pin button
        buttonState_more_water = digitalRead(pin_more_water);
        buttonState_less_water = digitalRead(pin_less_water);
        buttonState_stop_water = digitalRead(pin_stop_water);
        if (buttonState_stop_water != lastButtonState_stop_water){//Detection of pressed stop water button
          if ( buttonState_stop_water == HIGH ){
            last_bounce_time = millis();
            state_buttons = FILTER_BUTTON_STOP;
          }
        }else if (buttonState_less_water != lastButtonState_less_water ){//Detection of pressed less water button
          if ( buttonState_less_water == HIGH ){
            last_bounce_time = millis();
            state_buttons = FILTER_BUTTON_LESS;
            
          }
        }else if (buttonState_more_water != lastButtonState_more_water) {//Detection of pressed more water button
          if ( buttonState_more_water == HIGH ){
            last_bounce_time = millis();
            state_buttons = FILTER_BUTTON_MORE;
          }
        }
        lastButtonState_more_water = buttonState_more_water;
        lastButtonState_less_water = buttonState_less_water;
        lastButtonState_stop_water = buttonState_stop_water;
        break;
    case FILTER_BUTTON_STOP:
      if ((millis() - last_bounce_time) > FILTER_BUTTON_TIME){//Evaluates the time pressed from the last press to avoid a fake press
        buttonState_stop_water = digitalRead(pin_stop_water);
        if (buttonState_stop_water == HIGH) {   
          stop_water =true;//Stop water button is pressed
          state_buttons = WAITING_BUTTON_PRESSED;
        }else{
          state_buttons = WAITING_BUTTON_PRESSED;
        }
      }
      break;
    case FILTER_BUTTON_MORE:
      if ((millis() - last_bounce_time) > FILTER_BUTTON_TIME) {//Evaluates the time pressed from the last press to avoid a fake press
        buttonState_more_water = digitalRead(pin_more_water);
        if (buttonState_more_water == HIGH) {
          add_tapPostion = true;//Add water button is pressed
          state_buttons = WAITING_BUTTON_PRESSED;
        }else{
          state_buttons = WAITING_BUTTON_PRESSED;
        }
      }
        break;
    default:
      if ((millis() - last_bounce_time) > FILTER_BUTTON_TIME) {//Evaluates the time pressed from the last press to avoid a fake press
        buttonState_less_water = digitalRead(pin_less_water);
        if (buttonState_less_water == HIGH) {
          substract_tapPostion = true;//Less water button is pressed
          state_buttons = WAITING_BUTTON_PRESSED;
        }else{
          state_buttons = WAITING_BUTTON_PRESSED;
        }
      }
        break;
  }
}
