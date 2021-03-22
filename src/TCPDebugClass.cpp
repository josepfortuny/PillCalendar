/***********************
 * Name: TCPDebugClass
 * Function:  TCP Debug comunication functions of the instructions tcp protocol
 * Parameters: -
 * Return: -
 ***********************/
 
 /***************
 * Includes
 **************/
#include "TCPDebugClass.h"

TCPDebugClass::TCPDebugClass(unsigned int port) :  tcpServer(port) {
  serverPort = port;
}
/*Function to start the communication with the TCP server*/
void TCPDebugClass::begin(){
  #ifdef USE_SERIAL_DEBUG
  Serial.println("Starting TCP Debugging server...");
  #endif
  tcpServer.begin();
  tcpServer.setNoDelay(true);
  #ifdef USE_SERIAL_DEBUG 
  Serial.print("\tAvailable on port: ");
  Serial.println(serverPort);
  #endif
}
/*Function to check conection client*/
//Check if the client is conected, diconected
void TCPDebugClass::checkForClient(){
  if(_hasClient && dbgClient.connected()){
    return;
  }
  else if (_hasClient && !dbgClient.connected()) {
    #ifdef USE_SERIAL_DEBUG
    Serial.println("Debug client disconnected.");
    #endif
    _hasClient = false;
    return;
  }
  else if (!_hasClient && !dbgClient.connected()) {
    dbgClient = tcpServer.available();
    if(!dbgClient.connected())
      return;
    _hasClient = true;
    #ifdef USE_SERIAL_DEBUG 
    Serial.print("Debug client : \r\n\t");
    Serial.print(dbgClient.remoteIP());
    Serial.print(":");
    Serial.println(dbgClient.remotePort());
    #endif
  
    dbgClient.print("Debug client : \r\n\t");
    String tmp = String(dbgClient.remoteIP()[0]) + "." + String(dbgClient.remoteIP()[1]) + "." + String(dbgClient.remoteIP()[2]) + "." + String(dbgClient.remoteIP()[3]) + ":" +String(dbgClient.remotePort());
    dbgClient.println(tmp);
  }
}

bool TCPDebugClass::hasClient() {
  return dbgClient.connected();
}

size_t TCPDebugClass::write(const uint8_t *buffer, size_t size) {
  if(_hasClient) {
    dbgClient.write(buffer, size);
  }
}

size_t TCPDebugClass::write(uint8_t c) {
  if(_hasClient) {
    dbgClient.write(c);
  }
}

/* TCP debug session */
TCPDebugClass TCPDebug(1234);
