#include <Arduino.h>
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

// Replace with your network credentials
#include "credentials.h" //include a file with your credentials like: const char* ssid = "..."
//                                                                    const char* password = "..."  

// Set LED GPIO
const int isLoggingPin = 0;
const int stopLoggingPin = 2;
// Stores LED state
String ledState;
bool state = false;

// Create AsyncWebServer object on port 80 this is the default port if you enter the IP
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){ //Should change this maybe to handle some stuff but specifically
  Serial.print("-");
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(isLoggingPin)){
      ledState = "true";
    }
    else{
      ledState = "false";
    }
    Serial.print(ledState);
    return ledState;
  }
}

String isLoggingState(){
  state = digitalRead(isLoggingPin);
  String Sstate;
  if(state) Sstate="true";
  else Sstate="false";

  return Sstate;
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200); //Should be okay because we should communicate this with the arduino
  pinMode(isLoggingPin, INPUT);

  pinMode(stopLoggingPin, OUTPUT);
  digitalWrite(stopLoggingPin, LOW);

  // Initialize SPIFFS file system where the page will be stored
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Make the acces point(WiFi Network) with the name passed, you can also pass a password to it
  // WiFi.softAP("TorqueDAQ");


  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Print IP for the server
  // IPAddress myIP = WiFi.softAPIP(); 
  // Serial.print("IP del acces point: ");
  // Serial.println(myIP);
  // Serial.println("WebServer iniciado...");


  //The server endpoints, this should be request that the client could do

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/form.html", String(), false, processor);
  });
  server.on("/sample-rate", HTTP_POST, [](AsyncWebServerRequest *request){
    String sps;
    if(request->hasParam("SPS", true)){
      sps = request->getParam("SPS", true)->value();
      Serial.println("init:SPS=" + sps);
    }
    request->send(204);
  });
  
    //This endpoint allow the client to toggle the state
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){ 
    if(digitalRead(isLoggingPin)){
      digitalWrite(stopLoggingPin, HIGH);
    }
    request->send_P(200, "text/plain", isLoggingState().c_str());
  });

    //This endpoint allow the client to consult the logging state
  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){ 
    if(!digitalRead(isLoggingPin)){
      digitalWrite(stopLoggingPin, LOW);
    }
    request->send_P(200, "text/plain", isLoggingState().c_str());
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  // Start server
  server.begin();
}

void loop(){ //Loop is not necessary because we will handle everything by the server
  
}
