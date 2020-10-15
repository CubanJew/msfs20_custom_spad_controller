#pragma once
#ifndef _ANALOG_H

#include <Arduino.h>
#include "connections.h"
#include <CmdMessenger.h>  // CmdMessenger
#include "global.h"

extern CmdMessenger messenger;

void init_AnalogPins();
void scanAnalogInputs();
void sendPotValue(int simPotID, int val);
void sendGlareshieldLightState(bool state);

#define POT_INDEX_ECAM_UPPER          22 //92
#define POT_INDEX_ECAM_LOWER          23 //93
#define POT_INDEX_PFD_PILOT           18 //88
#define POT_INDEX_ND_PILOT            19 //89
#define POT_INDEX_FD                  17 //87
#define POT_INDEX_ND_COPILOT          21 //91
#define POT_INDEX_PFD_COPILOT         20 //90
#define POT_INDEX_OVERHEAD_PNL_LT     16 //86
#define POT_INDEX_MAIN_PNL_PED_LT     15 //85
#define POT_INDEX_DASH_PNL_LT         14 //84
#define POT_INDEX_FLOOD_DASH_PILOT    11 //81
#define POT_INDEX_FLOOD_MAIN_PNL      13 //83
#define POT_INDEX_FLOOD_DASH_COPILOT  12 //82


#define GLARESHIELD_LIGHT_PILOT       1    // glareshield lights on parameter 1 associated w/ POT_INDEX_FLOOD_DASH_PILOT (pot 11)
#define GLARESHIELD_LIGHT_COPILOT     2    // glareshield lights on parameter 2  associated w/ POT_INDEX_FLOOD_DASH_COPILOT (pot 12)
#define GLARESHIELD_LIGHT_MAIN        3    // glareshield lights on parameter 3 (pedastol) associated w/ POT_INDEX_FLOOD_MAIN_PNL (pot 13)




#define ANALOG_HYSTERESIS 5

#endif
