void neopixel_white() {
  strip.setLedColorData(1, 255, 255, 255);
  strip.show();
}
void neopixel_red() {
  strip.setLedColorData(1, 255, 0, 0);
  strip.show();
}
void neopixel_green() {
  strip.setLedColorData(1, 0, 255, 0);
  strip.show();
}

void neopixel_blue() {
  strip.setLedColorData(1, 0, 0, 255);
  strip.show();
}
void neopixel_sleep() {
  strip.setLedColorData(1, 0, 0, 0);
  strip.show();
}
