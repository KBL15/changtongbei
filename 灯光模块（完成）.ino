#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800); // 初始化LED灯带对象
Bounce debouncer = Bounce(); // 初始化Bounce对象
int button = 2; // 按钮引脚
int brightness = 0; // 亮度初始值

void setup() {
  pinMode(button, INPUT_PULLUP); // 设置按钮引脚为输入并开启内置上拉电阻

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  debouncer.attach(button); // 将Bounce对象绑定到按键引脚
  debouncer.interval(20); // 设置消抖时间为20毫秒

  Serial.begin(9600); // 启动串口通信
  strip.begin(); // 初始化LED灯带

  for (int i = 0; i < strip.numPixels(); i++) { // 遍历LED灯带中的每一个像素点
    strip.setPixelColor(i, strip.Color(250, 249, 222)); // 设置像素点颜色为白色
  }

  strip.show(); // 将所有像素点颜色刷新到LED灯带上
  strip.setBrightness(brightness);
  
  Serial.println("READY"); // 打印提示信息
  delay(2000); // 延时2秒
}

void loop() {
  debouncer.update(); // 更新Bounce对象状态
  delay(50); // 延时50毫秒

  if (debouncer.fell()) { // 如果按钮按下
    brightness += 50; // 将亮度加上85
    if (brightness > 200) { // 如果亮度大于255，亮度等级循环
      brightness = 0;
    }
    for (int i = 0; i < strip.numPixels(); i++) { // 遍历LED灯带中的每一个像素点
      strip.setPixelColor(i, strip.Color(250, 249, 222)); // 设置像素点颜色为白色
      strip.setBrightness(brightness); // 设置像素点亮度
    }
    strip.show(); // 将所有像素点颜色刷新到LED灯带上
    Serial.println(brightness); // 打印当前亮度值
  }
}
