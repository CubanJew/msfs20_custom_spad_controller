#include "digital.h"

uint16_t lastVal = 0; // of MCP2
uint16_t currentVal = 0;  // of MCP2

uint16_t lastVal_reg2 = 0;    // MCP3
uint16_t currentVal_reg2 =0;  // MCP3


BUTTON buttons_1[16];
BUTTON buttons_2[16];

void initDIButtonMap() {
  // MCP 2 buttons
  buttons_1[P_DI_PWR_EXT]             = BUTTON(kSimCommand, _PB_MOM,  MSFS,        ipstr_externalPower , -999);
  buttons_1[P_DI_TBD_2]               = BUTTON(kINVALID); // button 2 - to be implemented
  buttons_1[P_DI_TBD_1]               = BUTTON(kINVALID); // button 1 - to be implemented
  buttons_1[P_DI_REV_THRUST_TOG]      = BUTTON(kSimCommand, _PB_MOM,         MSFS,        ipstr_revThrustToggle ,   -999);
  buttons_1[P_DI_SPOILER_ARM]         = BUTTON(kSimCommand, _BINARY,         SIMCONNECT,  ipstr_spoilersArm ,       -999);
  buttons_1[P_DI_AUTOBRAKE_LOW]       = BUTTON(kSimCommand, _PB_MOM,         SIMCONNECT,  ipstr_abrakeControlLevel, 2); // AUTO-BRAKE= LOW
  buttons_1[P_DI_AUTOBRAKE_MED]       = BUTTON(kSimCommand, _PB_MOM,         SIMCONNECT,  ipstr_abrakeControlLevel, 3); // AUTO-BRAKE= MED
  buttons_1[P_DI_AUTOBRAKE_MAX]       = BUTTON(kSimCommand, _PB_MOM,         SIMCONNECT,  ipstr_abrakeControlLevel, 4); // AUTO-BRAKE= HIGH     @TODO: HANDLE UNKNOWN STATE IF IN-GAME CHANGED
  buttons_1[P_DI_APU_BLEED]           = BUTTON(kSimCommand, _PB_MOM,         MSFS,        ipstr_apuBleed ,          -999);
  buttons_1[P_DI_APU_GEN]             = BUTTON(kSimCommand, _PB_MOM,         SIMCONNECT,  ipstr_apuGen ,            -999);
  buttons_1[P_DI_APU_START]           = BUTTON(kSimCommand, _PB_MOM,         SIMCONNECT,  ipstr_apuStarter ,        -999);
  buttons_1[P_DI_APU_MST]             = BUTTON(kSimCommand, _PB_MOM,         MSFS,        ipstr_fuelValveToggle,     8);                  // APU MASTER  // @TODO: SUB TO DATA / LED
  buttons_1[P_DI_ENG_MST_1]           = BUTTON(kLOCAL_ENG_MST_STARTER_1,     _BINARY,     LOCAL, 0 ,                -999);    // SPECIAL LOGIC; LOCAL DEVICE VARIABLE (NEED TO SEND MULTIPLE SIMULATION EVENTS; HANDLE LOGIC IN SPAD)
  buttons_1[P_DI_ENG_MST_2]           = BUTTON(kLOCAL_ENG_MST_STARTER_2,     _BINARY,     LOCAL,  0 ,               -999);    // SPECIAL LOGIC; LOCAL DEVICE VARIABLE (NEED TO SEND MULTIPLE SIMULATION EVENTS; HANDLE LOGIC IN SPAD)
  buttons_1[P_DI_ENG_MODE_SS1]        = BUTTON(kLOCAL_ENG_MODE_SS_POS_CRANK, _BINARY,     LOCAL,  0 ,               -999);    // @TODO: IMPLEMENT PLACE HOLDER
  buttons_1[P_DI_ENG_MODE_SS2]        = BUTTON(kLOCAL_ENG_MODE_SS_POS_START, _BINARY,     LOCAL,  0 ,               -999);    // @TODO: IMPLEMENT PLACE HOLDER

  // MCP3 buttons
  buttons_2[P_DI_GPA0]                = BUTTON(kINVALID);
  buttons_2[P_DI_GPA1]                = BUTTON(kINVALID);
  buttons_2[P_DI_GPA2]                = BUTTON(kINVALID);
  buttons_2[P_DI_FOOT_BRAKE]          = BUTTON(kLOCAL_BRAKE_POS,             _BINARY,    LOCAL,        0 , -999);   // SPECIAL LOGIC
  buttons_2[P_DI_ANTISKID]            = BUTTON(kANTISKID,                    _TOGGLE,    SIMCONNECT,  ipstr_antiskid, -999);
  buttons_2[P_DI_BRAKE_PARK]          = BUTTON(kSimCommand,                  _TOGGLE,    SIMCONNECT,  ipstr_parkbrake, -999);  // SPECIAL LOGIC UNDER DATA SUB CALLBACK onChangeParkBrake()
  buttons_2[P_DI_GPA7_DO_NOT_USE]     = BUTTON(kINVALID);
  buttons_2[P_DI_GPB0]                = BUTTON(kINVALID);
  buttons_2[P_DI_FUEL_RT_PMP_2]       = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuel_pump, 6);
  buttons_2[P_DI_FUEL_RT_PMP_1]       = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuel_pump, 3);
  buttons_2[P_DI_FUEL_XFEED]          = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuelValveToggle, 1);
  buttons_2[P_DI_FUEL_CT_PMP_2]       = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuel_pump, 4);
  buttons_2[P_DI_FUEL_LT_PMP_2]       = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuel_pump, 5);
  buttons_2[P_DI_FUEL_LT_PMP_1]       = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuel_pump, 2);
  buttons_2[P_DI_FUEL_CT_PMP_1]       = BUTTON(kSimCommand,                 _PB_MOM,     MSFS,       ipstr_fuel_pump, 1);

 //APU MASTER OFF -ADD SIMCONNECT:APU_OFF_SWITCH EVENT
}

