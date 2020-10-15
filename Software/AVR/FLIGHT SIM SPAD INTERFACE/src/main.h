#pragma once
#ifndef _MAIN_H
// above added  since reorganization
#include "commands.h"
#include "connections.h"
#include "analog.h"
#include "digital.h"

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <CmdMessenger.h>  // CmdMessenger

#include "global.h"

//#include <avr/pgmspace.h>
enum BUTTON_TYPE {  // button type execution
    _PB_MOM,           //  _PB_MOM = only read on HIGH;
    _BINARY,           //  _BINARY = read HIGH & LOW and send currentVal as Event arg
    _TOGGLE            //  _TOGGLE = read HIGH & LOW but send Event w/ no ARGS. Requires data sub (?))
  };


enum PROVIDER {SIMCONNECT = 0, MSFS = 1, LOCAL=2};
typedef struct  BUTTON {
  uint8_t id;             // messenger command ID (COMMAND_TYPE). If "kINVALID" then button ignored during execution
  BUTTON_TYPE type;       // _PB_MOM, _BINARY, _TOGGLE handled differently during processing of button change state
  uint8_t currentVal = 0; // current button state
  bool change = false; //  button state changed since command last processed/executed
  PROVIDER provider;  // MSFS, SIMCONNECT, or LOCAL (local=expose device variable to SPAD)
  uint8_t cmdIndex;   // PROGMEM string table index reference
  int arg = -999;   // Command execution parameter for command execution. -999 denotes no argument (uses button state as argument, as-necessary)

  BUTTON(uint8_t id_, BUTTON_TYPE type_  , PROVIDER provider_, uint8_t cmdIndex_/*char *cmd_on_*/, int arg_) {    // binary switch
    id = id_;
    type = type_;
    provider = provider_;
    cmdIndex = cmdIndex_; //cmd_on = cmd_on_;
    arg = arg_;
  }
  BUTTON() {}
  BUTTON (COMMAND_TYPE cmdType) {
    if (cmdType == kINVALID)
      id = kINVALID;
  }
};



enum AUTOBRAKE_SELECTED {
  BRAKE_LO = 0, BRAKE_MED = 1, BRAKE_MAX = 2, BRAKE_OFF = 3
};



String getPStr(int index);
void initDIButtonMap();
void scanDigitalInputs();
void processChangeCommands(BUTTON * buttons);
void digitalWriteAutoBrakeLEDs();

void initPins();
void attachCommandCallbacks();
void onIdentifyRequest();
void onUnknownCommand();
void exposeLocalDeviceVariable(COMMAND_TYPE cmdID, String path, String varName, String description);
#endif
