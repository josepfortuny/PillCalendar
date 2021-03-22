/***********************
 * Name: Temperature
 * Function: This function get and evaluate the temperature of the tap witha a sensor. REF. https://naylampmechatronics.com/blog/46_Tutorial-sensor-de-temperatura-DS18B20.html
 * Parameters: pin_termometerv, int max_temperature
 * Return: float temperatureC, boolean data_temperature_ready, boolean temperature_max_reached
 ***********************/
 
 /***************
 * Includes *
 **************/
#include <OneWire.h>
#include <DallasTemperature.h>

/**********************
 *  Defines *
*********************/
#define THIRTY_SEC 30000 //global constant of 30000ms

/************
 * Variables *
 ************/
unsigned long timetemperature = 0;
float temperatureC = 0.0;
int pin_termometerv = 0;
int max_temperaturev = 0;
boolean data_temperature_ready = false;
boolean temperature_max_reached = false;

/*********
 * Processes and functions *
 *********/
//Initialize libraries
OneWire oneWire(pin_termometerv);//Onwire library call; the pin declared as a bus for OneWire communication is established
DallasTemperature sensors; //DallasTemperature library call

/*Return indicators to Firebase*/
float get_temperature(){//Return the temperature value to Firebase 
  return temperatureC;
}
boolean data_temperature_available(){//Return the indicator of the ready info to Firebase 
  return data_temperature_ready;
}
boolean max_temperature_reached(){//Return the indicator of MAX temperature reached to Firebase
  return temperature_max_reached;
}

/*Indicators reset*/
void reset_temperature_data(){
  data_temperature_ready = false;
  temperature_max_reached = false;
}
/*Configures the adquisition of temperature*/
void Setup_Temperature(int pin_termometer, int max_temperature){
  max_temperaturev = max_temperature;//Update the max temperature from Firebase
  pin_termometerv = pin_termometer;//Update the pin input from
  oneWire = OneWire (pin_termometerv);//Declares the Temperature sensor pin
  sensors = DallasTemperature (&oneWire);//A variable or object is declared for our sensor
  sensors.begin();//Temperature sensor starts
  timetemperature = millis();
}
/*Process to adquire and evaluate the temperature*/
void Process_Temperature(){    
  if (timetemperature+THIRTY_SEC< millis()){//Get the temperature every 30 seconds
    sensors.requestTemperatures(); //Send the command to read the temperature
    temperatureC = sensors.getTempCByIndex(0);//Get the temperature in ºC
    timetemperature = millis();//Update the temperature adquisition time 
    data_temperature_ready = true;//Indicator of data temeperature updated
    
    if (temperatureC >= max_temperaturev ){//Evaluates the actual temperature send the flag if the max temperature is reached
      temperature_max_reached = true;//Indicator of max temperature reached
    }
  }
}
