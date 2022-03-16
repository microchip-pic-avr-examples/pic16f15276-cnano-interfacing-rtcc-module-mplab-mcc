/*
 * File:   application.c
 * Author: I62418
 *
 * Created on November 30, 2021, 2:27 PM
 */

/*
 ***********************************************************
 *                                                         *
 *                  Include Header Files                   *
 *                                                         *
 ***********************************************************
*/
#include <xc.h>
#include "application.h"
#include "time.h"

// Internal RTCC and E2PROM Addresses
#define EEADDR                      (0x64)                                     // EEPROM ADDRESS
#define RTCC_ADDR                   (0x6F)                                     // RTCC REGISTER ADDRESS

// RTCC Time Parameter Addresses
#define RTCC_SECONDS                (0x00)                                     // CALENDAR & CLOCK REGISTER ADDRESSES
#define RTCC_MINUTES                (0x01)
#define RTCC_HOUR                   (0x02)
#define RTCC_DAY                    (0x03)
#define RTCC_DATE                   (0x04)
#define RTCC_MONTH                  (0x05)
#define RTCC_YEAR                   (0x06)

#define CONTROL_REG                 (0x07)                                     // CONTROL AND CALIBRATION REGISTER ADDRESSES
#define CALIBRATION                 (0x08)

// RTCC Alarm0 Registers
#define ALARM0_SECONDS              (0x0A)                                     // ALARM-0 REGISTER ADDRESSES
#define ALARM0_MINUTES              (0x0B)
#define ALARM0_HOUR                 (0x0C)
#define ALARM0_DAY                  (0x0D)
#define ALARM0_DATE                 (0x0E)
#define ALARM0_MONTH                (0x0F)

// RTCC Alarm1 Registers
#define ALARM1_SECONDS              (0x11)                                     // ALARM-1 REGISTER ADDRESSES 
#define ALARM1_MINUTES              (0x12)
#define ALARM1_HOUR                 (0x13)
#define ALARM1_DAY                  (0x14)
#define ALARM1_DATE                 (0x15)
#define ALARM1_MONTH                (0x16)

// RTCC Power Down Mode Registers
#define PWR_DOWN_MINUTES            (0x18)                                     // POWER DOWN MODE REGISTER ADDRESSES
#define PWR_DOWN_HOUR               (0x19)
#define PWR_DOWN_DATE               (0x1A)
#define PWR_DOWN_MONTH              (0x1B)

// RTCC Power Up Mode Registers
#define PWR_UP_MINUTES              (0x1C)                                     // POWER UP MODE REGISTER ADDRESSES
#define PWR_UP_HOUR                 (0x1D)
#define PWR_UP_DATE                 (0x1E)
#define PWR_UP_MONTH                (0x1F)


/******************************************************************************/

#define  SQWE                       (0x40)                                    //  Square Wave Enable BIT
#define  ALM_NO                     (0x00)                                    //  no alarm activated
#define  MFP_01H                    (0x00)                                    //  MFP = SQWAV(01 HERZ)
#define  OSCON                      (0x20)                                    //  state of the oscillator(running or not)
#define  VBATEN                     (0x08)                                    //  enable battery for back-up
#define  VBAT_CLR                   (0xEF)                                    //  Mask to clear VBAT flag BIT
#define  EXTOSC                     (0x08)                                    //  enable external 32khz signal
#define  ST_SET                     (0x80)                                    //  start oscillator

#define ALM_POL                     (0x07)                                    //  Alarm Polarity
#define ALM_MSK                     (0x04)                                    //  Alarm Mask
#define ALM0_EN                     (0x01)                                    //  Alarm0 enable
#define ALM1_EN                     (0x01)                                    //  Alarm1 enable
#define ALM0_NO                     (0x00)                                    //  Alarm0 disable
#define ALM1_NO                     (0x00)                                    //  Alarm1 disable

#define TIMESTAMP_LENGTH            (10)                                      //  Length of input timestamp
#define ARRAYSIZE                   (10)                                      //  LENGTH of input array of characters
#define ZERO                        (0)
#define WR_DELAY					(5)
#define RD_DELAY					(10)
#define HOLD_TIME					(1000)

/*
 ***********************************************************
 *                                                         *
 *              Static Function Declarations               *
 *                                                         *
 ***********************************************************
*/
static void rtcc_write(uint8_t addr, uint8_t data);
static uint8_t rtcc_read(uint8_t addr);

/*
 ***********************************************************
 *                                                         *
 *              User Defined Variables                     *
 *                                                         *
 ***********************************************************
*/
DateTime_t dateTime;
unsigned int arrIndex = ZERO;
unsigned char timeArray[ARRAYSIZE];
unsigned char userInput;
unsigned int setInterruptFlag = ZERO;
time_t usertimestamp;
volatile time_t readtime;
/*
 ***********************************************************
 *                                                         *
 *          User Defined Function Definitions              *
 *                                                         *
 ***********************************************************
*/

