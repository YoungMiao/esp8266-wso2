/**
 * Helloworld style, connect an ESP8266 to the IBM IoT Foundation
 * 
 * Author: Ant Elder
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <SPI.h>
#include <dht11.h>

dht11 DHT11;
#define DHT11_PIN 2

//-------- Customise these values -----------
const char* ssid = "maple";
const char* password = "12345678";
/*
#define ORG "x8kbk8"
#define DEVICE_TYPE "ESP8266"
#define DEVICE_ID "Test3"
#define TOKEN "@maple123456"
*/
#define ORG "dyob0d"
#define DEVICE_TYPE "ESP8266"
#define DEVICE_ID "Test3"
#define TOKEN "@maple123456"
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
//char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
String buildClientName();
String clientName = buildClientName();
String topicName = String("iot-2/evt/status/fmt/json"); // Topic

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
 Serial.begin(115200);
 Serial.println();

 Serial.print("Connecting to "); Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
 } 
 Serial.println("");

 Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void loop() {
  char clientStr[34];
  clientName.toCharArray(clientStr,34);
  char topicStr[26];
  topicName.toCharArray(topicStr,26);
 
  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
   }
  }
   Serial.println();
   // Build the JSON data to publish.
   String json = buildJson();
   char jsonStr[200];
   json.toCharArray(jsonStr,200);

   // Publish the data.
   boolean pubresult = client.publish(topicStr,jsonStr);
   Serial.print("attempt to send ");
   Serial.println(jsonStr);
   Serial.print("to ");
   Serial.println(topicStr);

   if (pubresult)
     Serial.println("successfully sent");
   else
     Serial.println("unsuccessfully sent");
   Serial.println();
   delay(5000);  
}
// Builds the clientName
String buildClientName (){
  String data = "";
  data+="d:";
  data+=ORG;
  data+=":";
  data+=DEVICE_TYPE;
  data+=":";
  data+="Test3";
  return data;
}

// Sends the string: {d:{"temperature" : "27.8"}} to Bluemix
String buildJson() {
  int chk = DHT11.read(DHT11_PIN);
   Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }
  float humid = (float)DHT11.humidity;
  float temp = (float)DHT11.temperature;
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"Temperature\": ";
  data+=temp ;
  data+=",\"Humidity\": ";
  data+=humid ;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}
