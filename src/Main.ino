/*********************************************
  Main
  ------------------

  This is the main code used to control an IOT device based in ESP8266 with some sensors: temperature, fall detection and electrovalve control.
  All this sensors are connected to an Android applicatation with WIFI conection and all the data involve is in the cloud using Firebase.
  
*********************************************/

/**********************
 *  Defines
*********************/
#define DEBUG
//#define PRINTCP
#ifdef DEBUG
  #define MyOwnDebug(x) Serial.println(x)
#else
  #ifdef PRINTCP
    #define MyOwnDebug(x) printtcp(x)
  #else
    #define MyOwnDebug(x)
  #endif
#endif
#define MAX_WATER_TEMPERATURE 28
/**********************
 *  PIN Defines
*********************/
#define PIN_SERVO 15 // D8 GPIO-15
#define PIN_MORE_WATER 5 // D1 GPIO-5
#define PIN_LESS_WATER 16 // D0 GPIO-16 
#define PIN_STOP_WATER 4  // D2 GPIO-4
#define PIN_TERMOMETER 0 //D3 GPIO-0
#define PIN_PRECENSE_BOTTOM 14 //D5 GPIO-14
#define PIN_PRECENSE_TOP 12 //D6 GPIO-12
#define FIREBASE_HOST "roboticagrupo08-dee61.firebaseio.com"
#define FIREBASE_AUTH "H2LRbOlKlVFyYJWcnPHGVZ6CVORVDPWoIgV7t0wo"//User key
#define WIFI_SSID "PISIT"//WIFI Network
#define WIFI_PASSWORD "jnN96tgX"
#define AP_SSID "WIFI_GRUPO8"//Acces point 
#define AP_PASSWORD "thebestgroup"//Password
#define AP_CHANNEL 6
#define SERVER_KEY "AAAAOssh_o0:APA91bGjz1HLoKbkbDHPVPwkyJJ9aCNySdn_qt1mEwv-FiSInyuPAwGRS6w5Ugc1c57y4vZ-oQggx6dfTGh4ODXDEP979nwhk0mSclEDXrBUGhrfSIHvwhFInd1HUSeVHJo-SapNGB-C"

/*Processes and functions */
//initialization and configuration of sensors and connections functions with the defined parameters
void setup() {
  Serial.begin(115200);
  Setup_WIFI_STA_AP_TCP_FIREBASE(AP_SSID,AP_PASSWORD,AP_CHANNEL,WIFI_SSID,WIFI_PASSWORD);
  Setup_Firebase(FIREBASE_HOST,FIREBASE_AUTH,SERVER_KEY);
  Setup_TCP();
  Setup_Buttons(PIN_MORE_WATER,PIN_LESS_WATER,PIN_STOP_WATER);
  Setup_Temperature(PIN_TERMOMETER, MAX_WATER_TEMPERATURE);
  Setup_FallDetection(PIN_PRECENSE_TOP,PIN_PRECENSE_BOTTOM);
  Setup_Servo(PIN_SERVO);
  Setup_Brain(); 
  delay(100);
} 

//Pocess loop of the activities functions implemented
void loop(){
   Process_Brain();
   Process_Temperature();
   Process_Buttons();
   Process_FallDetection();
   Process_Servo();
}
