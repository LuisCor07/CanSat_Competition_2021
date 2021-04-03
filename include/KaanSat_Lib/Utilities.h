/*
 * KA'AN SAT TEAM PROPRIETARY INFORMATION - kaansat@soyunaq.mx
 *
 * Property of KA'AN SAT TEAM from Universidad Aeronautica en Queretaro,
 * Unauthorized reproduction and/or distribution in other universities
 * is strictly prohibited.
 *
 * Redistribution of code between KA'AN SAT TEAM members is permitted,
 * under the following conditions:
 *
 *  Redistribution of source code is under the KAANSAT TEAM name.
 *
 *  Any part of the software will not be used by other teams or
 *  organizations.
 *
 * -------------------------------------------------------------------
 *                           FILE DESCRIPTION
 * -------------------------------------------------------------------
 * Utilities.h
 *
 *  Created on: 16/01/2021
 *      Author: Enrique
 */

#ifndef INCLUDE_KAANSAT_LIB_UTILITIES_H_
#define INCLUDE_KAANSAT_LIB_UTILITIES_H

/* --------------- LIBRARIES ---------------*/
#include "FreeRTOS.h"
#include "os_task.h"
#include "sys_core.h"
#include "sci.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "strings.h"

/* ------------------- DECLARATIONS ------------------*/
#define COMM_SIZE       2048
#define T_TELEMETRY     1000
#define T_SENSORS       1000
#define LONG_CMD_KEY    15
#define LONG_SP_PARAM   4
#define TEAM_NUMBER     "1714"

#define spiREGCS        spiREG4

#define R_air           20.97
#define P_ISA           101325.0

enum STATES
{
    PRELAUNCH, LAUNCH, DEPLOYMENT, SP1_RELEASE, SP2_RELEASE, LANDING
};

enum CANSAT
{
    CONTAINER, PAYLOAD
};
/*------------------ TASKS -------------------------*/

void vWaitToStart(void *pvParameters);
void vTelemetry(void *pvParameters);
void vSensors(void *pvParameters);

xTaskHandle xWTStartHandle;
xTaskHandle xTelemetryHandle;

/* -------------- VARIABLES ----------------*/
extern char command[COMM_SIZE];
extern uint32 buff_size;

extern char CMD_KEY[LONG_CMD_KEY];

/* -------------- TELEMETRY ----------------*/
                /* CONTAINER */

extern float MISSION_TIME;
extern char cMISSION_TIME[6];

extern int PACKET_COUNT;
extern char cPACKET_COUNT[6];

extern char MODE;

extern char SP1_RELEASED;
extern char SP2_RELEASED;

extern uint8 STATE;
extern char *FSW_STATE[];

extern int SP1_PC;
extern char cSP1_PC[6];

extern int SP2_PC;
extern char cSP2_PC[6];

//      Sensors

extern float ALTITUDE_BAR;
extern char cALTITUDE_BAR[8];

extern float PRESS_BAR;
extern char cPRESS_BAR[8];

extern float TEMPERATURE;
extern char cTEMPERATURE[8];

extern float VOLT_BATT;
extern char cVOLT_BATT[4];

extern char cLAT[10];
extern char cLON[10];
extern char cALT[7];
extern char cNSATS[3];

                        /* PAYLOADS */

extern char SP1_MISSION_TIME[LONG_SP_PARAM];
extern char SP1_PACKET_COUNT[LONG_SP_PARAM];
extern char SP1_PACKET_TYPE[LONG_SP_PARAM];
extern char SP1_ALTITUDE[LONG_SP_PARAM];
extern char SP1_TEMPERATURE[LONG_SP_PARAM];
extern char SP1_ROTATION_RATE[LONG_SP_PARAM];

extern char SP2_MISSION_TIME[LONG_SP_PARAM];
extern char SP2_PACKET_COUNT[LONG_SP_PARAM];
extern char SP2_PACKET_TYPE[LONG_SP_PARAM];
extern char SP2_ALTITUDE[LONG_SP_PARAM];
extern char SP2_TEMPERATURE[LONG_SP_PARAM];
extern char SP2_ROTATION_RATE[LONG_SP_PARAM];
/*---------------- COMMAND VARIABLES ----------------*/
extern bool telemetry_ON;
extern bool SP_ON;
/* ------------------ FUNCTIONS --------------------*/

void createTelemetryPacket();
bool sciSendData(uint32 numOfDat, char* charDat, bool CR);
static void reverse(char *s, size_t s_len);
size_t sltoa(char *s, long int n);
void ftoa(float n, char *res, int afterpoint);


#endif /* INCLUDE_KAANSAT_LIB_UTILITIES_H_ */
