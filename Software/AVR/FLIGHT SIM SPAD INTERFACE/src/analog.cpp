#include "analog.h"


uint8_t potLastVal[6] = {0, 0, 0, 0, 0, 0}; // PFD_ND_FD; ECAM; PANEL; FLOOD; TBD; SPEED BRAKE

void init_AnalogPins()
{
  // POTENTIOMETERS (lights)
  pinMode(P_AI_POT_PFD_ND_FD, INPUT);
  pinMode(P_AI_POT_ECAM,      INPUT);
  pinMode(P_AI_POT_PANEL,     INPUT);
  pinMode(P_AI_POT_FLOOD,     INPUT);
  pinMode(P_AI_POT_TBD,       INPUT);
  pinMode(P_AI_POT_SPOILER,   INPUT);
}

void scanAnalogInputs() {
  int sensorValue;
  // PFD & ND & FD Display
  sensorValue = map(analogRead(P_AI_POT_PFD_ND_FD), 0, 1023, 100, 0);
  if (abs(potLastVal[0] - sensorValue) >= ANALOG_HYSTERESIS) {
    sendPotValue(POT_INDEX_PFD_PILOT, sensorValue);  // PFD - PILOT
    sendPotValue(POT_INDEX_ND_PILOT, sensorValue);  // ND - PILOT
    sendPotValue(POT_INDEX_FD, sensorValue);  // FD
    sendPotValue(POT_INDEX_ND_COPILOT, sensorValue);  // PFD - COPILOT
    sendPotValue(POT_INDEX_PFD_COPILOT, sensorValue);  // ND-COPILOT
    potLastVal[0] = sensorValue;
  }
  // ECAM (lower&upper)
  sensorValue = map(analogRead(P_AI_POT_ECAM), 0, 1023, 100, 0) ;
  if (abs(potLastVal[1] - sensorValue) >= ANALOG_HYSTERESIS) {
    sendPotValue(POT_INDEX_ECAM_UPPER, sensorValue);  // ECAM - UPPER
    sendPotValue(POT_INDEX_ECAM_LOWER, sensorValue);  // ECAM - LOWER
    potLastVal[1] = sensorValue;
  }
  //  Panel
  sensorValue = map(analogRead(P_AI_POT_PANEL), 0, 1023, 100, 0);
  if (abs(potLastVal[2] - sensorValue) >= ANALOG_HYSTERESIS) {
    sendPotValue(POT_INDEX_OVERHEAD_PNL_LT, sensorValue);  // panel lighting
    sendPotValue(POT_INDEX_MAIN_PNL_PED_LT, sensorValue);  // main panel and pedastol lighting
    sendPotValue(POT_INDEX_DASH_PNL_LT, sensorValue);  // overhead panel lighting
    potLastVal[2] = sensorValue;
  }
  // Flood Lights
  sensorValue = map(analogRead(P_AI_POT_FLOOD), 0, 1023, 100, 0);
  if (abs(potLastVal[3] - sensorValue) >= ANALOG_HYSTERESIS) {
    //messenger.sendCmd(kSimCommand, "MSFS:GLARESHIELD_LIGHTS_ON");  //cmd_on);
    if(sensorValue == 0)
      sendGlareshieldLightState(false);
    else if (sensorValue > 0 && potLastVal[3] == 0)
      sendGlareshieldLightState(true);

    sendPotValue(POT_INDEX_FLOOD_DASH_PILOT, sensorValue);  // main panel flood light
    sendPotValue(POT_INDEX_FLOOD_MAIN_PNL, sensorValue);  // flood lite main panel
    sendPotValue(POT_INDEX_FLOOD_DASH_COPILOT, sensorValue);  // PED flood lt
    potLastVal[3] = sensorValue;
  }
  // TBD / Spare
  sensorValue = map(analogRead(P_AI_POT_TBD), 0, 1023, 100, 0);
  if (abs(potLastVal[4] - sensorValue) >= ANALOG_HYSTERESIS) {
    // @ TODO: implement
    potLastVal[4] = sensorValue;
  }
  // SPEED BRAKE / SPOILER   // @TODO: ADD BIAS FOR ZERO
  sensorValue = map(analogRead(P_AI_POT_SPOILER), 0, 1023, 0, 100);
  if (abs(potLastVal[5] - sensorValue) >= ANALOG_HYSTERESIS) {
    messenger.sendCmdStart(kSimCommand);
    messenger.sendCmdArg("SIMCONNECT:SPOILERS_SET");
    messenger.sendCmdArg(map(sensorValue, 0, 100, 0, 16383));
    messenger.sendCmdEnd();
    potLastVal[5] = sensorValue;
  }
}

// sends value to all pot IDs
void sendPotValue(int simPotID, int val) {
  messenger.sendCmdStart(kSimCommand);
  messenger.sendCmdArg("MSFS:LIGHT_POTENTIOMETER_" + String(simPotID) + "_SET");
  messenger.sendCmdArg(val);
  messenger.sendCmdEnd();
}
// Enables/Disables glareshield for flood lights (enabled if flood light value > 0)
void sendGlareshieldLightState(bool state)
{
  uint8_t glareShieldIds[3] = {GLARESHIELD_LIGHT_PILOT,GLARESHIELD_LIGHT_COPILOT, GLARESHIELD_LIGHT_MAIN};
  for (int i=0; i<3; i++)
  {
    messenger.sendCmdStart(kSimCommand);
    messenger.sendCmdArg(state == true ? "MSFS:GLARESHIELD_LIGHTS_ON" : "MSFS:GLARESHIELD_LIGHTS_OFF");
    messenger.sendCmdArg(glareShieldIds[i]);
    messenger.sendCmdEnd();
  }
}
