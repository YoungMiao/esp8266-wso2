/**
 * Helloworld style, connect an ESP8266 to the IBM IoT Foundation
 * 
 * Author: Ant Elder
 * License: Apache License v2
 */
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>

//DS18B20 on GPIO2
OneWire  ds(2);
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
 Serial.println("WiFi connected");  
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());

 Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void loop() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  

//Loop through all DS1820

while(ds.search(addr))
{ 
  Serial.print("ROM =");

//Topic is built from a static String plus the ID of the DS18B20
  String romcode = "temp/";
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
    romcode = romcode + String(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
// the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }

//convert RAW Temperature to String

char topicStr[26];
 topicName.toCharArray(topicStr,26);
String raw_temp = String(raw, DEC);
Serial.println(raw_temp);
  if (client.connected()){
    Serial.print("Sending payload: ");
    Serial.println(raw_temp);
  }
    if (client.publish(topicStr,(char*) raw_temp.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }

  delay(5000);
}
//End of the OneWire-Devices, reset Loop
Serial.println("End of Onewire Bus");
ds.reset_search();
return;
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
