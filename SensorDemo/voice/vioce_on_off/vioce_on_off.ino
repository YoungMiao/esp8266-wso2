//-----------voice-sensor------------

int sensorVoice = 2;
int sensorJidianqi = 13; //
void setup()
{
  pinMode(sensorJidianqi, OUTPUT);
  pinMode(sensorVoice, INPUT);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(sensorVoice) ==0)
  {
    delay(100);
    if (digitalRead(sensorVoice) ==0)
    {
      digitalWrite(sensorJidianqi, HIGH); //LOW to power on JiDianQi
      delay(4000);
      digitalWrite(sensorJidianqi, LOW); //
    }
  }
  else
  {
    digitalWrite(sensorJidianqi, LOW);
  }
}
