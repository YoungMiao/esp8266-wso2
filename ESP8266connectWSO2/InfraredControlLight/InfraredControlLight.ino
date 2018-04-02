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
#include <dht11.h>
#include "Base64.h"
#include <ArduinoJson.h>
#define DHT11_PIN 4
#include <NTPClient.h>
#define redPIN 5
#define Sensor 16

#define grePIN 14

dht11 DHT11;


const char* ssid = "wulianwang";
const char* password = "12345678";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* type ="ESP8266";
const char* deviceId ="ESP8266-001";
const char* mqtt_server= "192.168.200.136";
const int mqtt_port = 1886;
const char* httpGateway= "http://192.168.200.136:8280";

char user[]="admin";
char passwd[]="admin";
char clientId[100];
char clientSecret[100];
char publishTopic[100] ;
char subscribeTopic[100]; 
char json[512]={};
char accessToken[100];
char refreshToken[100];
char base64_user[12];
char base64_client[57];
char authorizationClient[200];
bool LEDstatus=false;

unsigned long currentTime;
unsigned long Time_Echo_us = 0;
//Len_mm_X100 = length*100 
unsigned long Len_mm_X100  = 0;  
unsigned long Len_Integer = 0; // 
unsigned int Len_Fraction = 0;

StaticJsonBuffer<200> jsonBuffer;

