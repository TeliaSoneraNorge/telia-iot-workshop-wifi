#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <ATT_IOT.h>
#include <ArduinoJson.h>

#define DHTPIN D7     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define http "api.allthingstalk.io"  // API endpoint
#define mqtt "api.allthingstalk.io"  // broker

const char deviceId[] = "XXXXXX";
const char deviceToken[] = "maker:XXXXXXXXXXXXXX";

void callback(char* topic, byte* payload, unsigned int length);
WiFiClient espClient;
PubSubClient pubSub(mqtt, 1883, callback, espClient);

ATTDevice device(deviceId, deviceToken);

Adafruit_SSD1306 display(-1);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  // Clear the buffer.
  display.clearDisplay();
  dht.begin();
  setupWiFi("telia1", "workshop");
  
  // Connect to AllThingsTalk
  while(!device.connect(&espClient, http)){
    Serial.println("retrying");
  }

  // Create the assets in case they do not exist
  device.addAsset("temperature", "temperature", "Current temperature", "sensor", "{\"type\": \"number\"}");
  device.addAsset("humidity", "humidity", "Current humidity", "sensor", "{\"type\": \"number\"}");
  
  // Subscribe to mqtt 
  while(!device.subscribe(pubSub)){
    Serial.println("retrying");
  }
}

void setupWiFi(const char* ssid, const char* password)
{
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

unsigned long prevTime;
unsigned int prevVal = 0;
int counter = 0;

void loop() {
  
  unsigned long curTime = millis();
  if (curTime > (prevTime + 1000))  // Update and send counter value every 5 seconds
  {
    device.send(String(dht.readTemperature()), "temperature");
    device.send(String(dht.readHumidity()), "humidity");
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println(String(dht.readTemperature()) + " C");
    display.println(String(dht.readHumidity()) + " %");
    display.display();
    
    counter++;
    prevTime = curTime;
  }
  device.process();  // Check for incoming messages
}

void callback(char* topic, byte* payload, unsigned int length) 
{ 
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Convert payload to json
  StaticJsonBuffer<500> jsonBuffer;
  char json[500];
  for (int i = 0; i < length; i++) {
    json[i] = (char)payload[i];
  }
  json[length] = '\0';
  
  JsonObject& root = jsonBuffer.parseObject(json);

  // Do something
  if(root.success())
  {
    const char* value = root["value"];
    Serial.println(value);
    if (strcmp(value,"true") == 0)
      digitalWrite(LED_BUILTIN, LOW);   // Turn the onboard LED on
    else
      digitalWrite(LED_BUILTIN, HIGH);  // Turn the onboard LED off

    device.send(value, "toggle");  // Send command back as ACK using JSON
  }
  else
    Serial.println("Parsing JSON failed");
}
