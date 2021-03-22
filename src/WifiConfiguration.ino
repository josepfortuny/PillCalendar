/***********************
 * Name: WifiConfiguration
 * Function: configurates the WIFI conection with ESP8266 device to update the status sensors in firebase
 * Parameters: *ap_ssid, *ap_password, ap_channel, *wifi_ssid, *wifi_password
 * Return: -
 ***********************/

/****
 * Includes *
 ****/ 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/*****
 * Constants *
 *****/
IPAddress serverIP(192,168,4,1);
IPAddress gateway(127,0,0,1);
IPAddress subnet(255,255,255,0);

/*********
 * Processes and functions *
 *********/
 
void Setup_WIFI_STA_AP_TCP_FIREBASE(const char *ap_ssid,const char *ap_password,int ap_channel,const char *wifi_ssid,const char *wifi_password){
  /* Creating the Mode STA_AP */
  WiFi.mode(WIFI_AP_STA);
  /* Creating the access point */
  WiFi.softAP(ap_ssid, ap_password,ap_channel);
  WiFi.softAPConfig(serverIP,gateway,subnet);
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.print("connecting");

//Waiting status for connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
// if you get a connection, report back via serial:
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
