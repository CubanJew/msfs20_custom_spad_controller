#include <Arduino.h>
#include "main.h"
#include "global.h"

CmdMessenger messenger(Serial);
Adafruit_MCP23017 mcp1, mcp2, mcp3;

AUTOBRAKE_SELECTED autobrakeSetting = BRAKE_OFF;
bool antiskid_en = false;
bool autoBrake_en = false;
bool isReady = false;

extern BUTTON buttons_1[], buttons_2[];

// LIMITATIONS:
/**

  TODO: A/SKID LOGIC CHECK AGAINST GAME STATE
  cHANGING aUTO BRAKE LEVEL CONTROL IN GAME will not update
*/

void setup() {
  Serial.begin(115200);
  init_AnalogPins();
  init_DigitalPins();

  initDIButtonMap();
  attachCommandCallbacks();
  // SIMCONNECT:STEERING_SET
}

void loop() {
  scanDigitalInputs();
  scanAnalogInputs();
  messenger.feedinSerialData();   // Process incoming serial data, and perform callbacks
  delay(100);

}


String getPStr(int index) {
  char cmd[_PSTR_BUFFER_SIZE];
  strcpy_P(cmd, (char*)pgm_read_word(&(PSTR_TABLE_CMDs[index]))); // Necessary casts and dereferencing, just copy.
  return String(cmd);
}
// special logic for keeping track of auto brake control level light status./
// Need's SUB'd to SIMCONNECT:ANTISKID BRAKES ACTIVE and
void digitalWriteAutoBrakeLEDs (AUTOBRAKE_SELECTED brakeLevel)
{
  mcp1.digitalWrite(P_DO_AUTOBRAKE_LO_LED, LOW);
  mcp1.digitalWrite(P_DO_AUTOBRAKE_MED_LED, LOW);
  mcp3.digitalWrite(P_DO_AUTOBRAKE_MAX_LED, LOW);

  // AUTO BRAKE level already set in-game (no way to access data)
  if (!autoBrake_en || brakeLevel == BRAKE_OFF) {
    autobrakeSetting = BRAKE_OFF;
  }
  // ANTI-SKID switch permissive to set brake level
  if (antiskid_en)
  {
    // handle toggle off overrides
    if (brakeLevel == BRAKE_LO) {
      if (autobrakeSetting == BRAKE_LO) { // already set to LOW; turn off LED
        mcp1.digitalWrite(P_DO_AUTOBRAKE_LO_LED, LOW);
        autobrakeSetting = BRAKE_OFF;
      } else {
        mcp1.digitalWrite(P_DO_AUTOBRAKE_LO_LED, HIGH);
        autobrakeSetting = BRAKE_LO;
      }
    } else if (brakeLevel == BRAKE_MED) {
      if (autobrakeSetting == BRAKE_MED) {
        mcp1.digitalWrite(P_DO_AUTOBRAKE_MED_LED, LOW);
        autobrakeSetting = BRAKE_OFF;
      } else {
        mcp1.digitalWrite(P_DO_AUTOBRAKE_MED_LED, HIGH);
        autobrakeSetting = BRAKE_MED;
      }

    } else if (brakeLevel == BRAKE_MAX) {
      if (autobrakeSetting == BRAKE_MAX) {
        mcp3.digitalWrite(P_DO_AUTOBRAKE_MAX_LED, LOW);
        autobrakeSetting = BRAKE_OFF;
      } else {
        mcp3.digitalWrite(P_DO_AUTOBRAKE_MAX_LED, HIGH);
        autobrakeSetting = BRAKE_MAX;
      }
    }
  }
  int brakeArg;
  if (autobrakeSetting == BRAKE_OFF)
    brakeArg = 1;
  else if (autobrakeSetting == BRAKE_MAX)
    brakeArg = 4; //buttons[4].arg;
  else if (autobrakeSetting == BRAKE_MED)
    brakeArg = 3; //buttons[5].arg;
  else if (autobrakeSetting == BRAKE_LO)
    brakeArg = 2; //buttons[6].arg;
  // NOTE: _CMD_PROVIDER IS PROBABLY BROKEN
  messenger.sendCmdStart(kSimCommand);  //buttons[ipstr_abrakeControlLevel].id);
  messenger.sendCmdArg("SIMCONNECT:" + getPStr(ipstr_abrakeControlLevel));  //_CMD_PROVIDER(ipstr_abrakeControlLevel));   //buttons[i].cmd_on);
  messenger.sendCmdArg(brakeArg);  // OFF -> SET_AUTOBRAKE_CONTROL=1
  messenger.sendCmdEnd();
}