static uint8_t rtcc_read(uint8_t addr) 
{
    bool retStatus;
    uint8_t readByte;
    retStatus =  I2C1_WriteRead(RTCC_ADDR, &addr,1,&readByte,1);
    if (retStatus)
    {
        while (I2C1_IsBusy())
        {
            I2C1_Tasks(); 
        }      
    }
    return readByte;
}

static void rtcc_write(uint8_t addr, uint8_t data) 
{
    bool retStatus;
    uint8_t txBuffer[2] = {0};
    txBuffer[0] = addr;
    txBuffer[1] = data;
    retStatus = I2C1_Write(RTCC_ADDR, txBuffer, 2);
    if (retStatus)
    {
        while (I2C1_IsBusy())
        {
            I2C1_Tasks(); 
        }
    }
    __delay_ms(WR_DELAY);
}


// Initializes the RTC 6 Click board with default values and enabling Alarm0 mode on MFP Pin
void rtc6_Initialize(void) 
{
    uint8_t reg = 0;

    dateTime.day = (rtcc_read(RTCC_DAY) & 0x07);
    rtcc_write(RTCC_DAY, (dateTime.day | 0x08));
    dateTime.sec = rtcc_read(RTCC_SECONDS);

    rtcc_write(CONTROL_REG, ALM_NO + SQWE + MFP_01H);                        // SQWE=0, NO ALARM is enabled} 
    
    rtcc_write(RTCC_SECONDS, dateTime.sec | ST_SET);                         // Enable the external crystal oscillator 
    while (!reg) 
    {
        reg = rtcc_read(RTCC_DAY);
        reg &= OSCON;
    }    
    rtcc_write(RTCC_DAY, dateTime.day | (VBATEN & VBAT_CLR));                // Enable external battery and VBAT needs to be cleared
}

 void rtc6_EnableAlarms(bool alarm0, bool alarm1)
{
    uint8_t reg = rtcc_read(CONTROL_REG);
    reg = (reg & 0xCF) | (alarm0 << 4) | (alarm1 << 5);
    rtcc_write(CONTROL_REG, reg);
}

// Sets the component to respective address on the RTC 6 Click Board
static void rtc6_SetComponent(uint8_t location, uint8_t mask, uint8_t time)
{
    uint8_t inMemory = rtcc_read(location) & mask;
    rtcc_write(location, inMemory | (time % 10) | ((time / 10) << 4)); 
}

// Sets the time parameters to the respective variables in the time structure
void rtc6_SetTime(time_t t) 
{
    struct tm *tm_t;
    memset(tm_t, 0, sizeof (tm_t));

    tm_t = localtime(&t);
    rtc6_SetComponent(RTCC_YEAR, 0x00, tm_t->tm_year % 100);                 // RTC Click has only two digits for setting the year
    rtc6_SetComponent(RTCC_MONTH, 0xD0, tm_t->tm_mon + 1);                   // time.h gives January as zero, clock expects 1
    rtc6_SetComponent(RTCC_DATE, 0x00, tm_t->tm_mday);
    rtc6_SetComponent(RTCC_MINUTES, 0x00, tm_t->tm_min);
    rtc6_SetComponent(RTCC_SECONDS, 0x80, tm_t->tm_sec);
    rtc6_SetComponent(RTCC_HOUR, 0x00, tm_t->tm_hour);
}

// Gets the values from the RTC ^ registers to the local time structure
static uint8_t rtc6_GetComponent(uint8_t location, uint8_t mask)
{
    uint8_t working = rtcc_read(location) & mask;
    return (working & 0x0F) + (((working & (mask & 0xF0)) >> 4) * 10);
}

// Respective time parameters are set into the local structure to make it readable for users
time_t rtc6_GetTime(void) 
{
    struct tm tm_t;
    memset(&tm_t, 0, sizeof (tm_t));
    
    tm_t.tm_year = rtc6_GetComponent(RTCC_YEAR, 0xFF) + 100; // Result only has two digits, this assumes 20xx
    tm_t.tm_mon = rtc6_GetComponent(RTCC_MONTH, 0x1F) - 1; // time.h expects January as zero, clock gives 1
    tm_t.tm_mday = rtc6_GetComponent(RTCC_DATE, 0x3F);
    tm_t.tm_hour = rtc6_GetComponent(RTCC_HOUR, 0x3F);
    tm_t.tm_min = rtc6_GetComponent(RTCC_MINUTES, 0x7F);
    tm_t.tm_sec = rtc6_GetComponent(RTCC_SECONDS, 0x7F);

    return mktime(&tm_t);
}

void rtc6_SetAlarm0(struct tm tm_t, bool almpol, uint8_t mask)
{
    rtcc_write(ALARM0_SECONDS, tm_t.tm_sec);
    rtcc_write(ALARM0_MINUTES, tm_t.tm_min);
    rtcc_write(ALARM0_HOUR, tm_t.tm_hour);
    rtcc_write(ALARM0_DATE, tm_t.tm_mday);
    rtcc_write(ALARM0_MONTH, tm_t.tm_mon + 1);                               // time.h gives January as zero, clock expects 1
    rtcc_write(ALARM0_DAY, tm_t.tm_wday | almpol << ALM_POL | mask << ALM_MSK);
}

