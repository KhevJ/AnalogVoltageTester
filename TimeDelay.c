/*
 * File:   TimeDelay.c
 * Author: Rushi V
 *
 * Created on October 5, 2020, 4:05 PM
 */


#include "xc.h"
#include "TimeDelay.h"


void delay_ms(uint16_t time_ms, uint8_t idle_on)
{
    //T2CON config
    TMR2 = 0x1F3F;
    
    
    //configuring timer on
    T2CONbits.TON = 1; //setting timer on
    T2CONbits.TCKPS = 2; //setting preScaler to 1:64
    IPC1bits.T2IP = 0b110; //setting this interrupt to 5
    IEC0bits.T2IE = 1 ; //enabling interrupt 
    IFS0bits.T2IF = 0; //clear interrupt flag
    
    PR2 = time_ms * 4; //After PR2 simplification
    TMR2 = 0;
    T2CONbits.TON = 1; //start timer
    
    if(idle_on == 1)
    {
        Idle(); 
    }
    T2CONbits.TON=0; // Stop timer
    return;
}

// Used in HW IO control Driver project 3
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
     IFS0bits.T2IF=0; //Clear timer 2 interrupt flag
    
    // TMR2flag = 1; // optional global variable created by user
    return;
}