void processChangeCommands(BUTTON * buttons) {
  for (int i = 0; i < 16; i++)
  {
    if ((buttons[i].id != kINVALID) && (buttons[i].change))
    {
      // MOMENTARY BUTTON PRESSED
      if (buttons[i].type == _PB_MOM && buttons[i].currentVal == 1)
      {
        //messenger.sendCmd(buttons[i].id, buttons[i].cmd_on);
        // SPECIAL CASE for auto-brake lights logic (button elements 4-6). Handle command seperately!
        if (buttons[i].cmdIndex == ipstr_abrakeControlLevel && buttons[i].arg == 4)
          digitalWriteAutoBrakeLEDs(BRAKE_MAX);   // high
        else if (buttons[i].cmdIndex == ipstr_abrakeControlLevel && buttons[i].arg == 3)
          digitalWriteAutoBrakeLEDs(BRAKE_MED);  // med
        else if (buttons[i].cmdIndex == ipstr_abrakeControlLevel && buttons[i].arg == 2)
          digitalWriteAutoBrakeLEDs(BRAKE_LO);   // low
        else
        {
          // below probably needss fixed
          if (buttons[i].arg == -999) // no argument
          {
            messenger.sendCmd(buttons[i].id, _CMD_PROVIDER(i));
          } else {
            messenger.sendCmdStart(buttons[i].id);
            messenger.sendCmdArg(_CMD_PROVIDER(i));   //buttons[i].cmd_on);
            messenger.sendCmdArg(buttons[i].arg);
            messenger.sendCmdEnd();
          }
        }


        // TOGGLE PUSHBUTTON
      } else if (buttons[i].type == _TOGGLE)
      {
        // ... TOGGLE BUTTON STATE CHANGE - SUBSCRIBED DATAE
          if (buttons[i].id != kSimCommand)
          {
            messenger.sendCmd(buttons[i].id, buttons[i].currentVal);
            // ... TOGGLE BUTTON STATE CHANGE - SEND VALUE
          }
          else if (buttons[i].id == kSimCommand)
          {
            messenger.sendCmd(kSimCommand, getPStr(buttons[i].cmdIndex));  //cmd_on);

          }
      // BINARY SWITCH BUTTON
      } else if (buttons[i].type == _BINARY)
      {
        // LOCAL PROVIDER MEANS LOGIC HANDLE BY SPAD (SPAD SCRIPTED EVENT PERFORMS LOGIC ON LOCAL DEVICE VARIABLE SUB)
        if(buttons[i].provider == LOCAL)
        {
          switch (buttons[i].id)
          {
            // ENG1/ENG2 STARTERS: SPAD scripted event actions:
            // CONDITION:
            //    yuri panel:SWITCHES/ENG1_STARTER_MASTER = 1 or -1
            //    yuri panel:SWITCHES/ENG2_STARTER_MASTER = 1 or -1
            //      (use 1/-1 for on/off so SPAD can distinguish from disconnection when value defaults to 0)
            // ACTIONS:
            //    TURN ON:
            //        SIMCONNECT:TOGGLE_STARTER1 [arg=0]
            //        MSFS:FUELSYSTEM_VALVE_OPEN [arg=6 for ENG 1; 7 for ENG 2]
            //    TURN OFF:
            //        SIMCONNECT:TOGGLE_STARTER1 [arg=0]
            //        MSFS:FUELSYSTEM_VALVE_CLOSE [arg=6 for ENG 1; 7 for ENG 2]

            // @TODO: HANDLE LOGIC FOR GAME/LOCAL MISMATCH
            case kLOCAL_ENG_MST_STARTER_1:
                messenger.sendCmd(kLOCAL_ENG_MST_STARTER_1, !buttons[i].currentVal == 0 ? -1 : 1);
                break;
            case kLOCAL_ENG_MST_STARTER_2:
                messenger.sendCmd(kLOCAL_ENG_MST_STARTER_2, !buttons[i].currentVal == 0 ? -1 : 1);
                break;

                // ENGINE MODE SELECTOR SWITCH  SPAD scripted event actions:
                // CONDITION:
                //    yuri panel:SWITCHES/ENG1_STARTER_MASTER = 1 or 0
                //    yuri panel:SWITCHES/ENG2_STARTER_MASTER = 1 or 0
                // ACTIONS:
                //    TURN ON:
                //        SIMCONNECT:TOGGLE_STARTER1 [arg=0]
                //        MSFS:FUELSYSTEM_VALVE_OPEN [arg=6 for ENG 1; 7 for ENG 2]
                //    TURN OFF:
                //        SIMCONNECT:TOGGLE_STARTER1 [arg=0]
                //        MSFS:FUELSYSTEM_VALVE_CLOSE [arg=6 for ENG 1; 7 for ENG 2]
            case kLOCAL_ENG_MODE_SS_POS_CRANK:
                messenger.sendCmd(kLOCAL_ENG_MODE_SS_POS_CRANK, buttons[i].currentVal);
                break;
            case kLOCAL_ENG_MODE_SS_POS_START:
                messenger.sendCmd(kLOCAL_ENG_MODE_SS_POS_START, buttons[i].currentVal);
                break;

                // BRAKE PEDAL  SPAD scripted event actions:
                // (use SPAD logic due to  2 simulation event calls required for left/right brake)
                // CONDITION:
                //    ...
                //
                // ACTIONS:
                //    TURN ON:
                //        SIMCONNECT:AXIS_LEFT_BRAKE_SET [arg=16383]
                //        SIMCONNECT:AXIS_RIGHT_BRAKE_SET [arg=16383]
                //    TURN OFF:
                //        SIMCONNECT:AXIS_LEFT_BRAKE_SET [arg=0]
                //        SIMCONNECT:AXIS_RIGHT_BRAKE_SET [arg=0]
            case kLOCAL_BRAKE_POS:
                messenger.sendCmd(kLOCAL_BRAKE_POS, buttons[i].currentVal);
                break;
          }
        } else
        {
          messenger.sendCmdStart(kSimCommand);
          messenger.sendCmdArg(getPStr(buttons[i].cmdIndex));   //_CMD_PROVIDER(ipstr_abrakeControlLevel));   //buttons[i].cmd_on);
          messenger.sendCmdArg(buttons[i].currentVal);  // OFF -> SET_AUTOBRAKE_CONTROL=1
          messenger.sendCmdEnd();
            //messenger.sendCmd(buttons[i].id, buttons[i].currentVal);
            //DEBUG_P(Serial.println("*" + buttons[i].currentVal ? buttons[i].cmd_on : buttons[i].cmd_off));
        }
      }
      buttons[i].change = false;
    }
  }
}


