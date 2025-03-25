# EOA-electronics MidiPression

## Introduction
This is a simple project to allow the connection of standard guitar Expression-Pedals to a Software-Synthesizer or VST-Host (e.g. MainStage).  
One can connect up to 5 Pedals, and two so calles "Button-Boxes" to the Hardware as it is by now.
All analog inputs can be calibrated directly via a single-button interface on the device to make the usae of different-branded hardware possible.  
The Button boxes can be either Foot-Switches or Launchpad-Style MIDI controllers.

## Hardware
For now there is no ready-to-use hardware for this project. You have to build it by yourself using a 32u4 based Arduino (we recommend Sparkfun Pro Micro (16MHz!!!) or of its clones).  
Analog inputs are stadard 6.3mm Jacks (Stereo-Pinout), while button-boxes are connected with 3.5mm Jacks (you may change this as you wish).  
The pinout is free to choose, and can be set up in the pins.h configuration file.

## Firmware
The firmware is built in PlatformIO. Everything should be ready to use. Just input your pin-configuration is the `Firmware/include/pins.h` file, and change settings in `Firmware/include/configuration.h` to your liking.  
>**IMPORTANT:**  
To build the firmware open the `Firmware`folder as the >project-root! Otherwise PlatformIO may not build successfully!  

The firmware uses `MIDIUSB`as a dependency. This will be downloaded automatically by PlatformIO, so make sure you are connected to the Internet when building.

## Copyright and Licensing
Please read the `LICENSE`file for more information on usage rights.  
Although not explicitly forbidden by the license, do NOT use the name "MidiPression" as a trademark, and please notify us (the creator) if you are planning on marketing this project as a product. We are thankful for your compliance!  
For questions feel free to contact us at `code@eoa-electronics.de`
