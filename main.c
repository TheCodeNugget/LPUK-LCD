#include <msp430g2553.h>
#include <stdio.h> // <== Required for sprintf
#include "uklcd.h"  // <== Where Magic Happens


/*
 * main.c
 */
int main(void) { // TODO: Delete This Comment

    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set range
    DCOCTL = CALDCO_1MHZ;

    WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
    P2DIR |= BIT0 + BIT3 + BIT4;              // Set P2.0(SER), P2.3(SCK) P2.4(RCK) to output direction

    char buffer[40]; //single line buffer to fit ST7076 Line Capacity

    volatile unsigned int ptr=0, charnum;

    initLCD();

    int k, lecCode = 1175, term = 2022;
    charnum = sprintf(buffer,"PEM - %d", lecCode); //Writes formatted String to given Char Array returns, string length Arguments(Char[], String, int) Important!: Limited to int
    for(k=0;k<charnum;k++) writedata(buffer[k],1);
    cursorDown();
    charnum = sprintf(buffer,"Spring - %d", term);
    for(k=0;k<charnum;k++) writedata(buffer[k],1);
    cursorOff();

    return 0;
}