void setup_wifi() {
  delay(100);
  WiFi.begin(ssid, password);
  Serial.print("connectint WIFI...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  timeClient.begin();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  String data=(char*)payload;
  Serial.print(data);
  char *a="ON";
  char *b="OFF";
  if(strstr(data.c_str(),a) != NULL){
    digitalWrite(redPIN,HIGH);
    LEDstatus=true;
    }
  if(strstr(data.c_str(),b) != NULL){
    digitalWrite(redPIN,LOW);
    LEDstatus=false;
    }
}

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

void registerapi(){
  snprintf (base64_user, 12,"%s:%s", user,passwd);
  int inputStringLength = sizeof(base64_user);
  int encodedLength = Base64.encodedLength(inputStringLength);
  char encodedString[encodedLength];
  Base64.encode(encodedString, base64_user, inputStringLength);
  
  HTTPClient http;    //Declare object of class HTTPClient
  char tokenEP[100];
  snprintf (tokenEP, 100, "%s/api-application-registration/register", httpGateway);
  //Serial.println(tokenEP);
  http.begin(tokenEP);      //Specify request destination
  http.addHeader("content-type", "application/json");  //Specify content-type header
  char authorization[100];
  snprintf (authorization, 100, "Basic %s", encodedString);
  http.addHeader("Authorization",authorization);
 
  char payload[200];
  snprintf (payload, 200, "{ \"applicationName\":\"DefaultApplication\", \"isAllowedToAllDomains\":false, \"tags\":[\"device_agent\"]}");
  //Serial.println(payload);
  int httpCode = http.POST(payload);   //Send the request
  String response = http.getString();  //Get the response payload
  http.end();  //Close connection

  char json[500];
  strcpy(json,response.c_str());
  JsonObject& root = jsonBuffer.parseObject(json);
  strcpy(clientId,root["client_id"]);
  strcpy(clientSecret,root["client_secret"]);
  }
void registerClient(){
  snprintf (base64_client,58,"%s:%s", clientId,clientSecret);
  int inputStringLength = sizeof(base64_client);
  int encodedLength = Base64.encodedLength(inputStringLength);
  char encodedString[encodedLength];
  Base64.encode(encodedString, base64_client, inputStringLength);
  HTTPClient http;
  char tokenTP[100];
  snprintf (tokenTP, 100, "%s/token?",httpGateway);
  Serial.println(tokenTP);
  http.begin(tokenTP);      //Specify request destination
  http.addHeader("Content-Type","application/x-www-form-urlencoded");  //Specify content-type header
  snprintf (authorizationClient, 200, "Basic %s", encodedString);
  http.addHeader("Authorization",authorizationClient);
  char param[512];
  snprintf (param, 512,"grant_type=password&username=%s&password=%s&scope=perm:device:enroll perm:device:disenroll perm:device:modify perm:device:operations perm:device:publish-event", user,passwd);
  int res = http.POST(param);
  String responseToken = http.getString();  //Get the response payload
  Serial.println(res);
  Serial.println(responseToken);
  http.end();  //Close connection
  char json[500];
  strcpy(json,responseToken.c_str());
  JsonObject& root = jsonBuffer.parseObject(json);
  strcpy(accessToken,root["access_token"]);
  strcpy(refreshToken,root["refresh_token"]);
  }
void registerdevice(){
  HTTPClient http;    //Declare object of class HTTPClient
  char tokenDE[100];
  snprintf (tokenDE, 100, "%s/api/device-mgt/v1.0/device/agent/enroll", httpGateway);
  http.begin(tokenDE);      //Specify request destination
  http.addHeader("accept", "application/json");  //Specify content-type header
  char accessTokenClient[100];
  snprintf (accessTokenClient, 100, "Bearer %s", accessToken);
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
  pinMode(redPIN, OUTPUT);
  pinMode(grePIN, OUTPUT);
  pinMode(Sensor, INPUT);

  digitalWrite(redPIN,HIGH);
  digitalWrite(grePIN,LOW);
  LEDstatus=true;
  Serial.begin(115200);
  delay(1000);
  registerapi();
  registerClient();
  //registerdevice();
  if (client.connect(deviceId, accessToken, "")) {
    Serial.println("MQTT Client Connected");
    } 
   else{
      Serial.println("Error while connecting with MQTT server.");
   }  
}

void loop() { 
   if(!client.connected()) {
      reconnect();
      return;
   }
    client.loop();
  // Publish the data.
  String strdata = buildStr();
  snprintf (publishTopic, 100, "carbon.super/%s/%s/events",type,deviceId);
  snprintf (subscribeTopic, 100, "carbon.super/%s/%s/operation/#",type,deviceId);
  boolean pubresult = client.publish(publishTopic,strdata.c_str());
   Serial.print("attempt to send ");
   Serial.println(strdata);
   Serial.print("to ");
   Serial.println(publishTopic);
   //client.subscribe(subscribeTopic);
   if (pubresult)
     Serial.println("successfully sent");
   else
     Serial.println("unsuccessfully sent");
   Serial.println();
   //digitalWrite(ledPower,HIGH); // turn the LED off

    int SensorState = digitalRead(Sensor);
    Serial.print("SensorState:");
    Serial.println(SensorState);
    if(SensorState==1){
      digitalWrite(grePIN,HIGH);
      }
    else{
      digitalWrite(grePIN,LOW);
      }
    delay(200); 
    
}
void getrefreshToken(){
  HTTPClient http;    //Declare object of class HTTPClient
  char tokenEP[100];
  snprintf (tokenEP, 100, "%s/token", httpGateway);
  Serial.println(tokenEP);
  http.begin(tokenEP);      //Specify request destination
  http.addHeader("content-type", "application/x-www-form-urlencoded");  //Specify content-type header
  http.addHeader("Authorization", authorizationClient);

  char payload[200];
  snprintf (payload, 200, "grant_type=refresh_token&refresh_token=%s", refreshToken);
  Serial.println(payload);
  int httpCode = http.POST(payload);   //Send the request
  String response = http.getString();  //Get the response payload
  strcpy(json,response.c_str());
  http.end();  //Close connection
  JsonObject& root = jsonBuffer.parseObject(json);
  strcpy(accessToken,root["access_token"]);
  strcpy(refreshToken,root["refresh_token"]);
  } 
void reconnect() {
  // Loop until we're reconnected
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  getrefreshToken();
  if (client.connect(deviceId, accessToken, "")) {
    Serial.println("MQTT Client Connected again");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    delay(1000);
  }
}

String buildStr() {
  int chk = DHT11.read(DHT11_PIN);
   Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println(""); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println(""); 
                break;
    default: 
                Serial.println(""); 
                break;
  }
  int humid = (int)DHT11.humidity;
  timeClient.update();
  int temp = (int)DHT11.temperature;
  currentTime=timeClient.getEpochTime();
  char data[200];
  if(LEDstatus){
    snprintf (data, 200, "{\"temperature\":%d,\"humidity\":%d,\"timestamp\":%ld,\"status\":true}",temp,humid,currentTime);
    }
   else{
    snprintf (data, 200, "{\"temperature\":%d,\"humidity\":%d,\"timestamp\":%ld,\"status\":false}",temp,humid,currentTime);
    }
  
  return data;
}
