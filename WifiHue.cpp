#include "WifiHue.h"

WifiHue::WifiHue(char* ipAddress, Stream* serial){
	_ipAddress = ipAddress;
	_serial = serial;
  //URL = String("");
}

boolean WifiHue::connect(char* clientName){
	_clientName = clientName;
   URL = String("https://") + String(_ipAddress)+ String("/api/") + String(_clientName);// + String("/");
  _serial->println(URL);
	//_deviceId = _deviceId;
}

boolean WifiHue::generateRequest(char* request, char* data){
	_serial -> flush();
  //_serial->write(0x04);
  _serial->print("beginCode");
  _serial->print("\n");
	_serial->print("https://");
	_serial->print(_ipAddress);
	_serial->print("/api/f1pjATrXnKQwWzu5aJbUF7CSY54uQruTH0SRqgBc/");
  //_serial->print(_username);
  //_serial->write(0x05);
  //_serial->write(0x06);
  _serial->print("\n");
	_serial->print(request);
	//_serial->write(0x05);
//	_serial->write(0x08);
  _serial->print("\n");
	_serial->print(data);
  _serial->print("\n");
  _serial->print("exitCode");
  _serial->print("\n");
	//_serial->write(0x03);

	return true;//waitForResponse();
}
boolean WifiHue::waitForResponse(){
	while(_serial->available() <=0 ){
		delay(10);
	}
	int i=0;
	_lastResponse[0]='\0';
	while(_serial->available() > 0){
		_lastResponse[i] = _serial->read();
		i++;
		delay(10);
	}
	_lastResponse[i]='\0';
	return true;
}