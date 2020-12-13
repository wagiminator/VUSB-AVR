/*
This example produces a "cursor jitter" 
For compatability with other libraries you may use VUSB_Mouse.move(X, Y, scroll, buttons)
*/

#include <VUSB_Mouse.h>

void setup()
{
  VUSB_Mouse.begin();
}

void loop()
{
  VUSB_Mouse.move(random(-1,2), random(-1,2),0, 0 );
//  VUSB_Mouse.delay(1);
  VUSB_Mouse.update();
}

