int analogPin = A0;                           //水位传感器连接到模拟口 0
int led = 12;                                 //报警LED灯连接到数字口 12
int val = 0;                                  //定 义变量 val 初值为 0
int data = 0;                                 //定义变量 data 初值为 0
void setup()
{
pinMode(led, OUTPUT);                 //定义 led 为输出引脚
Serial.begin(9600);                       //设定波特率为 9600
}
void loop()
{
val = analogRead(analogPin);             //读取模拟值送给变量 val
if( val>300){                             //判断变量 val 是否大于 600
digitalWrite(led,HIGH);    //变量 val 大于 600 时，点亮报警LED灯
}
else{
digitalWrite(led,LOW);     //变量 val 小于 600 时，熄灭报警LED灯
}
data = val;                                 //变量 val 赋值给变 量 data
Serial.println(data);                         //串口打印变量 data
delay(100);
}
