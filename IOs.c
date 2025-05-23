/*
 * File:   IOs.c
 * Author: khevinjugessur
 *
 * Created on October 21, 2022, 4:39 AM
 */

#include "xc.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"
#include "ADC.h"
#include "display.h"

extern uint16_t ADCOutput; //represents adcbuff value
extern int CNFlag; //cn interrupt flag


//// IOinit() with IO interrupts
void IOinit(void)
{
    //for led
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    //for push button 2
    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  // enable CN on CN0
    
    //for push button 3
    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RB4/CN0
    CNEN1bits.CN1IE = 1;  // enable CN on CN1
    
    //for push button 1
    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA2/CN0
    CNEN2bits.CN30IE = 1;  // enable CN on CN30
    
    //CN Interrupt settings
    IPC4bits.CNIP = 6; //set CN interrupt to 6
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    //setting up timer2
    T2CONbits.TSIDL = 0; //timer runs when the program is idle
    T2CONbits.T32 = 0; // use a 16 bit timer
    T2CONbits.TCS = 0; // allow use of internal clock
    return;
}


// IO Check to demo HW debugger

void IOcheck(void)
{
    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
    delay_ms(400,1);   // 400 msec delay to filter out debounces 
    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
    
    
    if ((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
    {
        
        CNFlag = 1; //set flag to 1
        
    }
    
    if ((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
    {
        CNFlag = 0; //set flag to 0
    }
    
    
    
    return;

}


///// Change of pin Interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
	IOcheck(); //check which push buttons are on or off and turns led accordingly
	IFS1bits.CNIF = 0;		// clear IF flag
	Nop();	//to help debouncing
    
    return;
}
