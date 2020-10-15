#define _INIT_P_STR_(varName, varVal)  const char varName [] PROGMEM = varVal;


// COMMAND STRINGS
#define _PSTR_BUFFER_SIZE 45
// switches
_INIT_P_STR_(pstr_antiskid,           "ANTISKID BRAKES ACTIVE");
_INIT_P_STR_(pstr_fuelValveToggle,    "FUELSYSTEM_VALVE_TOGGLE");
_INIT_P_STR_(pstr_fuel_pump,          "FUELSYSTEM_PUMP_TOGGLE");
_INIT_P_STR_(pstr_parkbrake,          "PARKING_BRAKES");
_INIT_P_STR_(pstr_abrakeControlLevel, "SET_AUTOBRAKE_CONTROL");
_INIT_P_STR_(pstr_externalPower,      "TOGGLE_EXTERNAL_POWER");
_INIT_P_STR_(pstr_apuBleed,           "APU_BLEED_AIR_SOURCE_TOGGLE");
_INIT_P_STR_(pstr_apuGen,             "APU_GENERATOR_SWITCH_TOGGLE");
_INIT_P_STR_(pstr_spoilersArm,        "SPOILERS_ARM_SET");
_INIT_P_STR_(pstr_revThrustToggle,    "THROTTLE_REVERSE_THRUST_TOGGLE");
_INIT_P_STR_(pstr_parkBrakeState,     "BRAKE PARKING INDICATOR"); // sub (SIMCONNECT)
_INIT_P_STR_(pstr_apuGenSwitchPos,    "APU GENERATOR SWITCH"); // sub (SIMCONNECT)
_INIT_P_STR_(pstr_apuBleedSwitchPos,    "BLEED AIR SOURCE CONTROL"); // sub (SIMCONNECT)
_INIT_P_STR_(pstr_apuMasterSwitchPos,    "FUELSYSTEM VALVE SWITCH:8"); // sub (MSFS)
_INIT_P_STR_(pstr_extPowerSwitchPos,    "EXTERNAL POWER ON"); // sub (MSFS)
_INIT_P_STR_(pstr_apuStarter,           "APU_STARTER");   //SIMCONNECT:
//_INIT_P_STR_(pstr_engMasterSwitchPos, "GENERAL ENG STARTER ACTIVE:");   // sub (MSFS)


#define ipstr_antiskid           0
#define ipstr_fuelValveToggle    1
#define ipstr_fuel_pump          2
#define ipstr_parkbrake          3
#define ipstr_abrakeControlLevel 4
#define ipstr_externalPower      5
#define ipstr_apuBleed           6
#define ipstr_apuGen             7
#define ipstr_spoilersArm        8
#define ipstr_revThrustToggle    9
#define ipstr_parkBrakeState    10
#define ipstr_apuGenSwitchPos   11
#define ipstr_apuBleedSwitchPos 12
#define ipstr_apuMasterSwitchPos  13
#define ipstr_extPowerSwitchPos   14
#define ipstr_apuStarter          15
//#define ipstr_engMasterSwitchPos  15


const char* const PSTR_TABLE_CMDs[] PROGMEM = {pstr_antiskid, pstr_fuelValveToggle, pstr_fuel_pump, pstr_parkbrake,
  pstr_abrakeControlLevel, pstr_externalPower, pstr_apuBleed, pstr_apuGen, pstr_spoilersArm, pstr_revThrustToggle,
  pstr_parkBrakeState,
   pstr_apuGenSwitchPos, pstr_apuBleedSwitchPos, pstr_apuMasterSwitchPos,
   pstr_extPowerSwitchPos,
   pstr_apuStarter
 };

 //