void onChangeFP1() {
  mcp1.digitalWrite(P_DO_FUEL_PMP_LT_1_LED, messenger.readInt32Arg());
}
void onChangeFP2() {
  mcp1.digitalWrite(P_DO_FUEL_PMP_LT_2_LED, messenger.readInt32Arg());
}
void onChangeFP3() {
  mcp1.digitalWrite(P_DO_FUEL_PMP_CT_1_LED, messenger.readInt32Arg());
}
void onChangeFP4() {
  mcp1.digitalWrite(P_DO_FUEL_PMP_CT_2_LED, messenger.readInt32Arg());
}
void onChangeFP5() {
  mcp1.digitalWrite(P_DO_FUEL_PMP_RT_1_LED, messenger.readInt32Arg());
}
void onChangeFP6() {
  mcp1.digitalWrite(P_DO_FUEL_PMP_RT_2_LED, messenger.readInt32Arg());
}
void onChangeFuelXFeed() {
  mcp1.digitalWrite(P_DO_FUEL_XFEED_LED, messenger.readInt32Arg());
}

void onChangeAntiSkidBrake()
{
  antiskid_en = (bool)messenger.readInt32Arg();
  // if antiskid disabled, turn off brake level control level
  if (!antiskid_en)
    digitalWriteAutoBrakeLEDs(BRAKE_OFF);
}
// LOCAL LOGIC FOR AUTO BRAKE LEVEL CONTROL
void onChangeAutoBrakeCB() {
  //int val = messenger.readInt32Arg();
  autoBrake_en =  !(messenger.readInt32Arg());  // 0 = ON, 1 = OFF.
  // Currently no way to read Auto Braake Level control state from game.
  // If auto brake level control set in-gaame turn on all 3 auto-brake lights on as an UNKNOWN value condition
  //messenger.sendCmd(kDebug, String(String("GOT CB CHANGE-") + String(val)));
  //if (autoBrake_en )
  //  {
  //  mcp1.digitalWrite(P_DO_AUTOBRAKE_LO_LED, HIGH);
  //  mcp1.digitalWrite(P_DO_AUTOBRAKE_MED_LED, HIGH);
  //  mcp3.digitalWrite(P_DO_AUTOBRAKE_MAX_LED, HIGH);
  //}

}
void onChangeParkBrake() {
  // On controller/game position mismatch toggle game's park brake switch to force alignment
  if (((bool)messenger.readInt32Arg() != (mcp3.digitalRead(P_DI_BRAKE_PARK)))) // mcp input inverted instead of changing wiring
      messenger.sendCmd(kSimCommand, getPStr(ipstr_parkbrake));
}

