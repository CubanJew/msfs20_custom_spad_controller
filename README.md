# msfs20_custom_spad_controller
Microsoft Flight Simulator 2020 - Custom Flight Panel Controller

### Description:
Custom control panel for Microsoft Flight Simulator 2020 using SPAD.neXt as interface between game and Arduino microcontroller. Designed with A320 in mind. Panel is meant to be mounted to the side of Logitech G Flight Simulator Panel bracket. 

![alt text](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/resources/readme_image.jpg?raw=true)
![alt text](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/resources/readme_image2.png?raw=true)


Supports:
- **17 pushbutton with LED** (LED is controlled by state of control in the game).
	_[fuel pumps 1-6, cross-feed pump, APU-master, APU-start, APU-bleed air, APU-gen, external power, auto-brake level control (low + med + max), x2 TBD (spare)]_
	
- **5 toggle switches**
	_[anti-skid, ground spoiler arm, engine 1 + 2 master starters, reverse throttle toggle]_
	
- **2 selector switches**
	_[engine mode switch (crank/normal/ignition), parking break]_
- **6 potentiometers** (5 rotary + 1 slide)
	_[lights: PFD/ND/FD, ECAM, flood, panel, x1 TBD (spare);  speed brake (slide pot)]_
- **1 foot pedal switch**
	_[brake pedals]_


Control board utilizes x3 MCP23017 I2C I/O expander ICs to provide control over 48 distinct inputs/outputs with just 2 Arduino pins. For LED outputs, each MCP23017 I/O pin is connected to an LED with a series resistor. For button inputs, each button is connected directly to an MCP23017 I/O pin, with an internal pull-up resistor enabled via software. All buttons are daisy-chained to ground on one side, and therefore no additional components are necessary for handling button inputs. Note that the pushbutton LEDs are controlled independently of the pushbutton meaning that the LED light state will reflect the state of the respective control in the game whether you cycle the control in the game or via the control panels physical button. The Arduino microcontrller handles detecting button presses and controls LED lights, and utilizes a 2-way serial connection with SPAD.neXT to interface with the game. 


### Installation Instructions:
1. Build the control interface board by following [schematics](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Electrical/Schematic.pdf).

2. 3D print STL files located in `/Hardware/Panel 3D Files/STL/`. You will need 3 Brackets, 1 Slide Potentiometer Lever Hat, and one of each of the three faceplates. Use soldering iron to heat insert four M3-4mm knurled brass inserts into each bracket's corner faceplate mounting holes and secure each faceplate to bracket with four M3-6mm bolts. Note that bracket top mounting holes are misaligned relative to the bottom mounting holes. If you wish to attach the three brackets vertically, drill new upper mounting holes to 2 of 3 the brackets. Bracket mounting hole alignment will be fixed in future. 

3. Install buttons/potentiometers on to faceplate holes (note: you will need to use a deburring tool to slightly enlarge the component holes as there is very little tolerance) and wire up connections between panel and control interface board. Refer to [Bill of Materials](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Bill%20of%20Materials.txt) for parts information. 

4. To upload microcontroller firmware, install PlatformIO IDE and add `/Software/AVR/FLIGHT SIM SPAD INTERFACE/` as new folder project. Upload code to your Arduino Pro Micro. 

5. Add control interface board to SPAD.neXt as a new Serial device to SPAD.neXt under settings->Devices->Serial Devices->"Add New Device" with the following settings:

	> Serial Port = = your USB COM port
	
	> Speed = 115200
	
	> Enable DTR = ON
	
	> 32 Bit = OFF
	
   Save settings and restart SPAD.neXt. Verify control interface board serial device is connected to SPAD.neXt by going to panels->SERIAL COMXX and selecting "Show Log". If it is not connected press "Connect".
   
6. Add each of the scripts included in `/Software/SPAD.next Configuration/` folder to SPAD.neXt script panel.


![alt text](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/resources/3d%20render.png?raw=true)

References:

- [Fusion 360 3D Design File](https://a360.co/2SDgtfb) or `/Hardware/Panel 3D Files/FLIGHT SIM COCKPIT v27.f3d`
- [Schematic](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Electrical/Schematic.pdf)
- [I/O Reference Table](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Electrical/IO%20Table.JPG) 
- [Bill of Material](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Bill%20of%20Materials.txt)
- [Project Photo Gallery](https://imgur.com/a/4q6AOHk)

Notes:
1. Faceplate component holes do not have much tolerance; you will need to use a debururing to slightly enlarge the holes. 
2. The size of the microcontroller firmware is large relative to flash program memory of the Arduino Pro Micro. As such, many repeated firmware uploads may randomly corrupt the bootloader. You will need another Arduino or an AVR programmer to reupload the bootloader to allow firmware uploading from your IDE.  

To-do list:
- Fix bracket top mounting hole alignment.
- Combine ground spoiler arm toggle switch and speed brake slide potentiometer into a single physical component.
- Add logic to force game's initial state of anti-skid, ground spoiler arm, and parking brake to match control board when controller is first connected.
- Implement spare 2 buttons and 1 potentiometer on panel. 
- Fabricate front faceplates with backlit labels using http://www.frontpanelexpress.com/ 
- Clean up code
- PWM brightness control for LEDs
