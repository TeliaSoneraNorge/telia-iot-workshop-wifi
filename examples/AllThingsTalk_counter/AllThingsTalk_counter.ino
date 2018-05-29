/*    _   _ _ _____ _    _              _____     _ _     ___ ___  _  __
 *   /_\ | | |_   _| |_ (_)_ _  __ _ __|_   _|_ _| | |__ / __|   \| |/ /
 *  / _ \| | | | | | ' \| | ' \/ _` (_-< | |/ _` | | / / \__ \ |) | ' <
 * /_/ \_\_|_| |_| |_||_|_|_||_\__, /__/ |_|\__,_|_|_\_\ |___/___/|_|\_\
 *                             |___/
 *
 * Copyright 2017 AllThingsTalk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <ATT_IOT.h>

// AllThingsTalk device credentials
char deviceId[] = "XXXXXX";
char token[] = "maker:XXXXXXXXXXXXXX";

// Wifi settings
const char* ssid = "telia1";
const char* password = "workshop";

// http and mqtt endpoints
#define httpServer "api.allthingstalk.io"
#define mqttServer "api.allthingstalk.io"

ATTDevice device(deviceId, token);

//required for the device
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient espClient;
PubSubClient pubSub(mqttServer, 1883, callback, espClient);

void setup()
{
  // Start serial link for debugging
  Serial.begin(9600);
  
  setup_wifi();

  // Connect the device with AllThingsTalk
  while(!device.connect(&espClient, httpServer)) {
    Serial.println("retrying");
  }

  // Create the counter asset in case it does not exist
  device.addAsset("counter", "counter", "counting up", "sensor", "{\"type\": \"integer\"}");

  // Make sure that we can receive messages over mqtt
  while(!device.subscribe(pubSub)){
    Serial.println("retrying");
  }
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

unsigned long oldTime;
unsigned int prevVal = 0;
int counter = 0;

void loop()
{
  unsigned long curTime = millis();

  // Update and send counter value every 3 seconds
  if (curTime > (oldTime + 3000))
  {
    counter++;
    device.send(String(counter), "counter");
    oldTime = curTime;
  }
  device.process(); 
}

// Callback function
// Handle messages that were sent from the AllThingsTalk cloud to this device
void callback(char* topic, byte* payload, unsigned int length) 
{ 
  String assetName = device.getAssetName(topic, strlen(topic));
  Serial.print("Data arrived from asset: ");
  Serial.println(assetName);
}
