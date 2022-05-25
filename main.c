#include <msp430g2553.h>
#include "uklcd.h"  // <-- Where Magic Happens

/*
 * main.c
 */
int main(void) { // TODO: Delete This Comment

    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set range
    DCOCTL = CALDCO_1MHZ;

    WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
    P2DIR |= BIT0 + BIT3 + BIT4;              // Set P2.0(SER), P2.3(SCK) P2.4(RCK) to output direction

    volatile unsigned int ptr=0;

    initLCD();

    cursorDown();
    int k;
    char input[40]= "---------------E-------E";
    for(k=0;k<strlen(input);k++) writedata(input[k],1);
    scrollLeft();

    return 0;
}

