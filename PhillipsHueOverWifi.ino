// Program to control phillips hue lights using an ESP32 based board.
// Currently running on https://docs.m5stack.com/en/core/stamp_c3u

#include "ArduinoHue.h"
#include "WifiHue.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

//Setting up the WiFi handling
const char* ssid = "stonks";
const char* WiFiPassword = "stormblaster5000";
char* hueClientName = "f1pjATrXnKQwWzu5aJbUF7CSY54uQruTH0SRqgBc";

//Set up the I/O control
int brightnessPin = 3;
int brightnessValue = 0;
int lastBrightness = 0;
int colorPin = 4;
long colorValue = 0;
long lastColor = 0;

//Hue Data constants
const int LIGHT = 10;
//const String OFF_DATA = "{\"on\": false}";
//const String ON_DATA = "{\"on\": true}";

//set to true for debug output
bool debug = false;

//Rework this for wifi control***************
WifiHue hue("192.168.1.8", &Serial);
int test = 0;
int next = 0;
unsigned long currentMillis = 0;


//Button Definition code, probably belongs in it's own file
struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {9, 0, false};  //the built in button on the M5stamp-C3U is connected to GPIO pin 9

void ARDUINO_ISR_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}

void ARDUINO_ISR_ATTR isr() {
    button1.numberKeyPresses += 1;
    button1.pressed = true;
}

void setup(){
  delay(1000);
  Serial.begin(115200);
  Serial.println("starting up");

  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
  
  neopixelWrite(2,100,100,100);

  //Setup WiFi Connection
  initializeWiFi();

  hue.connect(hueClientName);
}

void loop(){
  
    //if we lose wifi, skip this rest and let me know
  if(WiFi.status()!=WL_CONNECTED)
  {     
        Serial.println("waiting on wifi");
        delay(1000);
        return;
  }
    
  //temporary line below for initial bring up, this will be built up based on function call later
  char* serverPath;
  serverPath = "https://192.168.1.8/api/f1pjATrXnKQwWzu5aJbUF7CSY54uQruTH0SRqgBc/lights/3/state/";
  

  //compare brightness and color values from the potentiometers, send update command if change is large enough
  if (abs(brightnessValue - lastBrightness) > 80)
  {
    Serial.print("New Brightness Value = ");
    Serial.println(brightnessValue);
    lastBrightness = brightnessValue;
    int brightness = lastBrightness/16;
    hue.setBri(LIGHT, brightness);
    delay(200);
  }
  brightnessValue = analogRead(brightnessPin);

  int colorDifference = colorValue - lastColor;
  //Serial.println(colorDifference);
  //Serial.println(abs(colorDifference));
  //delay(2000);
  if (abs(colorValue - lastColor) > 80)
  {
    Serial.print("New Color Value = ");
    Serial.println(colorValue);
    lastColor = colorValue;
    long color = lastColor*16;
    hue.setHueSat(LIGHT, color, 254);
    delay(200);
  }
  colorValue = analogRead(colorPin);
  


  
  // if (!(millis()- currentMillis > 5000))
   // return;
  //if (!Serial) 
  //  return; // wait for serial port to connect. Needed for native USB

    if (button1.pressed) {


      //Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
      button1.pressed = false;
  
      if (next==0)
      {
        neopixelWrite(2,0,100,0); 
        //Serial.println("on");
        putJson(serverPath,ON_DATA);
        //http.begin(serverPath);
        //http.PUT(ON_DATA);
        //hue.turnOn(10);
        next = 1;
      }
      else if (next == 1)
      {
        neopixelWrite(2,100,0,0); 
        //Serial.println("off");
        putJson(serverPath,OFF_DATA);
        //http.begin(serverPath);
        //http.PUT(OFF_DATA);
        //hue.turnOff(10);
        next = 0;
      }
     // delay(5000);
    }

  if(debug)
  {
    Serial.print(currentMillis);
    Serial.println();
    Serial.print(millis());
    Serial.println();
  }
  currentMillis = millis();


    //hue.connect(/*"YOUR_DEVICE_ID",*/ "f1pjATrXnKQwWzu5aJbUF7CSY54uQruTH0SRqgBc");
  //Everything else.
  //Serial.print("test now ");
  //Serial.print(test);
  //Serial.println();
  //test++;
  //delay(5000);
  
}

void putJson(const char *url, String content) {
  Serial.println("putJson");
  Serial.printf("PUT %s: %s\n", url, content.c_str());

  HTTPClient http;
  http.begin(url);
  int httpCode = http.PUT(content);
  if (httpCode > 0) {
    Serial.printf("Code: %d\n", httpCode);
  } else {
    Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
  }
  Serial.println(http.getString());
  http.end();
}

void initializeWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, WiFiPassword);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

