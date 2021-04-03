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
 * Utilities.c
 *
 *  Created on: 16/01/2021
 *      Author: Enrique
 *
 */

#include "KaanSat_Lib/Utilities.h"

/* ------------------ VARIABLES ------------------*/
char command[COMM_SIZE] = {};
uint32 buff_size = 0;

char CMD_KEY[LONG_CMD_KEY] = {};

/* ------------------- TELEMETRY -----------------------*/
                    /* CONTAINER */
float MISSION_TIME = 0.0;
char cMISSION_TIME[6];

int PACKET_COUNT = 0;
char cPACKET_COUNT[6] = "PC";

char MODE = 'F';

char SP1_RELEASED = 'N';
char SP2_RELEASED = 'N';

extern uint8 STATE = PRELAUNCH;
char *FSW_STATE[] = {"PRELAUNCH","LAUNCH","DEPLOYMENT","SP1_RELEASE","SP2_RELEASE","LANDING"};

int SP1_PC = 0;
char cSP1_PC[6] = "P1";

int SP2_PC = 0;
char cSP2_PC[6] = "P2";

//       Sensors

float ALTITUDE_BAR = 0.0;
char cALTITUDE_BAR[8] = "h";

float PRESS_BAR = 1.0;
char cPRESS_BAR[8] = "PR";

float TEMPERATURE = 0.0;
char cTEMPERATURE[8] = "T";

float VOLT_BATT = 0.0;
char cVOLT_BATT[4] = "V";

char cLAT[10] = "LA" ;
char cLON[10] = "LO" ;
char cALT[7] = "HG" ;
char cNSATS[3] = "NS" ;

                    /* PAYLOADS */

char SP1_MISSION_TIME[LONG_SP_PARAM] = "X";
char SP1_PACKET_COUNT[LONG_SP_PARAM] = "X";
char SP1_PACKET_TYPE[LONG_SP_PARAM] = "X";
char SP1_ALTITUDE[LONG_SP_PARAM] = "X";
char SP1_TEMPERATURE[LONG_SP_PARAM] = "X";
char SP1_ROTATION_RATE[LONG_SP_PARAM] = "X";

char SP2_MISSION_TIME[LONG_SP_PARAM] = "X";
char SP2_PACKET_COUNT[LONG_SP_PARAM] = "X";
char SP2_PACKET_TYPE[LONG_SP_PARAM] = "X";
char SP2_ALTITUDE[LONG_SP_PARAM] = "X";
char SP2_TEMPERATURE[LONG_SP_PARAM] = "X";
char SP2_ROTATION_RATE[LONG_SP_PARAM] = "X";

/*---------------- COMMAND VARIABLES ----------------*/
bool telemetry_ON = false;
bool SP_ON = 0;
/* ------------ TELEMETRY FORMAT -------------------*/
//static const char* FORMAT = "1714,%s,C,%c,%c,%c,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s;1714,";
static const char* CONT_FORMAT = "%s,%s,%s,%s,%s,%s,%s,%s;";

/* ------------------ FUNCTIONS --------------------*/
void createTelemetryPacket()
{
    ftoa(PACKET_COUNT, cPACKET_COUNT, 0);
    ftoa(SP1_PC, cSP1_PC, 0);
    ftoa(SP2_PC, cSP2_PC, 0);

    ftoa(ALTITUDE_BAR, cALTITUDE_BAR, 1);
    ftoa(TEMPERATURE, cTEMPERATURE, 1);

    buff_size = sprintf(command,
                        CONT_FORMAT,                     /* <TELEMETRY_FORMAT> */
                        /* <MISSION_TIME> */
                        cPACKET_COUNT,              /* <PACKET_COUNT> */
                        /* <PACKET_TYPE> */
                        //MODE,                       /* <MODE> */
                        //SP1_RELEASED,               /* <SP1_RELEASED> */
                        //SP2_RELEASED,               /* <SP2_RELEASED> */
                        cALTITUDE_BAR,              /* <ALTITUD> */
                        //cTEMPERATURE,               /* <TEMP> */
                        /* <VOLTAGE> */
                        /* <GPS_TME> */
                        /* <GPS_LATITUDE> */
                        /* <GPS_LONGITUDE> */
                        /* <GPS_ALTITUDE> */
                        /* <GPS_SATS> */
                        //FSW_STATE[STATE],               /* <SOFTWARE_STATE> */
                        //cSP1_PC,                        /* <SP1_PACKET_COUNT> */
                        //cSP2_PC,                        /* <SP2_PACKET_COUNT> */
                        /* <CMD_ECHO> */
                        SP1_MISSION_TIME,               /* <MISSION_TIME> */
                        SP1_PACKET_COUNT,               /* <PACKET_COUNT> */
                        SP1_PACKET_TYPE,                /* <PACKET_TYPE> */
                        SP1_ALTITUDE,                   /* <ALTITUD> */
                        SP1_TEMPERATURE,                /* <TEMP> */
                        SP1_ROTATION_RATE              /* <SP_ROTATION_RATE> */
//                        SP2_MISSION_TIME,               /* <MISSION_TIME> */
//                        SP2_PACKET_COUNT,               /* <PACKET_COUNT> */
//                        SP2_PACKET_TYPE,                /* <PACKET_TYPE> */
//                        SP2_ALTITUDE,                   /* <ALTITUD> */
//                        SP2_TEMPERATURE,                /* <TEMP> */
//                        SP2_ROTATION_RATE               /* <SP_ROTATION_RATE> */
                        );
}

bool sciSendData(uint32 numOfDat, char* charDat, bool CR)
{
    sciSend(scilinREG, numOfDat, (uint8 *)charDat);
    if (CR)
        sciSend(scilinREG, 0x02, (unsigned char *)"\r\n");
    return true;
}

static void reverse(char *s, size_t s_len)
{
    size_t i, j;
    char swap;

    for (i = 0, j = s_len - 1; i < j; ++i, --j)
    {
        swap = s[i];
        s[i] = s[j];
        s[j] = swap;
    }
}

size_t sltoa(char *s, long int n)
{
    size_t i = 0;
    long int sign_mask;
    unsigned long int nn;

    sign_mask = n >> sizeof(long int) * 8 - 1;
    nn = (n + sign_mask) ^ sign_mask;
    do {
        s[i++] = nn % 10 + '0';
    } while (nn /= 10);

    s[i] = '-';
    i += sign_mask & 1;
    s[i] = '\0';

    reverse(s, i);
    return (i);
}

void ftoa(float n, char *res, int afterpoint)
{
    int ipart = (int)n, fpart = 0;
    /* Convertir parte entera */
    int i = sltoa(res, ipart );
    /* Agregar parte decimal */
    if(afterpoint > 0)
    {
        res[i] = '.';
        fpart = (int)(( n - (float)ipart ) * pow(10, afterpoint ));
        sltoa(&res[i+1], abs(fpart) );
    }
}
