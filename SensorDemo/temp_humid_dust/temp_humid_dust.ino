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
#define DHT11_PIN 4

//-------- Customise these values -----------
const char* ssid = "maple";
const char* password = "12345678";

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

// --------Pmdefine-----
int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
// -------temphunid define----
float humid,temp,pm;
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
 pinMode(ledPower,OUTPUT);
}

void loop() {
  // ------getpm -----
   digitalWrite(ledPower,LOW); // power on the LED
   delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
   digitalWrite(ledPower,HIGH); // turn the LED off
   delayMicroseconds(sleepTime);
   
  // 0 - 5V mapped to 0 - 1023 integer values
   // recover voltage
   calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
   // Chris Nafis (c) 2012
   dustDensity = 0.17 * calcVoltage - 0.01;
   if(dustDensity<0)
    dustDensity = 0;
   
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
  getTemHum();
  pm = getPM();
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"Temperature\": ";
  data+=temp ;
  data+=",\"Humidity\": ";
  data+=humid ;
  data+=",\"PM25\": ";
  data+=pm ;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}
void getTemHum(){
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
  humid = (float)DHT11.humidity;
  temp = (float)DHT11.temperature;
  }
float getPM(){
   float finDensity = dustDensity*1000;
   float outputPM,Clow,Chigh,Ilow,Ihigh;

   if(0<finDensity && finDensity<= 15.4){
    Clow = 0;
    Chigh = 15.4;
    Ilow = 0;
    Ihigh = 50;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量良好 ");
    }
   else if(15.5<finDensity && finDensity<=40.4){
    Clow = 15.5;
    Chigh = 40.4;
    Ilow = 51;
    Ihigh = 100;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量中等 ");
    }
   else if(40.5<finDensity && finDensity<=65.4){
    Clow = 40.5;
    Chigh = 65.4;
    Ilow = 101;
    Ihigh = 150;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量对敏感人群不健康 ");
    }
   else if(65.5 < finDensity && finDensity <= 150.4){
    Clow = 65.5;
    Chigh = 150.4;
    Ilow = 151;
    Ihigh = 200;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量不健康 ");
    }
   else if(150.5 < finDensity && finDensity <= 250.4){
    Clow = 150.5;
    Chigh = 250.4;
    Ilow = 201;
    Ihigh = 300;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量非常不健康 ");
    }
   else if(250.5 < finDensity && finDensity <= 350.4){
    Clow = 250.5;
    Chigh = 350.4;
    Ilow = 301;
    Ihigh = 400;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量有毒害 ");
    }
   else if(350.5 < finDensity && finDensity <= 500.4){
    Clow = 350.5;
    Chigh = 500.4;
    Ilow = 401;
    Ihigh = 500;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" 空气质量有毒害 ");
    }
    else if(500.5 < finDensity){
    Clow = 500.5;
    Chigh = 700.4;
    Ilow = 501;
    Ihigh = 600;
    outputPM = ((Ihigh -Ilow)/(Chigh-Clow))*(finDensity - Clow)+Ilow;
    Serial.print(" PM已经突破天际 ");
    }
  Serial.print(" PM2.5 = ");
  Serial.println(outputPM);
  return outputPM;
  }
