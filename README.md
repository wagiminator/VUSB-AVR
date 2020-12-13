# VUSB-AVR
VUSB-AVR is a board definition package for the Arduino IDE, which was put together from already existing projects to simplify the development of USB devices with AVR microcontrollers. It is based on V-USB, which is a software-only implementation of low-speed USB devices for AVR microcontrollers, making it possible to build USB hardware with almost any AVR microcontroller, not requiring any additonal chip. The package includes board definitions for ATtiny44/84/45/85 and ATmega8/168p/328p as well as libraries and example sketches to build CDC, keyboard, mouse and joystick applications.

The package is based on:
- [V-USB by Objective Development](https://www.obdev.at/products/vusb/index.html)
- [Digistump libraries and examples](https://github.com/digistump/DigistumpArduino/tree/master/digistump-avr)
- [V-USB adaptation by Ralf Beesner](https://www.elektronik-labor.de/Arduino/VUSBduino.htm)
- [ATtiny core by David A. Mellis](https://github.com/damellis/attiny)
- [ATmega core by Carlos Rodrigues](https://github.com/carlosefr/atmega)

# Installation
- Open Arduino IDE
- Go to **File -> Preferences**
- Enter the following URL in **Additional Boards Manager URLs**:
    ```
    https://raw.githubusercontent.com/wagiminator/VUSB-AVR/main/archive/package_wagiminator_vusb_avr_index.json
    ```
- Separate the URLs using a comma ( **,** ) if you have more than one URL
- Go to **Tools -> Board -> Boards Manager...**
- Wait for the platform indexes to finish downloading
- Scroll down until you see the **AVR-USB** entry and click on it
- Click **Install**.
- After installation is complete close the **Boards Manager** window

# Hardware Setup
A possible and frequently used wiring is shown below:

![wiring.png](https://github.com/wagiminator/VUSB-AVR/blob/main/vusb_wiring.png)

The standard connection of D-/D+ to the physical pins of the AVR is shown in the table below:

|AVR Microcontroller|D-|D+|Clock|
|:-|:-|:-|:-|
|ATtiny45/85|PB3|PB4|16.5 MHz internal|
|ATtiny44/84|PA1|PA0|12 MHz external|
|ATmega8/168p/328p|PD7|PD2|12 MHz external|

# Software Setup
- Open Arduino IDE
- Go to **Tools -> Board -> VUSB AVR** and select **VUSB-AVR**
- Go to **Tools** and select your **CPU** and your **Programmer**
- Connect your programmer to your PC and your board
- Go to **Tools -> Burn Bootloader** to burn the fuses
- Now you can upload your sketch

For the example sketches go to **File -> Examples -> Examples for VUSB-AVR**.

