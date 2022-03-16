/**
 * Generated Driver File
 * 
 * @file pins.c
 * 
 * @ingroup  pinsdriver
 * 
 * @brief This is generated driver implementation for pins. 
 *        This file provides implementations for pin APIs for all pins selected in the GUI.
 *
 * @version Driver Version 3.0.0
*/

/*
� [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../pins.h"

void (*RD6_InterruptHandler)(void);

void PIN_MANAGER_Initialize(void)
{
   /**
    LATx registers
    */
    LATA = 0x0;
    LATB = 0x0;
    LATC = 0x0;
    LATD = 0x0;
    LATE = 0x0;

    /**
    TRISx registers
    */
    TRISA = 0xFF;
    TRISB = 0xFE;
    TRISC = 0xFF;
    TRISD = 0xFF;
    TRISE = 0xB;

    /**
    ANSELx registers
    */
    ANSELA = 0xFF;
    ANSELB = 0xFC;
    ANSELC = 0xE7;
    ANSELD = 0xBF;
    ANSELE = 0x3;

    /**
    WPUx registers
    */
    WPUA = 0x0;
    WPUB = 0x0;
    WPUC = 0x18;
    WPUD = 0x40;
    WPUE = 0x0;
  

    /**
    APFCONx registers
    */

    /**
    ODx registers
    */
   
    ODCONA = 0x0;
    ODCONB = 0x0;
    ODCONC = 0x0;
    ODCOND = 0x0;
    ODCONE = 0x0;
    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;
    SLRCOND = 0xFF;
    SLRCONE = 0x7;
    /**
    INLVLx registers
    */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xFF;
    INLVLD = 0xFF;
    INLVLE = 0xF;

    /**
    PPS registers
    */
    RX1PPS = 0x9; //RB1->EUSART1:RX1;
    RB0PPS = 0x05;  //RB0->EUSART1:TX1;
    SSP1CLKPPS = 0x13;  //RC3->MSSP1:SCL1;
    RC3PPS = 0x07;  //RC3->MSSP1:SCL1;
    SSP1DATPPS = 0x14;  //RC4->MSSP1:SDA1;
    RC4PPS = 0x08;  //RC4->MSSP1:SDA1;

   /**
    IOCx registers 
    */
    IOCAP = 0x0;
    IOCAN = 0x0;
    IOCAF = 0x0;
    IOCBP = 0x0;
    IOCBN = 0x0;
    IOCBF = 0x0;
    IOCCP = 0x0;
    IOCCN = 0x0;
    IOCCF = 0x0;
    IOCDP = 0x40;
    IOCDN = 0x0;
    IOCDF = 0x0;
    IOCEP = 0x0;
    IOCEN = 0x0;
    IOCEF = 0x0;

    RD6_SetInterruptHandler(RD6_DefaultInterruptHandler);

}
  
void PIN_MANAGER_IOC(void)
{
    // interrupt on change for pin RD6}
    if(IOCDFbits.IOCDF6 == 1)
    {
        RD6_ISR();  
    }
}
   
/**
   RD6 Interrupt Service Routine
*/
void RD6_ISR(void) {

    // Add custom IOCDF6 code

    // Call the interrupt handler for the callback registered at runtime
    if(RD6_InterruptHandler)
    {
        RD6_InterruptHandler();
    }
    IOCDFbits.IOCDF6 = 0;
}

/**
  Allows selecting an interrupt handler for IOCDF6 at application runtime
*/
void RD6_SetInterruptHandler(void (* InterruptHandler)(void)){
    RD6_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCDF6
*/
void RD6_DefaultInterruptHandler(void){
    // add your RD6 interrupt custom code
    // or set custom function using RD6_SetInterruptHandler()
}
/**
 End of File
*/