#include <Bounce2.h> //导入Bounce库

#define BUTTON_PIN 2
#define LED_PIN 9

Bounce debouncer = Bounce(); //创建Bounce对象

int ledBrightness = 0; //默认亮度为0

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  debouncer.attach(BUTTON_PIN); //将Bounce对象绑定到按键引脚
  debouncer.interval(20); //设置消抖时间为20毫秒
}

void loop() {
  debouncer.update(); //更新Bounce对象状态
  if (debouncer.fell()) { //按键按下时
    ledBrightness += 85; //增加亮度等级
    if (ledBrightness > 255) { //亮度等级循环
      ledBrightness = 0;
    }
    Serial.print(ledBrightness); //设置LED亮度
  }
}
