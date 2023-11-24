#include "WS0010.h"
WS0010 oled;

void setup() {
  oled.init();
  oled.print("Hoşgeldiniz...½");
  oled.moveCursor(1,0);
  oled.print("Lütfen bekleyin!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
