// Potentioemter inputs
#define P_AI_POT_PFD_ND_FD         A1  // FD/ND/FD brightness
#define P_AI_POT_ECAM              9   // ECAM brightness
#define P_AI_POT_PANEL             8   // panel lighting (overhead, main, pedastool)
#define P_AI_POT_FLOOD             A3  // panel flood lights
#define P_AI_POT_TBD               A0  // TO BE IMPLEMENTED
#define P_AI_POT_SPOILER           A2  // speed brake

/// LED outputs - MCP1
// OUTPUT mcp.digitalWrite() :   {} 0-7=GPA0:GPA7; 8-15=GPB7:GPB0} - BE CAUREFUL ABOUIT REASSIGNING INPUTS. CHECK CODE FOR IMPACT
#define P_DO_FUEL_PMP_CT_1_LED  7   // GPA7 pin 21  PMP 3
#define P_DO_FUEL_PMP_LT_1_LED  6   // GPA6 pin 22  PMP 1
#define P_DO_FUEL_PMP_LT_2_LED  5   // GPA5 pin 23  PMP 2
#define P_DO_EXT_PWR_LED        4 // GPA4 pin xx  EXT PWR
#define P_DO_BTN2_LED           3 // GPA3 pin xx  BTN2  (@TBD)
#define P_DO_BTN1_LED           2 // GPA2 pin xx  BTN1 (@TBD)

#define P_DO_AUTOBRAKE_LO_LED   1   // GPA1 pin xx  AUTOBRK-LO
#define P_DO_AUTOBRAKE_MED_LED  0   // GPA0 pin xx  AUTOBRK-MED
#define P_DO_FUEL_PMP_CT_2_LED  15  //8   // GPB7 pin xx  PMP 4
#define P_DO_FUEL_XFEED_LED     14   // GPB6 pin xx  X-FEED PMP
#define P_DO_FUEL_PMP_RT_1_LED  13  //10  // GPB5 pin xx  PMP 5
#define P_DO_FUEL_PMP_RT_2_LED  12 //14  //11  // GPB4 pin XX  PMP 6
#define P_DO_APU_GEN_LED        9 // B1
#define P_DO_APU_BLEED_LED      8 // B0
#define P_DO_APU_MASTER_LED     11// B3
#define P_DO_APU_STARTER_LED    10  // B2

// SWITCH INDEX POSITIONS - MCP2 (buttons_1)
#define P_DI_PWR_EXT             0 // GPA0
#define P_DI_TBD_2               1 // GPA1 - TO BE IMPLEMENTED
#define P_DI_TBD_1               2 // GPA2 - TO BE IMPLEMENTED
#define P_DI_REV_THRUST_TOG      3 // GPA3
#define P_DI_SPOILER_ARM         4 // GPA4
#define P_DI_AUTOBRAKE_LOW       5 // GPA5
#define P_DI_AUTOBRAKE_MED       6 // GPA6
#define P_DI_AUTOBRAKE_MAX       7 // GPA7
#define P_DI_APU_BLEED           8 // GPB0
#define P_DI_APU_GEN             9 // GPB1
#define P_DI_APU_START          10 // GPB2
#define P_DI_APU_MST            11 // GPB3
#define P_DI_ENG_MST_2          12 // GPB4
#define P_DI_ENG_MST_1          13 // GPB5
#define P_DI_ENG_MODE_SS2       14 // GPB6
#define P_DI_ENG_MODE_SS1       15 // GPB7

// SWITCH INDEX POSITION - MCP3 (buttons_2)
#define P_DO_AUTOBRAKE_MAX_LED      7 // GPA7 (MCP3 ONLY) - SPECIAL CASE!!!
//P_DI_GPA3
#define P_DI_GPA0                   0   // GPA0 - NOT IMPLEMENTED
#define P_DI_GPA1                   1   // GPA1 - NOT IMPLEMENTED
#define P_DI_GPA2                   2   // GPA2 - NOT IMPLEMENTED
#define P_DI_FOOT_BRAKE             3   // GPA3
#define P_DI_ANTISKID               4   // GPA4
#define P_DI_BRAKE_PARK             5   // GPA5 - !!!! IF RE-ASSIGN PIN OR MCP # THEN UPDATE onChangeParkBrake()
#define P_DI_GPA6                   6   // GPA6 - NOT IMPLEMENTED
#define P_DI_GPA7_DO_NOT_USE        7   // GPA7  - !!!!!!!! DO NOT USE!!!!!! USED AS OUTPUT

// NOTE: MCP B REGISTERS MAY BE WIRED IN REVERSE ORDER
#define P_DI_GPB0                  8 // B0 // 15 //B7 //8   // GPB0 - NOT IMPLEMENTED
#define P_DI_FUEL_RT_PMP_2         15 // B7 14 //B6 //9   // GPB1
#define P_DI_FUEL_RT_PMP_1         14 // B6 // 13 //B4+B5 //10  // GPB2
#define P_DI_FUEL_XFEED            13 // B5 // 12 //B4+B5 //11  // GPB3
#define P_DI_FUEL_CT_PMP_2         12 // B4// 11 //B3 //12  // GPB4
#define P_DI_FUEL_LT_PMP_2         11 // B3   // 10 //B2 //13  // GPB5
#define P_DI_FUEL_LT_PMP_1         10 // B2  // 9 //B1 //14  // GPB6
#define P_DI_FUEL_CT_PMP_1         9  // B1 // 8 //B0 //15  // GPB7



#define _CMD_PROVIDER(i) buttons[i].provider ? "MSFS:" + String(getPStr(buttons[i].cmdIndex)) : "SIMCONNECT:" + String(getPStr(buttons[i].cmdIndex))
