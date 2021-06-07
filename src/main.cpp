#include <Arduino.h>
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>

// Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Replace with your network credentials
const char* ssid = "...";
const char* password = "...";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// String getTemperature() {
//   float temperature = bme.readTemperature();
//   // Read temperature as Fahrenheit (isFahrenheit = true)
//   //float t = dht.readTemperature(true);
//   Serial.println(temperature);
//   return String(temperature);
// }
  
// String getHumidity() {
//   float humidity = bme.readHumidity();
//   Serial.println(humidity);
//   return String(humidity);
// }

// String getPressure() {
//   float pressure = bme.readPressure()/ 100.0F;
//   Serial.println(pressure);
//   return String(pressure);
// }

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.print("-");
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "true";
    }
    else{
      ledState = "false";
    }
    Serial.print(ledState);
    return ledState;
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // // Initialize the sensor
  // if (!bme.begin(0x76)) {
  //   Serial.println("Could not find a valid BME280 sensor, check wiring!");
  //   while (1);
  // }

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }
  WiFi.softAP("ESP8266");


  // Print ESP32 Local IP Address
  // Serial.println(WiFi.localIP());
  IPAddress myIP = WiFi.softAPIP(); 
  Serial.print("IP del acces point: ");
  Serial.println(myIP);
  Serial.println("WebServer iniciado...");

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", getTemperature().c_str());
  // });
  
  // server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", getHumidity().c_str());
  // });
  
  // server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", getPressure().c_str());
  // });

  // Start server
  server.begin();
}
 
void loop(){
  
}
