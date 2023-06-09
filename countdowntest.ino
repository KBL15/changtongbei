#include <U8g2lib.h>
#include <Bounce2.h>

#define BUTTON_PIN 2
#define LONG_PRESS_TIME 1000
#define COUNTDOWN_TIME_MAX 60
#define COUNTDOWN_TIME_MIN 1

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Bounce button = Bounce();

void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.attach(BUTTON_PIN);
  button.interval(10);
}

void loop() {
  static bool countdown_running = false;
  static bool button_pressed = false;
  static unsigned long last_button_press = 0;
  static int countdown_time = COUNTDOWN_TIME_MIN;

  unsigned long current_time = millis();

  button.update();

  if (countdown_running) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.setCursor(0, 20);
    u8g2.print("Countdown:");
    u8g2.setCursor(0, 40);
    u8g2.print(countdown_time);
    u8g2.drawStr(60, 40, "s");
    u8g2.sendBuffer();

    if (current_time - last_button_press < LONG_PRESS_TIME) {
      // 按键按下时间不足长按阈值，增加倒计时时间
      if (button.fell()) {
        countdown_time++;
        if (countdown_time > COUNTDOWN_TIME_MAX) {
          countdown_time = COUNTDOWN_TIME_MIN;
        }
        last_button_press = current_time;
      }
    } else {
      // 长按按键停止倒计时
      if (button.read() == HIGH) {
        countdown_running = false;
        button_pressed = false;
        u8g2.clearBuffer();
        u8g2.sendBuffer();
      }
    }

    // 倒计时结束
    if (countdown_time == 0) {
      countdown_running = false;
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_profont12_tf);
      u8g2.drawStr(0, 20, "Countdown finished!");
      u8g2.drawStr(0, 40, "Press to restart");
      u8g2.sendBuffer();

      // 等待按键重新开始倒计时
      while (!button_pressed) {
        if (button.read() == HIGH) {
          button_pressed = true;
          last_button_press = current_time;
          countdown_time = COUNTDOWN_TIME_MIN;
          break;
        }
        delay(50);
      }

      // 清空显示屏
      u8g2.clearBuffer();
      u8g2.sendBuffer();
    } else {
      // 更新倒计时时间
      countdown_time = COUNTDOWN_TIME_MAX - (current_time - last_button_press) / 1000;
    }
  }

  // 检测按键是否被释放
  if (button.rose() && button_pressed) {
    button_pressed = false;
    if(countdown_time > 0) {
// 开始倒计时
countdown_running = true;
last_button_press = current_time;
}
}

if (!countdown_running) {
// 显示输入倒计时时间
u8g2.clearBuffer();
u8g2.setFont(u8g2_font_profont12_tf);
u8g2.drawStr(0, 20, "Set countdown:");
u8g2.setCursor(0, 40);
u8g2.print(countdown_time);
u8g2.drawStr(60, 40, "s");
u8g2.sendBuffer();
// 等待输入倒计时时间
if (button.fell()) {
  last_button_press = current_time;
}
if (button.rose()) {
  if (current_time - last_button_press >= LONG_PRESS_TIME) {
    // 长按按键设置最大倒计时时间
    countdown_time = COUNTDOWN_TIME_MAX;
  } else {
    // 短按按键增加倒计时时间
    countdown_time++;
    if (countdown_time > COUNTDOWN_TIME_MAX) {
      countdown_time = COUNTDOWN_TIME_MIN;
    }
  }
  last_button_press = current_time;
}
}
}