void onChangeAPU_gen() {
  mcp1.digitalWrite(P_DO_APU_GEN_LED, messenger.readInt32Arg());

}
void onChangeAPU_bleed() {
  mcp1.digitalWrite(P_DO_APU_BLEED_LED, messenger.readInt32Arg() == 0 ? HIGH : LOW); // verify valrange (0-3)
}

void onChangeAPU_master() {
  mcp1.digitalWrite(P_DO_APU_MASTER_LED, messenger.readInt32Arg());
}

void onChangeExtPower() {
  mcp1.digitalWrite(P_DO_EXT_PWR_LED, messenger.readInt32Arg()); // verify valrange (0-3)

}

void onChangeEng1Master() {
  // On controller/game position mismatch toggle engine master switch
  int gameVal = messenger.readInt32Arg();
  if (gameVal != !mcp2.digitalRead(P_DI_ENG_MST_1))
    messenger.sendCmd(kLOCAL_ENG_MST_STARTER_1,!gameVal);

}
void onChangeEng2Master() {
  // On controller/game position mismatch toggle engine master switch
  int gameVal = messenger.readInt32Arg();
  if (gameVal != !mcp2.digitalRead(P_DI_ENG_MST_2))
    messenger.sendCmd(kLOCAL_ENG_MST_STARTER_2,!gameVal);
}

