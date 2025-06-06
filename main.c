/*
 * File:   main.c
 * Author: khevinjugessur
 *
 * Created on November 9, 2022, 3:57 AM
 */

// MPLAB header libraries

#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files

#include "ChangeClk.h"
#include "ADC.h"
#include "IOs.h"
#include "UART2.h"
#include "display.h"
#include "TimeDelay.h"

// PRE-PROCESSOR DIRECTIVES - CONFIGURATION BITS

// Code protection 
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

//// WDT
//#pragma config FWDTEN = OFF // WDT is off
//#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
//#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
//#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = ON // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

//JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768 

// WDT ? CONFIGURATION BITS FWDT REGISTER
#pragma config FWDTEN = OFF // WDT is off ? lets user turn on WDT using SWDTEN bit
#pragma config WINDIS = ON // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // PR32 for 32 bit or PR128 for 128 bit prescaler
#pragma config WDTPS = PS4096 // PS1 ?> PS32768 for postscaler 1 -> 32768

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

// Global variables
int CNFlag = 0;
uint16_t ADCOutput = 0; // the output voltage of the ADC
//MAIN
int main(void) {

     
    // Change Clock of MCU - Adjust UART Baud rate on PC accordingly
     NewClk(500); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
     
    AD1PCFG = 0xFFFF; // Set to 0 on ADC on AN5
    InitUART2();      //Initialize UART settings and enable UART module
    IOinit();  //initialize IOs

    while(1)
    {
        
        ADCOutput = do_ADC();//get adcbuff value
        display(ADCOutput); //display on UART
        if(RCONbits.WDTO == 1){ //if WDT flag is 1
         LATBbits.LATB8 = 0; //turn led off
         RCONbits.WDTO = 0; //set WDT flag to 0
         RCONbits.SWDTEN = 0; //turn watchdog timer off
        }

    }
    
    return 0;
}

