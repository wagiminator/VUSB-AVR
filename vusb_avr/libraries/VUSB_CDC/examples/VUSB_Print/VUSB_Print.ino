#include <VUSB_CDC.h>

int i;

void setup() {
  // initialize the digital pin as an output.
  Serial_VUSB.begin();
  /*
  remember the SerialUSB starts as soon as you call begin
  and doesn't restart the board when you open the serial monitor
  (like the uno does) - so if you print to it and you don't
  have a serial monitor open that text is lost.
  */
}

void loop() {
  for (i = 1; i < 32768; i++) {
    //wrap your strings in F() to save ram!
    Serial_VUSB.print(i);
    Serial_VUSB.println(F(" 1234567890 abcdefghijklmnopqrstuvwxyz !\"§$%&/()=?")); //wrap your strings in F() to save ram!
  }
  // Serial_VUSB.delay(100);
  /*
  if you don't call a SerialUSB function (write, print, read, available, etc)
  every 10ms or less then you must throw in some SerialUSB.refresh();
  for the USB to keep alive - also replace your delays - ie. delay(100);
  with SerialUSB.delays ie. SerialUSB.delay(100);
  */
}
