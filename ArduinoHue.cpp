#include "ArduinoHue.h"

int ArduinoHue::turnOn(int light){
	strcpy_P(_buffer, (char*)turn_on_off);
	sprintf(_lastResponse, _buffer, "true");
	return fillRequestAndProcess(light,true,String(ON_DATA));
}
int ArduinoHue::turnOff(int light){
	strcpy_P(_buffer, (char*)turn_on_off);
	sprintf(_lastResponse, _buffer, "false");
	return fillRequestAndProcess(light,true,String(OFF_DATA));
	
}
int ArduinoHue::fillRequestAndProcess(int light,boolean useState, String requestData){
	String requestURL = URL+String("lights/")+String(light);
  if(useState){
    requestURL = requestURL+"/state/";
		sprintf(_buffer, "lights/%i/state/",light);
	}
	else{
		sprintf(_buffer, "lights/%i/",light);
	}
	if(generateRequest(_buffer, _lastResponse)){
		return evaluateResponse();
	}
	return STATUS_POST_ERROR;	
}
int ArduinoHue::setHueSat(int light, long hue, int sat){
	strcpy_P(_buffer, (char*)change_color_string);
   
  //Serial.print("_buffer = ");
  //Serial.println(_buffer);
	sprintf(_lastResponse, _buffer, hue, sat);
  /*
  Serial.print("_buffer = ");
  Serial.println(_buffer);
  Serial.print("hue = ");
  Serial.println(hue);
  Serial.print("_lastResponse = " );
  Serial.println(_lastResponse);
  */
	return fillRequestAndProcess(light,true,String(_lastResponse));
}
int ArduinoHue::evaluateResponse(){
	char* pch = strstr (_lastResponse,"\"response\":\"success\"");
	if(pch == NULL){
		pch = strstr (_lastResponse,"\"response\":\"error\"");
		if(pch == NULL){
			return STATUS_SERVER_UNKNOWN_ERROR;
		}
		return STATUS_SERVER_KNOWN_ERROR;
	}
	return STATUS_SUCCESS;
}

int ArduinoHue::setBri(int light, int bri){
	strcpy_P(_buffer, (char*)set_bri_string); 
	sprintf(_lastResponse, _buffer, bri);
	return fillRequestAndProcess(light,true,String(_lastResponse));

}
int ArduinoHue::setShortAlert(int light, boolean on){
	strcpy_P(_buffer, (char*)set_alert_string); 
	sprintf(_lastResponse, _buffer, on);

	return fillRequestAndProcess(light,true,String(_lastResponse));

}
int ArduinoHue::setLongAlert(int light, boolean on){
	strcpy_P(_buffer, (char*)set_alert_string); 
	sprintf(_lastResponse, _buffer, on);
	return fillRequestAndProcess(light,true,String(_lastResponse));

}
int ArduinoHue::setTransition(int light, int transition){
	strcpy_P(_buffer, (char*)set_alert_string); 
	sprintf(_lastResponse, _buffer, transition);
	return fillRequestAndProcess(light,true,String(_lastResponse));
}