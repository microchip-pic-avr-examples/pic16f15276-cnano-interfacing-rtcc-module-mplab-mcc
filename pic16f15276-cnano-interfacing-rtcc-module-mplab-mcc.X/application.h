/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef RTC6_H
#define	RTC6_H

#ifdef	__cplusplus
extern "C" {
#endif

// include files
#include <time.h>
#include <bits/alltypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/i2c_host/mssp1.h"

typedef struct {
    int sec, min, hr;
    int year, month, date, day;
} DateTime_t;

/**
   @Param
    none
   @Returns
    none
   @Description
    Initializes all the Rtcc registers
   @Example
    none
 */
void rtc6_Initialize(void);

/**
   @Param
    time structure, alarm polarity, mask value
   @Returns
    none
   @Description
    Sets Alarm0 Time for user
   @Example
    none
 */
void rtc6_SetAlarm0(struct tm tm_t, bool almpol, uint8_t mask);

/**
   @Param
    time structure, alarm polarity, mask value
   @Returns
    none
   @Description
    Sets Alarm1 Time for user
   @Example
    none
 */
void rtc6_SetAlarm1(struct tm tm_t, bool almpol, uint8_t mask);

/**
   @Param
    Timestamp
   @Returns
    none
   @Description
    Sets the time parameter in the register
   @Example
    none
 */
void rtc6_SetTime(time_t);

/**
   @Param
    none
   @Returns
    long long timestamp
   @Description
    Gets timestamp from rtc registers
   @Example
    none
 */
time_t rtc6_GetTime(void);

/**
   @Param
    none
   @Returns
    none
   @Description
    Implements the functionality of Rtcc
   @Example
    none
 */
void rtc_Application(void);

/**
   @Param
    none
   @Returns
    none
   @Description
    It Clears the match condition after interrupt(alarm) happens
   @Example
    none
 */
void rtc6_ClearAlarm0(void);

/**
   @Param
    none
   @Returns
    none
   @Description
    It Clears the match condition after interrupt(alarm) happens
   @Example
    none
 */
void rtc6_ClearAlarm1(void);

/**
   @Param
    none
   @Returns
    none
   @Description
    Interrupt Routine for alarm match
   @Example
    none
 */
void Rtcc_External_Interrupt(void);

#ifdef	__cplusplus
}
#endif

#endif