void onChangeStarter() {
  mcp1.digitalWrite(P_DO_APU_STARTER_LED, messenger.readInt32Arg()); // verify valrange (0-3)

}
void attachCommandCallbacks()
{
  // Attach callback methods - dont forget to sub below
  messenger.attach(onUnknownCommand);
  messenger.attach(kRequest  , onIdentifyRequest);
  messenger.attach(kFUEL_PMP1 , onChangeFP1);
  messenger.attach(kFUEL_PMP2 , onChangeFP2);
  messenger.attach(kFUEL_PMP3 , onChangeFP3);
  messenger.attach(kFUEL_PMP4 , onChangeFP4);
  messenger.attach(kFUEL_PMP5 , onChangeFP5);
  messenger.attach(kFUEL_PMP6 , onChangeFP6);
  messenger.attach(kFUEL_XFEED , onChangeFuelXFeed);
  messenger.attach(kAUTOBRAKE_CB_EN, onChangeAutoBrakeCB);
  messenger.attach(kANTISKID, onChangeAntiSkidBrake);
  messenger.attach(kPARKBRAKE_POS, onChangeParkBrake);

  messenger.attach(kAPU_GEN_POS, onChangeAPU_gen);
  messenger.attach(kAPU_BLEED_POS, onChangeAPU_bleed);
  messenger.attach(kAPU_MASTER_POS, onChangeAPU_master);

  messenger.attach(kExtPower_POS, onChangeExtPower);

  messenger.attach(kENG1_Master_Pos, onChangeEng1Master);
  messenger.attach(kENG2_Master_Pos, onChangeEng2Master);

  messenger.attach(kAPU_STARTER, onChangeStarter);

}

// Called when a received command has no attached function
void onUnknownCommand()
{
}

void subscribeData(int channelID, PROVIDER provider, char * dataStr) {
  messenger.sendCmdStart(kCommand);
  messenger.sendCmdArg("SUBSCRIBE");
  messenger.sendCmdArg(channelID);
  messenger.sendCmdArg(provider ? "MSFS:" + String(dataStr) : "SIMCONNECT:" + String(dataStr));
  messenger.sendCmdEnd();
}

