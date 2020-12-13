// VUSB_Mouse moves the cursor in a rectangle
// for compatability with other libraries you may use VUSB_Mouse.move(X, Y, scroll, buttons)

#include <VUSB_Mouse.h>

byte i;

void setup() {
  VUSB_Mouse.begin();
}

void loop() {

  for (i = 1; i < 255; i++) {
    VUSB_Mouse.move(-5, 0, 0, 0);
    VUSB_Mouse.delay(2);
  }
  for (i = 1; i < 255; i++) {
    VUSB_Mouse.move( 0, -5, 0, 0);
    VUSB_Mouse.delay(2);
  }
  for (i = 1; i < 255; i++) {
    VUSB_Mouse.move( 5, 0, 0, 0);
    VUSB_Mouse.delay(2);
  }
  for (i = 1; i < 255; i++) {
    VUSB_Mouse.move( 0, 5, 0,  0);
    VUSB_Mouse.delay(2);
  }
}
