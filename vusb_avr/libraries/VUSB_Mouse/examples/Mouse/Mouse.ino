// VUSB_Mouse test and usage documentation
// CAUTION!!!! This does click things!!!!!!!!
// Originally created by Sean Murphy (duckythescientist)

#include <VUSB_Mouse.h>

void setup() {
  VUSB_Mouse.begin();
}

void loop() {

  // move across the screen
  // these are signed chars
  VUSB_Mouse.moveY(10); //down 10
  VUSB_Mouse.delay(500);
  VUSB_Mouse.moveX(20); //right 20
  VUSB_Mouse.delay(500);
  VUSB_Mouse.scroll(5);
  VUSB_Mouse.delay(500);

  // VUSB_Mouse.move(X, Y, scroll) also works

  // three buttons are the three LSBs of an unsigned char
  VUSB_Mouse.setButtons(1<<0); //left click
  VUSB_Mouse.delay(500);
  VUSB_Mouse.setButtons(0); //unclick all
  VUSB_Mouse.delay(500);

  //or you can use these functions to click
  VUSB_Mouse.rightClick();
  VUSB_Mouse.delay(500);
  VUSB_Mouse.leftClick();
  VUSB_Mouse.delay(500);
  VUSB_Mouse.middleClick();
  VUSB_Mouse.delay(500);

  // VUSB_Mouse.move(X, Y, scroll, buttons) works, too

  // use VUSB_Mouse.delay(), or make sure to call
  // VUSB_Mouse.update() at least every 50ms

}
