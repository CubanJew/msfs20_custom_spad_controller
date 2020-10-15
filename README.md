# msfs20_custom_spad_controller
Microsoft Flight Simulator 2020 - Custom Panel Controller

### Description:
Custom control panel for Microsoft Flight Simulator 2020 using SPAD.neXt as interface between game and Arduino microcontroller. Designed with A320 in mind. Panel is meant to be mounted to the side of Logitech G Flight Simulator Panel bracket. 

Supports:
- **17 pushbutton with LED** (LED is controlled by state of control in the game).
	_[fuel pumps 1-6, cross-feed pump, APU-master, APU-start, APU-bleed air, APU-gen, external power, auto-brake level control (low + med + max), x2 TBD (spare)]_
	
- **5 toggle switches**
	_[anti-skid, ground spoiler arm, engine 1 + 2 master starters, reverse throttle toggle]_
	
- **2 selector switches**
	_[engine mode switch (crank/normal/ignition), parking break]_
- **6 potentiometers** (5 rotary + 1 slide)
	_[lights: PFD/ND/FD, ECAM, flood, panel, 1 TBD (spare);  speed brake (slide pot)]_
- **1 foot pedal switch**
	_[brake pedals]_


Control board utilizes x3 MCP23017 I2C I/O expander ICs to provide control over 48 distinct inputs/outputs with just 2 Arduino pins. For LED outputs, each MCP23017 I/O pin is connected to an LED with a series resistor. For button inputs, each button is connected directly to an MCP23017 I/O pin, with an internal pull-up resistor enabled via software. All buttons are daisy-chained to ground on one side, and therefore no additional components are necessary for handling button inputs. 


### Installation Instructions:

1. Upload firmware (located in folder `\Software\AVR\`) to Arduino Pro Micro

2. Add Arduino as Serial device to SPAD.neXt under settings->Devices->Serial Devices.

	Speed = 115200
	
	Enable DTR = ON
	
	32 Bit = OFF
	

3. Add each of the scripts included in `\Software\SPAD.next Configuration\` folder to SPAD.neXt script panel.


![alt text](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/resources/3d%20render.png?raw=true)

References:

- [Fusion 360 3D Design File](https://a360.co/2SDgtfb)
- [Schematic](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Electrical/Schematic.pdf)
- [I/O Reference Table](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Electrical/IO%20Table.JPG) 
- [Bill of Material](https://github.com/CubanJew/msfs20_custom_spad_controller/blob/main/Hardware/Bill%20of%20Materials.txt)

Notes:
Faceplate component holes do not have much tolerance; you will need to use a debururing to slightly enlarge the holes. 

Issues:
Bracket top/bottom mounting holes are not vertically aligned.
