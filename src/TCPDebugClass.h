/***********************
 * Name: TCPDebugClass
 * Function: TCP Debug comunication structure class using Wifi for conect server-client
 * Parameters:port, uint8_t, *buf, size
 * Return: -
 ***********************/ 
 
/***************
 * Includes
 **************/
#ifndef TCPDebugClass_h
#define TCPDebugClass_h
#include <Arduino.h>
#include <Print.h>
#include <ESP8266WiFi.h>
#include <WiFiServer.h>

/**********************
 *  Defines
*********************/
//#define USE_SERIAL_DEBUG//ELIMINAR
#define BUFF_SIZE 2048

const char startCode[] PROGMEM = {'S','T','A','R','T'};
const char stopCode[] PROGMEM = {'S','T','O','P'};
    //: public Print
class TCPDebugClass : public Print
{
  public:
    TCPDebugClass();
    TCPDebugClass(unsigned int port);
    void begin();
    void checkForClient();
    bool hasClient();
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
  private:
    WiFiServer tcpServer;
    unsigned int serverPort;
    WiFiClient dbgClient;
    bool _hasClient = false;
    uint8_t buff[BUFF_SIZE];
    
};

extern TCPDebugClass TCPDebug;

#endif
