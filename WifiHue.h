#ifndef WIFIHUE_H
#define WIFIHUE_H
#include <Arduino.h>
#include <Stream.h>
#include "ArduinoHue.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>



class WifiHue: public ArduinoHue{
public:
	WifiHue(char* ipAddress, Stream* serial);
	//boolean connect(char* deviceId, char* username);
  boolean connect(char* clientName);
protected:
	char* _ipAddress;
	char* _clientName;
	boolean generateRequest(char* request, char* data);
	boolean waitForResponse();
	Stream* _serial;
  String user;
  String ipAddress;
};

#endif