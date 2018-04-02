int sensorVoice = 2;
void setup()
{
  pinMode(sensorVoice, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int temp = digitalRead(sensorVoice);
  Serial.println(temp);
  delay(200);
  
}
