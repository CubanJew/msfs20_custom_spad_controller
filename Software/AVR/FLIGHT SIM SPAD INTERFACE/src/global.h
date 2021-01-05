#pragma once
#ifndef _GLOBAL_H



enum
COMMAND_TYPE
{
  kRequest = 0, // Request from SPAD.neXt
  kCommand = 1, // Command to SPAD.neXt
  kEvent = 2, // Events from SPAD.neXt
  kDebug = 3, // Debug strings to SPAD.neXt Logfile
  kSimCommand = 4, // Send Event to Simulation
  kFUEL_PMP1 = 10, // CMDID for exposed data to SPAD.neXt
  kFUEL_PMP2 = 11,
  kFUEL_PMP3 = 12,
  kFUEL_PMP4 = 13,
  kFUEL_PMP5 = 14,
  kFUEL_PMP6 = 15,
  kFUEL_XFEED = 16,
  kLIGHT_ND = 17,  //??
  kANTISKID = 18,     // REMOVE???
  kAUTOBRAKE_CB_EN = 19,  // ground spoiler arm
  kPARKBRAKE_POS = 20,
  kAPU_GEN_POS = 21,
  kAPU_BLEED_POS = 22,
  kAPU_MASTER_POS = 23,
  kExtPower_POS = 24,
  kAPU_STARTER = 31,
  kGND_Spoiler_Arm = 36,

  kLOCAL_ENG_MST_STARTER_1 = 25,      // controller's switch position
  kLOCAL_ENG_MST_STARTER_2 = 26,      // controller's switch position
  kLOCAL_ENG_MODE_SS_POS_CRANK = 27,  // controller's switch position
  kLOCAL_ENG_MODE_SS_POS_START = 28,  // controller's switch position
  kLOCAL_PARK_BRAKE = 35,
  kLOCAL_BRAKE_POS = 32,
  kLOCAL_GND_SPOILER_ARM = 33,
  kLOCAL_ANTISKID = 34,


  kENG1_Master_Pos = 29,  //in-game ENG1 MASTER switch position
  kENG2_Master_Pos = 30,  //in-game ENG2 MASTER switch position





  kINVALID = 255,

};



#endif
