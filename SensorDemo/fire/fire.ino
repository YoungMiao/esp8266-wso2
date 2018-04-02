void setup()
{
  Serial.begin(9600);               
  pinMode(8, OUTPUT);                 //引脚8用于控制红色LED灯闪烁
  pinMode(9, OUTPUT);                //引脚9用于控制蜂鸣器报警
  pinMode(14, INPUT);                  //输入引脚14，作为检测远红外火焰传感器的变化
}

void loop()
{
  int i=analogRead(14);                //从引脚14输入的值赋给变量i       
  if(i>850)                        //无火焰时i=0，但日光灯及其他可能的远红外线干扰将会使其不等于0                                    //将火焰传感器的门限值设为i=200（可以根据需要自调）
  {
  analogWrite(9,240);
  analogWrite(8,255);
  delay(500);
  analogWrite(9,100);
  analogWrite(8,100);
  delay(500) ;                        //蜂鸣器高低音调鸣响，红色LED高低亮度闪烁
  Serial.println(i);                //串口监视器查看火焰传感器数值
  }
  else                          //未满足火焰传感器门限值
  {
    analogWrite(9,0);
    analogWrite(8,0);                //蜂鸣器停止鸣响，红色LED小灯熄灭
    Serial.println(i);    //监视器查看数值
    delay(500);
   }
}