// Callback function to respond to indentify request. This is part of the
// Auto connection handshake.
void onIdentifyRequest()
{
  char *szRequest = messenger.readStringArg();

  if (strcmp(szRequest, "INIT") == 0) {
    messenger.sendCmdStart(kRequest);
    messenger.sendCmdArg("SPAD");
    // Unique Device ID: Change this!
    messenger.sendCmdArg(F("{DD7E3826-E439-4484-B186-F1443F3BC522}"));
    // Device Name for UI
    messenger.sendCmdArg("CJ Custom Panel");
    messenger.sendCmdEnd();
    return;
  }

  if (strcmp(szRequest, "PING") == 0) {
    messenger.sendCmdStart(kRequest);
    messenger.sendCmdArg("PONG");
    messenger.sendCmdArg(messenger.readInt32Arg());
    messenger.sendCmdEnd();
    return;
  }
  if (strcmp(szRequest, "CONFIG") == 0) {

    // SUBSCRIBE TO DATA
    // FUEL PUMP STATUS LIGHTS
    int pumpIDs[] = {2, 5, 1, 4, 3, 6}; // order of pump indicies left-to-right
    int pumpChannel[] =   {kFUEL_PMP1, kFUEL_PMP2, kFUEL_PMP3, kFUEL_PMP4, kFUEL_PMP5, kFUEL_PMP6}; // CMDID for exposed data to SPAD.neXt
    for (int i = 0; i < 6; i++)
      subscribeData(pumpChannel[i], MSFS,       ("FUELSYSTEM PUMP ACTIVE:" + String(pumpIDs[i])).c_str());

    subscribeData(kFUEL_XFEED,      MSFS,      "FUELSYSTEM VALVE SWITCH:1"); // FUEL X-FEED
    subscribeData(kANTISKID,        SIMCONNECT, getPStr(ipstr_antiskid).c_str()/*.c_str()*/); //str_antiskid); // ANTI-SKID  (also needed for digitalWriteAutoBrakeLEDs())
    subscribeData(kAUTOBRAKE_CB_EN, SIMCONNECT, "AUTO BRAKE SWITCH CB"); // auto brake level control circuit breaker (needed for digitalWriteAutoBrakeLEDs())
    subscribeData(kPARKBRAKE_POS,   SIMCONNECT, getPStr(ipstr_parkBrakeState).c_str());
    // APU ...
    subscribeData(kAPU_GEN_POS,    SIMCONNECT, getPStr(ipstr_apuGenSwitchPos).c_str());
    subscribeData(kAPU_BLEED_POS,   SIMCONNECT, getPStr(ipstr_apuBleedSwitchPos).c_str());
    subscribeData(kAPU_MASTER_POS,   MSFS, getPStr(ipstr_apuMasterSwitchPos).c_str());

    subscribeData(kExtPower_POS,   MSFS, getPStr(ipstr_extPowerSwitchPos).c_str());

    subscribeData(kAPU_STARTER,   MSFS, "APU SWITCH");


    // expose LOCAL DEVICE variable for MASTER START SWITCH (spad scripted event handles logic) ...
    // ... ENG1-MASTER SWITCH & ENG2-MASTER SWITCH:
    exposeLocalDeviceVariable(kLOCAL_ENG_MST_STARTER_1, "switches/ENG1_STARTER_MASTER", "?", "?");
    exposeLocalDeviceVariable(kLOCAL_ENG_MST_STARTER_2, "switches/ENG2_STARTER_MASTER", "?", "?");
    // ... ENGINE MODE SELECTOR SWITCH:
    exposeLocalDeviceVariable(kLOCAL_ENG_MODE_SS_POS_CRANK, "switches/ENG_MODE_SS_CRANK", "?", "?");
    exposeLocalDeviceVariable(kLOCAL_ENG_MODE_SS_POS_START, "switches/ENG_MODE_SS_START", "?", "?");

    exposeLocalDeviceVariable(kLOCAL_BRAKE_POS, "switches/BRAKE", "?", "?");


    // ... end expose local device variable

    // ENGINE MASTER SWITCH (MONITOR FOR GAME/CONTROLLER MISMATCH)
    //subscribeData(kENG1_Master_Pos,   MSFS,   String("GENERAL ENG STARTER ACTIVE:1").c_str());
    //subscribeData(kENG2_Master_Pos,   MSFS,    String("GENERAL ENG STARTER ACTIVE:2").c_str());
    //subscribeData(kENG2_Master_Pos,   MSFS,     String(getPStr(String(ipstr_engMasterSwitchPos).c_str()) + "2").c_str());

    //TURBINE_IGNITION_SWITCH_SET1
    //TURBINE_IGNITION_SWITCH_SET2


  }

  // tell SPAD.neXT we are done with config
  messenger.sendCmd(kRequest, "CONFIG");
  isReady = true;



  // SEND as-found state of physical toggle switches (that use toggle events) in event controller reconnected mid-game
  //messenger.sendCmd(kLOCAL_ENG_MST_STARTER_1, !buttons_1[i].currentVal == 0 ? -1 : 1);
  messenger.sendCmd(kLOCAL_ENG_MST_STARTER_1, !buttons_1[P_DI_ENG_MST_1].currentVal == 1 ? 1 : -1);
  messenger.sendCmd(kLOCAL_ENG_MST_STARTER_2, !buttons_1[P_DI_ENG_MST_2].currentVal == 1 ? 1 : -1);

  messenger.sendCmd(kLOCAL_ENG_MODE_SS_POS_CRANK, buttons_1[P_DI_ENG_MODE_SS1].currentVal == 1 ? 1 : -1);
  messenger.sendCmd(kLOCAL_ENG_MODE_SS_POS_START, buttons_1[P_DI_ENG_MODE_SS2].currentVal == 1 ? 1 : -1);
  //
  return;
}
 void exposeLocalDeviceVariable(COMMAND_TYPE cmdID, String path, String varName, String description)
{
    messenger.sendCmdStart(kCommand);
    messenger.sendCmdArg("ADD");
    messenger.sendCmdArg(cmdID);
    messenger.sendCmdArg(path); // will become "SERIAL:<guid>/leds/systemled"
    messenger.sendCmdArg("U8"); // unsigned 8bit data type
    messenger.sendCmdArg("RW"); // read/write
    messenger.sendCmdArg(varName);  // Name?
    messenger.sendCmdArg(description);  // Description
    messenger.sendCmdEnd();

 }
