/*
  Standalone Sketch to use with a Arduino UNO and a
  Sharp Optical Dust Sensor GP2Y1010AU0F
*/

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup(){
   Serial.begin(9600);
   pinMode(ledPower,OUTPUT);
}

void loop(){
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
   
   //mg/m3 to ug/m3
   dustDensity = max(0,dustDensity);
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
  Serial.println(outputPM); // unit: mg/m3

  delay(1000);
}
