#include <VUSB_DigiUSB.h>

void setup() {
  VUSB_DigiUSB.begin();
}

void get_input() {
  int lastRead;
  // when there are no characters to read, or the character isn't a newline
  while (true) { // loop forever
    if (VUSB_DigiUSB.available()) {
      // something to read
      lastRead = VUSB_DigiUSB.read();
      VUSB_DigiUSB.write(lastRead);
      
      if (lastRead == '\n') {
        break; // when we get a newline, break out of loop
      }
    }
    
    // refresh the usb port for 10 milliseconds
    VUSB_DigiUSB.delay(10);
  }
}

void loop() {
  // print output
  VUSB_DigiUSB.println("Waiting for input...");
  // get input
  get_input();
}