void init_DigitalPins() {
  mcp1.begin();   // MCP1 - D.O. only
  mcp2.begin(4);  // MCP2 - D.I. only
  mcp3.begin(2);  // MCP3 - D.O. + D.I.
  for (int i = 0; i < 16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp1.digitalWrite(i, LOW);  // turn on a 100K pullup internally

    mcp2.pinMode(i, INPUT);
    mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally

    mcp3.pinMode(i, INPUT);
    mcp3.pullUp(i, HIGH);  // turn on a 100K pullup internally

  }
    mcp3.pinMode(P_DO_AUTOBRAKE_MAX_LED, OUTPUT);
}

// Flag button state changes for later processing
void scanDigitalInputs()  {

  // MCP2 scan
  uint16_t val;
  val = mcp2.readGPIOAB();    //readGPIO(1)1);   // GPB
  val = ~val;
  currentVal = val;
  uint16_t change = (val ^ lastVal);
  for (int i = 0; i < 16; i++) {
    if (change & (uint16_t)round(pow(2, i))) { //BUTTONS_FLAG[i]) {
      buttons_1[i].change = true;
      buttons_1[i].currentVal = (currentVal & (uint16_t)round(pow(2, i))) >> i; //BUTTONS_FLAG[i]) >> i;
    }
  }
  lastVal = currentVal;

  // MCP3 scan
  val = mcp3.readGPIOAB();    //readGPIO(1)1);   // GPB
  val = ~val;
  currentVal_reg2 = val;
  change = (val ^ lastVal_reg2);
  for (int i = 0; i < 16; i++) {
    if ((buttons_2[i].id != kINVALID) && (change & (uint16_t)round(pow(2, i)))) { //BUTTONS_FLAG[i]) {
      buttons_2[i].change = true;
      buttons_2[i].currentVal = (currentVal_reg2 & (uint16_t)round(pow(2, i))) >> i; //BUTTONS_FLAG[i]) >> i;
    }
  }
  lastVal_reg2 = currentVal_reg2;

  // Execute commands for button state changes
  processChangeCommands(buttons_1);
  //Serial.println(currentVal, BIN);
  processChangeCommands(buttons_2);
  //Serial.println(currentVal_reg2, BIN);
}
