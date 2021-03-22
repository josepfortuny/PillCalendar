/***********************
 * Name: TCP
 * Function: Client-Server conection verifying
 * Parameters:String aux
 * Return: -
 ***********************/
 
 /***************
 * Includes
 **************/
#include "TCPDebugClass.h"

/*********
 * Processes and functions *
 *********/
 //Configuration tcp begin
 void Setup_TCP(){
  TCPDebug.begin();
}

//TCP verifying
void printtcp(String aux){
  //Verify the connection
  if(!TCPDebug.hasClient()){
    TCPDebug.checkForClient();
  }else{
      TCPDebug.print(aux+"\n");
  }
}
