int Sensor= 2;
void setup() {
   Serial.begin(9600);
   pinMode(Sensor, INPUT);
}
void loop() {
   int SensorState = digitalRead(Sensor);
   if (SensorState == 0){
      Serial.println("在检测范围内木有人！！！");
    }
   Serial.println(SensorState);
   delay(500);      
}
