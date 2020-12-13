#include <VUSB_CDC.h>
void setup() {                
  // initialize the digital pin as an output.
  Serial_VUSB.begin(); 
}

// the loop routine runs over and over again forever:
void loop() {
  
  if (Serial_VUSB.available()) {
    Serial_VUSB.write(Serial_VUSB.read());
  }
  
   //SerialUSB.delay(10);
   /*
   if you don't call a SerialUSB function (write, print, read, available, etc) 
   every 10ms or less then you must throw in some SerialUSB.refresh(); 
   for the USB to keep alive - also replace your delays - ie. delay(100); 
   with SerialUSB.delays ie. SerialUSB.delay(100);
   */
}