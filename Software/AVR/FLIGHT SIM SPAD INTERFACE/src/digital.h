#pragma once
#ifndef _DIGITAL_H

#include <Arduino.h>
#include "connections.h"
#include <CmdMessenger.h>  // CmdMessenger
#include <Adafruit_MCP23017.h>
#include "global.h"

#include "main.h"

extern Adafruit_MCP23017 mcp1, mcp2, mcp3;


void init_DigitalPins();
void scanDigitalInputs();

#endif
