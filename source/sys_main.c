/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/*----------------- OS Libraries --------------------*/
#include "FreeRTOS.h"
#include "os_task.h"
#include "sys_core.h"
#include "gio.h"
#include "sci.h"
/*------------- KA'AN SAT Libraries -----------------*/
#include "KaanSat_Lib/Utilities.h"
#include "KaanSat_Lib/Commands.h"
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
static char receivedData[20];
int i = 0;
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void sciNotification(sciBASE_t *sci, unsigned flags);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    gioInit();

    /* ---------------- SCI CONFIG----------------*/
    sciInit();
    sciEnableNotification(scilinREG, SCI_RX_INT);
    _enable_IRQ();
    _enable_interrupt_();
    sciReceive(scilinREG, 1, ( unsigned char *)receivedData);
    /* --------------- READ SD STATE --------------*/

    /* --------------- CREATE TASK --------------*/
    xTaskCreate(vTelemetry,"T. Container",512, NULL, 1, &xTelemetryHandle);
    xTaskCreate(vWaitToStart,"W.To S.",configMINIMAL_STACK_SIZE, NULL, 1, &xWTStartHandle);
    xTaskCreate(vSensors,"Sensores",configMINIMAL_STACK_SIZE, NULL, 1, &xWTStartHandle);

    vTaskSuspend(xTelemetryHandle);
    vTaskStartScheduler();
    while(1);
/* USER CODE END */
}


/* USER CODE BEGIN (4) */
void vWaitToStart(void *pvParameters)
{
    portTickType xWaitTime;
    xWaitTime = xTaskGetTickCount();

    while(1)
    {
        if(telemetry_ON)
        {
            __delay_cycles(106);
            vTaskResume(xTelemetryHandle);
            vTaskSuspend(NULL);
            __delay_cycles(106);
        }
        sciSendData(sprintf(command, "."), command, 0);
        vTaskDelayUntil(&xWaitTime, T_TELEMETRY/portTICK_RATE_MS);
    }
}

void vTelemetry(void *pvParameters)
{
    portTickType xTelemetryTime;
    xTelemetryTime = xTaskGetTickCount();

    while(1)
    {
        if(!telemetry_ON)
        {
            __delay_cycles(106);
            vTaskResume(xWTStartHandle);
            vTaskSuspend(NULL);
            __delay_cycles(106);
        }

        createTelemetryPacket();
        sciSendData(buff_size, command, 0);

        PACKET_COUNT++;
        SP1_PC++;
        SP2_PC++;

        vTaskDelayUntil(&xTelemetryTime, T_TELEMETRY/portTICK_RATE_MS);
    }
}

void vSensors(void *pvParameters)
{
    portTickType xSensorsTime;
    xSensorsTime = xTaskGetTickCount();
    while(1)
    {
        ALTITUDE_BAR = (pow((PRESS_BAR/P_ISA),(1/5.2559)) - 1)/(-1*0.0000225577);
        vTaskDelayUntil(&xSensorsTime, T_SENSORS/portTICK_RATE_MS);
    }
}

void sciNotification(sciBASE_t *sci, unsigned flags )
{
    sciReceive(scilinREG, 1, (unsigned char *)&receivedData);
    getCommand(receivedData[0]);
}


/* USER CODE END */
