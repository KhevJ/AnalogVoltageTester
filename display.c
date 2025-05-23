/*
 * File:   display.c
 * Author: khevinjugessur
 *
 * Created on November 10, 2022, 8:25 AM
 */

#include <xc.h>
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "ADC.h"
#include "UART2.h"
//#include "IOs.h"

extern int CNFlag; //cn interrupt flag

void display(uint16_t value){

    XmitUART2('\r', 1); //return to start line
    //if the value is too low
    if(value<250 && CNFlag == 1){
        Disp2Dec(value);//display adc value in dec
        Disp2String(" Voltage too low"); //display that the voltage is too low
        LATBbits.LATB8 = 1; //turn led on
        RCONbits.SWDTEN = 1; // turns on WDT
        Idle(); // put in idle mode
    }

    else{
        Disp2Dec(value);//display adc value in hex
        Disp2String(" Voltage is good"); //display that the voltage is good
    }
    
    delay_ms(2000,0); //delays by 2s for reading the display more easily
      
    return;
}