void rtc6_ClearAlarm0(void)
{
    uint8_t reg = rtcc_read(ALARM0_DAY);
    rtcc_write(ALARM0_DAY, reg);
}

void rtc6_SetAlarm1(struct tm tm_t, bool almpol, uint8_t mask)
{
    rtcc_write(ALARM1_SECONDS, tm_t.tm_sec);
    rtcc_write(ALARM1_MINUTES, tm_t.tm_min);
    rtcc_write(ALARM1_HOUR, tm_t.tm_hour);
    rtcc_write(ALARM1_DATE, tm_t.tm_mday);
    rtcc_write(ALARM1_MONTH, tm_t.tm_mon + 1);                               // time.h gives January as zero, clock expects 1
    rtcc_write(ALARM1_DAY, tm_t.tm_wday | almpol << ALM_POL | mask << ALM_MSK);
}

// Clears the previous alarm status if any at initialization
void rtc6_ClearAlarm1(void)
{
    uint8_t reg = rtcc_read(ALARM1_DAY);
    rtcc_write(ALARM1_DAY, reg);
}

// Interrupt Function for alarm match condition
void Rtcc_External_Interrupt(void)
{
    setInterruptFlag++;                                                      // To call up the clear register value function
    LED_Toggle();                                                            // Visual indicator
}

// Main Application Code
void rtc_Application(void) 
{
    if(EUSART1_IsRxReady()) 
    {        
        userInput=EUSART1_Read();
        switch(userInput)  
        {          
         case 'A':                                                          // Case-A defines the functionality for user to set the timestamp
            {
                printf("\n\t\r Option selected is : %c \n", userInput);
                printf("\n\t\r Enter the timestamp \n");
                printf("\n\t\r Format is in epochs (universal time ticks)\n");

                userInput = EUSART1_Read();
                
                if(arrIndex != TIMESTAMP_LENGTH)
                {
                    for (arrIndex=0;arrIndex<TIMESTAMP_LENGTH;arrIndex++) // Accepting epoch time from the data visualizer
                    {
                        timeArray[arrIndex] = userInput;                    // Adding up the characters received
                        userInput = EUSART1_Read();
                    }
                    usertimestamp = atoll(timeArray);
                    printf("\n\t\r seconds since the Epoch: %ld\n", (long) usertimestamp);
                    arrIndex = ZERO;
                    __delay_ms(RD_DELAY);
                    rtc6_SetTime(usertimestamp);                             // Writes the user defined timestamp to the registers
                }                
                printf("\nTime and Date is Set \n\n");
                break;
            }
         case 'B':                                                          // Case-B defines the functionality for user to set the alarm
            {
                printf("\n\t\r Option selected is : %c \n", userInput);
                struct tm *tm_t_alarm;
                rtc6_ClearAlarm0();
                rtc6_EnableAlarms(ALM0_EN,ALM1_NO);
                printf("\n\t\r Enter alarm time : \n");
                printf("\n\t\r Format is in epochs (universal time ticks) \n");                
                userInput = EUSART1_Read();
                
                if(arrIndex != TIMESTAMP_LENGTH)
                {
                    for (arrIndex=0;arrIndex<TIMESTAMP_LENGTH;arrIndex++) // Accepting epoch time from the data visualizer
                    {
                        timeArray[arrIndex] = userInput;                     // Adding up the characters received
                        userInput = EUSART1_Read();
                    }
                    usertimestamp = atoll(timeArray);
                    arrIndex = ZERO;
                    __delay_ms(RD_DELAY);
                    tm_t_alarm = localtime(&usertimestamp);
                    printf("\n\t\r Seconds since the Epoch: %ld\n", (long) usertimestamp);
                    rtc6_SetAlarm0(*tm_t_alarm, ALM_POL, ALM_MSK);           // Writes the alarm timestamp to the registers
                }
                printf("\n\t\r Alarm Time is Set \n\n");
                break;
            }
         case 'C':                                                           // Case-C defines the functionality for user to read the timestamp
            {
                printf("\n\t\r Reading Current Time \n\n");
                struct tm *read_tm_t;                                                      
                readtime = rtc6_GetTime();                                   // Reads the timestamp at the current instant
                read_tm_t = localtime(&readtime);
                printf("\t\r Current time is : %04d-%02d-%02d %02d:%02d:%02d\n", read_tm_t->tm_year+1900, read_tm_t->tm_mon+1, read_tm_t->tm_mday, read_tm_t->tm_hour, read_tm_t->tm_min, read_tm_t->tm_sec);
                __delay_ms(HOLD_TIME);
                break;
            }
         default:
            {
                break;
            }       
        }
    }
    
    // Checks if the interrupt flag is set to 2 for alarm message and also to  clear the alarm registers
    if(setInterruptFlag == 2)                                           
    {
        for(int i=0;i<3;i++)
        {
            printf("\n\t\r -------ALARM------- \n");                         // Display alarm message on terminal
        }                                                 
        rtc6_ClearAlarm0();                                                  // Clear Alarm Register
        setInterruptFlag = 0;                                                // Reset Interrupt Flag
    }
}