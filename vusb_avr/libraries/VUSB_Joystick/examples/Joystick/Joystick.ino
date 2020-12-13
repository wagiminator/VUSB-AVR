
#include "VUSB_Joystick.h"

void setup() {
}


void loop() {
  // If not using plentiful VUSB_Joystick.delay() calls, make sure to
  // VUSB_Joystick.update(); // call this at least every 50ms
  // calling more often than that is fine
  // this will actually only send the data every once in a while unless the data is different
  
  // you may set the values from a raw byte array with:
  // char myBuf[8] = {
  //   x, y, xrot, yrot, zrot, slider,
  //   buttonLowByte, buttonHighByte
  // };
  // VUSB_Joystick.setValues(myBuf);
  
  // Or we can also set values like this:
  VUSB_Joystick.setX((byte) (millis() / 100)); // scroll X left to right repeatedly
  VUSB_Joystick.setY((byte) 0x30);
  VUSB_Joystick.setXROT((byte) 0x60);
  VUSB_Joystick.setYROT((byte) 0x90);
  VUSB_Joystick.setZROT((byte) 0xB0);
  VUSB_Joystick.setSLIDER((byte) 0xF0);
  
  // it's best to use VUSB_Joystick.delay() because it knows how to talk to
  // the connected computer - otherwise the USB link can crash with the 
  // regular arduino delay() function
  VUSB_Joystick.delay(50); // wait 50 milliseconds
  
  // we can also set buttons like this (lowByte, highByte)
  // VUSB_Joystick.setButtons(0x00, 0x00);
}