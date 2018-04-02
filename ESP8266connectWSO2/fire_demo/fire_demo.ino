/**
* Copyright (c) 2017, WSO2 Inc. (http://www.wso2.org) All Rights Reserved.
*
* WSO2 Inc. licenses this file to you under the Apache License,
* Version 2.0 (the "License"); you may not use this file except
* in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied. See the License for the
* specific language governing permissions and limitations
* under the License.
**/
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "Base64.h"
#include <ArduinoJson.h>
#define fire_PIN 0


const char* ssid = "wulianwang";
const char* password = "12345678";



const char* type ="test";
const char* deviceId ="test001";

const char* mqtt_server= "192.168.7.51";
const int mqtt_port = 1886;
const char* httpGateway= "http://192.168.7.51:8280";


String user="admin";
String passwd="admin";
String clientId;
String clientSecret;

String publishTopic;
String subscribeTopic; 
String accessToken;
String refreshToken;
String authorizationClient;

unsigned long interval = 0;
unsigned long timeinterval = 0;

DynamicJsonBuffer jsonBuffer;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
}

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

void registerapi(){
  String base64user = user + ":" + passwd;
  int inputStringLength = base64user.length();
  int encodedLength = Base64.encodedLength(inputStringLength);

  char base64up[inputStringLength +1];
  snprintf (base64up, inputStringLength +1, base64user.c_str(), inputStringLength);  
  char encodedString[encodedLength];
  Base64.encode(encodedString, base64up, inputStringLength);
  
  HTTPClient http;    //Declare object of class HTTPClient
  char tokenEP[100];
  snprintf (tokenEP, 100, "%s/api-application-registration/register", httpGateway);
  http.begin(tokenEP);      //Specify request destination
  http.addHeader("content-type", "application/json");  //Specify content-type header
  String authorization;
  authorization = "Basic " + String(encodedString);
  http.addHeader("Authorization",authorization.c_str());
 
  char payload[200];
  snprintf (payload, 200, "{ \"applicationName\":\"DefaultApplication\", \"isAllowedToAllDomains\":false, \"tags\":[\"device_agent\"]}");
  //Serial.println(payload);
  int httpCode = http.POST(payload);   //Send the request
  String response = http.getString();  //Get the response payload
  http.end();  //Close connection

  JsonObject& root = jsonBuffer.parseObject(response);
  String clitId = root["client_id"];
  String clitSecret = root["client_secret"];
  clientId = clitId;
  clientSecret = clitSecret;
  }
void registerClient(){
  String base64_client;
  base64_client = clientId + ":" + clientSecret;
  int inputStringLength = base64_client.length();
  int encodedLength = Base64.encodedLength(inputStringLength);
  char encodedString[encodedLength];
  Base64.encode(encodedString, base64_client.c_str(), inputStringLength);
  HTTPClient http;
  char tokenTP[100];
  snprintf (tokenTP, 100, "%s/token?",httpGateway);
  Serial.println(tokenTP);
  http.begin(tokenTP);      //Specify request destination
  http.addHeader("Content-Type","application/x-www-form-urlencoded");  //Specify content-type header
  authorizationClient = "Basic " + String(encodedString);
  Serial.println(authorizationClient);
  http.addHeader("Authorization",authorizationClient);
  String params = "grant_type=password&username="+ user +"&password="+ passwd +"&scope=perm:device:enroll perm:device:disenroll perm:device:modify perm:device:operations perm:device:publish-event";
  int res = http.POST(params);
  String responseToken = http.getString();  //Get the response payload
  http.end();  //Close connection

  JsonObject& root = jsonBuffer.parseObject(responseToken);
  if (!root.success()) {
    return ;
  }
  String acTkn = root["access_token"];
  accessToken = acTkn;
  String reTkn = root["refresh_token"];
  refreshToken = reTkn;
  }
void registerdevice(){
  HTTPClient http;    //Declare object of class HTTPClient
  char tokenDE[100];
  snprintf (tokenDE, 100, "%s/api/device-mgt/v1.0/device/agent/enroll", httpGateway);
  http.begin(tokenDE);      //Specify request destination
  http.addHeader("accept", "application/json");  //Specify content-type header
  String accessTokenClient;
  accessTokenClient = "Bearer " + accessToken;
  http.addHeader("Authorization",accessTokenClient);
  Serial.print("Authorization:");
  Serial.println(accessTokenClient);
  http.addHeader("content-type", "application/json");
  char payload[200];
  snprintf (payload, 200, "{ \"name\": \"devicename\", \"type\": \"%s\", \"description\": \"descritption\", \"deviceIdentifier\": \"%s\", \"enrolmentInfo\": {\"ownership\": \"BYOD\", \"status\": \"ACTIVE\"} ,\"properties\": [{\"name\": \"propertyName\",\"value\": \"propertyValue\"}]}",type,deviceId);
  int dev = http.POST(payload);
  String responseDevice= http.getString();  //Get the response payload
  Serial.println(dev);
  Serial.println(responseDevice);
  }

void setup() {
  setup_wifi();
  Serial.begin(115200);
  delay(15000);
  registerapi();
  registerClient();
  //registerdevice();
  if (client.connect(deviceId, accessToken.c_str(), "")) {
    Serial.println("MQTT Client Connected");
    } 
   else{
      Serial.println("Error while connecting with MQTT server.");
   }
   pinMode(fire_PIN, INPUT);  
}

void loop() { 
   if(!client.connected()) {
      reconnect();
      return;
   }
   Serial.println("loop.......................................");
    client.loop();
  // Publish the data.
  String strdata = buildStr();
  publishTopic = "carbon.super/" + String(type) +"/" + String(deviceId) + "/events";
  subscribeTopic = "carbon.super/" + String(type) +"/" + String(deviceId) + "/command";
  boolean pubresult = client.publish(publishTopic.c_str(),strdata.c_str());
   Serial.print("attempt to send ");
   Serial.println(strdata);
   Serial.print("to ");
   Serial.println(publishTopic);
}
void getrefreshToken(){
  HTTPClient http;    //Declare object of class HTTPClient
  char tokenEP[100];
  snprintf (tokenEP, 100, "%s/token", httpGateway);
  http.begin(tokenEP);      //Specify request destination
  http.addHeader("content-type", "application/x-www-form-urlencoded");  //Specify content-type header
  http.addHeader("Authorization", authorizationClient);

  String payload;
  payload = "grant_type=refresh_token&refresh_token=" + refreshToken;
  int httpCode = http.POST(payload.c_str());   //Send the request
  String response = http.getString();  //Get the response payload
  http.end();  //Close connection
  Serial.println(httpCode);
  
  JsonObject& root = jsonBuffer.parseObject(response);
  String acTkn = root["access_token"];
  accessToken = acTkn;
  String reTkn = root["refresh_token"];
  refreshToken = reTkn;
  } 
void reconnect() {
  // Loop until we're reconnected
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  getrefreshToken();
  if (client.connect(deviceId, accessToken.c_str(), "")) {
    Serial.println("MQTT Client Connected again");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    delay(1000);
  }
}

String buildStr() {
  int value = analogRead(fire_PIN);                //从引脚0输入的值赋给变量value
  char data[200];
  long temp = 100;
  snprintf (data, 200, "{\"fire\":%d,\"temp\":%ld}", value,temp);
  //snprintf (data, 200, "{\"temperature\":%d}", value);
  //snprintf (data, 200, "{\"temp\":%f}", (float)value);
    Serial.println(data); 
  return data;
}
