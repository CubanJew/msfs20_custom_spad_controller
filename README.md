# msfs20_custom_spad_controller
Microsoft Flight Simulator 2020 - Custom Panel Controller

### Description:
Custom control panel for Microsoft Flight Simulator 2020 using SPAD.neXt as interface between game and Arduino microcontroller.
Supports 
- 17 pushbutton LEDs (LED is controlled by state of control in the game).
- 5 toggle switches
- 2 selector switches
- 6 potentiometers (5 rotary + 1 slide)
- 1 foot pedal switch



Control board utilizes x3 MCP23017 I2C I/O expander ICs to provide control over 48 distinct inputs/outputs with just 2 Arduino pins. For LED outputs, each MCP23017 I/O pin is connected to an LED with a series resistor. For button inputs, each button is connected directly to an MCP23017 I/O pin, with an internal pull-up resistor enabled via software. All buttons are daisy-chained to ground on one side, and therefore no additional components are necessary for handling button inputs. 


### Installation Instructions:

1. Upload firmware (located in folder `\Software\AVR\`) to Arduino Pro Micro

2. Add Arduino as Serial device to SPAD.neXt under settings->Devices->Serial Devices.
	Speed = 115200
	Enable DTR = ON
	32 Bit = OFF

3. Add each of the scripts included in `\Software\SPAD.next Configuration\` folder to SPAD.neXt script panel.



References:

- [Fusion 360 3D Design File](https://a360.co/2SDgtfb)
