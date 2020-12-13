#define USB_CFG_DEVICE_NAME     'D','i','g','i','B','l','i','n','k'
#define USB_CFG_DEVICE_NAME_LEN 9
#include <VUSB_DigiUSB.h>
byte in = 0;
int Blue = 0;
int Red = 0;
int Green = 0;

int next = 0;

void setup() {
    VUSB_DigiUSB.begin();
    pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
    pinMode(2,OUTPUT);
}


void loop() {
  setBlue();
      VUSB_DigiUSB.refresh();
      setBlue();
     if (VUSB_DigiUSB.available() > 0) {
       in = 0;
       
       in = VUSB_DigiUSB.read();
       if (next == 0){
         if(in == 115){
           next = 1;
           VUSB_DigiUSB.println("Start");
         }
       }
       else if (next == 1){
            Red = in;
            VUSB_DigiUSB.print("Red ");
            VUSB_DigiUSB.println(in,DEC);
            next = 2;
       }
       else if (next == 2){
            Green = in;
            VUSB_DigiUSB.print("Green ");
            VUSB_DigiUSB.println(in,DEC);
            next = 3;
       }
       else if (next == 3){
            Blue = in;
            VUSB_DigiUSB.print("Blue ");
            VUSB_DigiUSB.println(in,DEC);
            next = 0;
       }


        
        
     }
     

    analogWrite(0,Red);
    analogWrite(1,Green);
     setBlue();

     
   
}

void setBlue(){
    if(Blue == 0){
      digitalWrite(2,LOW);
      return;
    }
    else if(Blue == 255){
      digitalWrite(2,HIGH);
      return;
    }
    // On period  
    for (int x=0;x<Blue;x++){
    digitalWrite(2,HIGH);
    } 
    
    // Off period
    
    for(int x=0;x<(255-Blue);x++){
    digitalWrite(2,LOW);
    }
  
} 

