//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.
/**********************
 *  Defines
*********************/

#include "FirebaseESP8266.h"
#include <ArduinoJson.h>

/*Processes and functions */

// Set these to run example.
StaticJsonBuffer<200> jsonBuffer;
String server_key;
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;
int iteration = 0;

//Configurates the structure of firebase
void Setup_Firebase(String firebase_host, String firebase_auth, String serv_key){
  Firebase.begin(firebase_host, firebase_auth);
  server_key = serv_key;
  Firebase.reconnectWiFi(true);
  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);
  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

//Structure message with the temperature value included. This function send the data to firebase
void set_TemperatureFirebase(String path_temperature,String temperature){
  Firebase.setString (firebaseData,path_temperature,temperature);
}

//Structure message with the presence value included to set.This function send the data to firebase
void set_PrecenseFirebase(String path_precense,String precense){
  Firebase.setString(firebaseData,path_precense,precense);
}

//Structure message with the tap position value included to set. This function get the data to firebase
void set_tapPositionFirebase(String path_tapPosition,int tapPosition){
  Firebase.setInt(firebaseData,path_tapPosition,tapPosition);
}
//Structure message with the tap position value included to get. This function get the data from firebase
int get_tapPostitionFirebase(String path_tapPosition){
   Firebase.getInt(firebaseData,path_tapPosition);
   return firebaseData.intData();
}
/*Function to get the tokens from firebase*/
int get_howManyTokens(String path_tokens){
  iteration = 0;
  int j=0;
  Firebase.getJSON(firebaseData,path_tokens);
  FirebaseJson &json = firebaseData.jsonObject();
  size_t len = json.iteratorBegin();
  for (size_t i = 0; i < len; i++){
    //json.iteratorGet(i, type, key, value);
    j ++;
  }
  json.iteratorEnd();
  return j;
}
/*Function to get the token of the firebase*/
String get_tokensFirebase(String path_tokens, int position){
  int j = 0;
  int type = 0;
  String key, value = "";
  Firebase.getJSON(firebaseData,path_tokens);
  FirebaseJson &json = firebaseData.jsonObject();
  size_t len = json.iteratorBegin();
  for (size_t i = 0; i < len; i++){
    if ( j == position){
      json.iteratorGet(i, type, key, value);
      json.iteratorEnd();
      MyOwnDebug("VALUE" +key);
      return value;
    }
    j += 1; 
  }
  
}
/*Function to post Firebase Cloud Messaging*/
//Sends the FCM message
void post_FCM(String Title, String Body, String path_tokens){
  int num_tokens = get_howManyTokens(path_tokens);
  if (num_tokens > 0){
    String  tokensarray[num_tokens];
    firebaseData.fcm.begin(server_key);
    for (int i=0;i<num_tokens ; i++){
      MyOwnDebug(get_tokensFirebase(path_tokens,i));
      firebaseData.fcm.addDeviceToken(get_tokensFirebase(path_tokens,i));
    }
    firebaseData.fcm.setPriority("high");
    //MyOwnDebug(tokensarray[0]);
    sendMessage(Title,Body);
  }
 firebaseData.fcm.clearDeviceToken();
}

//Message structure data to send to firebase
void sendMessage(String Title,String Body){
  int num_tokens = 4;
  //String data = "\"data\":{";
  //data += "\"body\": \"" + Body + "\",";
  String data = "{\"body\": \"" + Body + "\",";
  data += "\"title\" : \"" + Title + "\" ";
  data += "}";
  //firebaseData.fcm.setNotifyMessage(Title, Body );
  firebaseData.fcm.setDataMessage(data);
  //firebaseData.fcm.setDataMessage("{\"myData\":" + String(num_tokens) + "}");
  if (Firebase.sendMessage(firebaseData, 0)){//send message to recipient index 0
    MyOwnDebug(firebaseData.fcm.getSendResult());
  }else{
    MyOwnDebug("REASON: " + firebaseData.errorReason());
  }
}
