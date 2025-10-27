// Duncan Wilson Oct 2025 - v1 - MQTT messager to vespera

#include <DHT.h>

// DHT22
#define DHTPIN 6      
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

// Switch to control whether the lights change or not
#define SWITCH_PIN 7 

// works with MKR1010

#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010

/*
**** please enter your sensitive data in the Secret tab/arduino_secrets.h
**** using format below
#define SECRET_SSID "ssid name"
#define SECRET_PASS "ssid password"
#define SECRET_MQTTUSER "user name - eg student"
#define SECRET_MQTTPASS "password";
 */
const char* ssid          = "CE-Hub-Student";
const char* password      = "casa-ce-gagarin-public-service";
const char* mqtt_username = "student";
const char* mqtt_password = "ce2021-mqtt-forget-whale";
const char* mqtt_server   = "mqtt.cetools.org";
const int mqtt_port       = 1884;

// create wifi object and mqtt object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Make sure to update your lightid value below with the one you have been allocated
String lightId = "11"; // the topic id number or user number being used.

// Here we define the MQTT topic we will be publishing data to
String mqtt_topic = "student/CASA0014/luminaire/" + lightId;            
String clientId = ""; // will set once i have mac address so that it is unique

// NeoPixel Configuration - we need to know this to know how to send messages 
// to vespera 
const int num_leds = 72;
const int payload_size = num_leds * 3; // x3 for RGB

// Create the byte array to send in MQTT payload this stores all the colours 
// in memory so that they can be accessed in for example the rainbow function
byte RGBpayload[payload_size];

void setup() {
  Serial.begin(115200);
  //while (!Serial); // Wait for serial port to connect (useful for debugging)
  Serial.println("Vespera");
  dht.begin();
  pinMode(SWITCH_PIN, INPUT_PULLUP); // The switch is disconnected for high level and low level closure


  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

  Serial.print("This device is Vespera ");
  Serial.println(lightId);

  // Connect to WiFi
  startWifi();

  // Connect to MQTT broker
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setBufferSize(2000);
  mqttClient.setCallback(callback);
  
  Serial.println("Set-up complete");
}
 
void loop() {
  // Reconnect if necessary
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  
  if (WiFi.status() != WL_CONNECTED){
    startWifi();
  }
  // keep mqtt alive
  mqttClient.loop();


  while (true) {
    mqttClient.loop();
    if (WiFi.status() != WL_CONNECTED){
      startWifi();
    }
    int switchState = digitalRead(SWITCH_PIN);
    if (switchState == LOW) {
      float temp = dht.readTemperature();
      float humi = dht.readHumidity();
      if (isnan(temp) || isnan(humi)) {
        Serial.println("Unable to read temperature and humidity data.");
        delay(1000);
        continue;
      }
      //Adjust the color of the lights within the range of 25 to 30 degrees Celsius.
      int r, g, b;
      if (temp <= 25) {
        r = 0; g = 0; b = 255;
      } else if (temp >= 30) {
        r = 255; g = 0; b = 0;
      } else {
        float ratio = (temp - 25) / 5.0;
        r = (int)(0 + ratio * 255);
        g = 0;
        b = (int)(255 - ratio * 255);
      }
      //Adjust the shade of the light color between 50% and 100% humidity.
      float factor;
      if (humi <= 50) {
        factor = 1.0;
      } else if (humi >= 100) {
        factor = 0.5;
      } else {
        factor = 1.0 - (humi - 50) / 100.0;
        factor = 0.5 + (1.0 - (humi - 50) / 50.0) * 0.5; // 50% at 1.0，100% at 0.5
      }
  r = (int)(r * factor);
  g = (int)(g * factor);
  b = (int)(b * factor);
      for(int n=0; n<num_leds; n++){
        RGBpayload[n * 3 + 0] = (byte)r;
        RGBpayload[n * 3 + 1] = (byte)g;
        RGBpayload[n * 3 + 2] = (byte)b;
      }
      if (mqttClient.connected()) {
        mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
      }
  Serial.print("temperature: ");
  Serial.print(temp);
  Serial.print("°C, humidity: ");
  Serial.print(humi);
  Serial.print("%, shade factor: ");
  Serial.print(factor, 2);
  Serial.print(", RGB: ");
  Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
    } else {
      Serial.println("The switch is off, and the lights remain in their current state.");
    }
    delay(1000);
  }
}

// Function to update the R, G, B values of a single LED pixel
// RGB can a value between 0-254, pixel is 0-71 for a 72 neopixel strip
void send_RGB_to_pixel(int r, int g, int b, int pixel) {
  // Check if the mqttClient is connected before publishing
  if (mqttClient.connected()) {
    // Update the byte array with the specified RGB color pattern
    RGBpayload[pixel * 3 + 0] = (byte)r; // Red
    RGBpayload[pixel * 3 + 1] = (byte)g; // Green
    RGBpayload[pixel * 3 + 2] = (byte)b; // Blue

    // Publish the byte array
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    
    Serial.println("Published whole byte array after updating a single pixel.");
  } else {
    Serial.println("MQTT mqttClient not connected, cannot publish from *send_RGB_to_pixel*.");
  }
}

void send_all_off() {
  // Check if the mqttClient is connected before publishing
  if (mqttClient.connected()) {
    // Fill the byte array with the specified RGB color pattern
    for(int pixel=0; pixel < num_leds; pixel++){
      RGBpayload[pixel * 3 + 0] = (byte)0; // Red
      RGBpayload[pixel * 3 + 1] = (byte)0; // Green
      RGBpayload[pixel * 3 + 2] = (byte)0; // Blue
    }
    // Publish the byte array
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    
    Serial.println("Published an all zero (off) byte array.");
  } else {
    Serial.println("MQTT mqttClient not connected, cannot publish from *send_all_off*.");
  }
}

void send_all_random() {
  // Check if the mqttClient is connected before publishing
  if (mqttClient.connected()) {
    // Fill the byte array with the specified RGB color pattern
    for(int pixel=0; pixel < num_leds; pixel++){
      RGBpayload[pixel * 3 + 0] = (byte)random(50,256); // Red - 256 is exclusive, so it goes up to 255
      RGBpayload[pixel * 3 + 1] = (byte)random(50,256); // Green
      RGBpayload[pixel * 3 + 2] = (byte)random(50,256); // Blue
    }
    // Publish the byte array
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    
    Serial.println("Published an all random byte array.");
  } else {
    Serial.println("MQTT mqttClient not connected, cannot publish from *send_all_random*.");
  }
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}



