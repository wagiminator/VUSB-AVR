// VUSB_Joystick, 2 axis, four switches
// axis potentiometers are attached to analog 6 and 7
// switches are attached to digital 2-5

#include"VUSB_Joystick.h"
void setup() {

/*
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  oder shorter; saves ~ 150 Bytes:
*/
  PORTA = 0x3c;                                              // activate pullups for digital inputs 2 ... 5

}

void loop() {

  char Buttons_lw = (PINA & 0x3c) ;                          // input pins PORT A, bit 2 ... 5 filtered
  Buttons_lw = ~(Buttons_lw / 4) ;                           // shifted to bit 0 ... 3 and inverted
  unsigned int X =   analogRead(6);                          // 0 ... 1023 
  X = X / 4;                                                 // shifted to 0 ... 255
  unsigned int Y =   analogRead(7);
  Y = Y / 4;
  DigiJoystick.setX((byte) X); 
  DigiJoystick.setY((byte) Y);
//  DigiJoystick.setXROT((byte) 0x00);                       // all unused
//  DigiJoystick.setYROT((byte) 0x00);
//  DigiJoystick.setZROT((byte) 0x00);
//  DigiJoystick.setSLIDER((byte) 0x00);

  DigiJoystick.setButtons(Buttons_lw, 0x00);                 // hi byte always 0 , low byte 0 ... 3
  DigiJoystick.update(); 

}
