/*
 * uklcd.h - Basic Header For using Sitronix ST7066 LCD Controller on UsluKukla Booster V3.2 for TI MSP430 Launchpad
 *
 *  Created on: 25 May 2022
 *      Author: Ken The Nugget / Kyilmaz18
 *
 */

#ifndef UKLCD_H_
#define UKLCD_H_

// User Functions
void initLCD(); // Boots up and prepares LCD
void writedata(unsigned int data, int operation); // Write Char to Line
void clearLCD(); // Clears Screen
void cursorOn(); // Shows Cursor
void cursorOff(); // Hides Cursor
void cursorHome(); // Sets Cursor to Line-1 Char-1
void cursorLeft(); // Shifts Cursor 1-Char left
void cursorRight(); // Shifts Cursor 1-Char right
void cursorDown(); // Shifts Cursor 1-Line Down
void scrollLeft(); // Scrolls Line 1-Char Left Lines are scrolled together
void scrollRight(); // Scrolls Line 1-Char Right Lines are scrolled together

// Header Functions
void sendlcdata(int data, int I3, int I4); // Write Data to SN74HC
void pulseLCD(int operation,unsigned int data); // Pulse LCD to Register Data


// User Function Code Below
void initLCD() { // Initialise LCD According to Hitachi HD44780 Datasheet

    volatile unsigned int ptr;
    volatile unsigned int tmp;
    volatile unsigned int upper=240;//0x00F0;

    __delay_cycles(16000); // 15ms Ready Time (Refer to Datasheet)

    ptr=0x0030; // LCD Power-UP
    tmp=upper&ptr; // Move Command to Upper 4-Bits
    tmp=tmp>>4; // Shift Right 4 Times to Line Command With 2nd SN74 on UsluKukla
    sendlcddata(tmp, 0, 0); // Send Command to LCD
    pulseLCD(0,tmp); // Pulse LCD to Register Command
    __delay_cycles(4200); // Wait 4.1ms for power to stabilise

    ptr=0x0030; // Initialise LCD Module
    tmp=upper&ptr;
    tmp=tmp>>4;
    sendlcddata(tmp, 0, 0);
    pulseLCD(0,tmp);
    __delay_cycles(110); // Wait 100us for module bootup

    ptr=0x0030; // Initialise LCD Driver
    tmp=upper&ptr;
    tmp=tmp>>4;
    sendlcddata(tmp, 0, 0);
    pulseLCD(0,tmp);
    __delay_cycles(45); // Wait 40us for driver ready

    ptr=0x0020; // Power on display and set up interface
    tmp=upper&ptr;
    tmp=tmp>>4;
    sendlcddata(tmp, 0, 0);
    pulseLCD(0,tmp);
    __delay_cycles(45); // Wait 40us for interface ready

    ptr=0x0028; // Define Operation Mode, 4-Bit / 16x2 Display / 5x8 Font
    writedata(ptr, 0);
    __delay_cycles(45); // Wait Command Execute

    ptr=0x0006; // Incremental Entry Mode Set
    writedata(ptr, 0);
    __delay_cycles(45); // Wait Command Execute

    ptr=0x000F; // Display On
    writedata(ptr, 0);
    __delay_cycles(45); // Wait Command Execute

    ptr=0x0001; //clear display
    writedata(ptr, 0);
    __delay_cycles(2000); // Wait Command Register

    ptr=0x0002; //return home
    writedata(ptr, 0);
    __delay_cycles(2000); // Wait Command Execute

    ptr=0x0080; //set DDRAM address
    writedata(ptr, 0);
    __delay_cycles(2000); // Wait Command Execute
}

//if operation is zero ==> command ; if operation is one ==> data
//writes 8-bit data into 2 4-Bit Messages
void writedata(unsigned int data, int operation){ //Data is received in 8-Bit Format from char array

    // LCD Runs in 4-Bit Mode, 8-Bit Data is Sent in 2 Packets
    volatile unsigned int upper=240; //0x00F0 Upper 4 Bits
    volatile unsigned int lower=15; //0x000F Lower 4 Bits

        volatile unsigned int tmp=upper&data; //Isolate Upper 4 Bits of Data
        tmp=tmp>>4; // Shift 4 Bits right to send data to correct register of SN74
        sendlcddata(tmp, operation, 0);
        pulseLCD(operation,tmp); //Pulses LCD to refresh display with new data
        __delay_cycles(45); //Command Execution Time ~40us
        tmp=lower&data; //Isolate Lower 4 Bits of Data
        sendlcddata(tmp, operation, 0);
        pulseLCD(operation,tmp);
        __delay_cycles(45);
}

// LCD Command Set Below for Manipulation
void clearLCD() {
    writedata(0x0001, 0);
    __delay_cycles(2000); // Execution Length Depends on the Data in Buffer 80us-1.7ms
}

void cursorOn() {
    writedata(0x000E, 0);
    __delay_cycles(45);
}

void cursorOff() {
    writedata(0x000C, 0);
    __delay_cycles(45);
}

void cursorHome() {
    writedata(0x0002, 0);
    __delay_cycles(2000); // Execution Length Depends on the Data in Buffer 80us-1.7ms
}

void cursorLeft() {
    writedata(0x0010, 0);
    __delay_cycles(45); // All Cursor Shifts Take 40us
}

void cursorRight() {
    writedata(0x0014, 0);
    __delay_cycles(45); // All Cursor Shifts Take 40us
}

void cursorDown() { // CursorUP Not Available / Return to home and resposition
    writedata(0x00C0, 0);
    __delay_cycles(45); // All Cursor Shifts Take 40us
}

// ST7066 LCD Controller Holds 40 Chars per line Switches to next line on 41st Char
// Can Scroll Left Or Right To display out of screen chars
// Both Lines Scroll Together
void scrollLeft () { // Scrolls 1 Char Left
    writedata(0x0018, 0);
    __delay_cycles(45); // All Scroll Shifts Take 40us
}

void scrollRight() { // Scrolls 1 Char Right
    writedata(0x001C, 0);
    __delay_cycles(45); // All Scroll Shifts Take 40us
}

// Header Helper Code Below
void pulseLCD(int operation,unsigned int data){ // Pulse LCD for 12-Cycles to Register Data/Commands
    __delay_cycles(2);
    sendlcddata(data, operation, 1);
    __delay_cycles(2);
    sendlcddata(data, operation, 0);
    __delay_cycles(2);
}

// SN74HC To ST7066 Wiring of UsluKukla
//
//             IC102                      IC104
//            +------------+             +------------+
//            |            |             |            |     * 0's are Written Into IC102 and QA/QB of IC104
//        P2.0|SER       QA|      *------|SER       QA|     * to place the data and command bits onto correct register pins
//        P2.3|SRCLK     QB|      |  P2.3|SRCLK     QB|
//        P2.4|RCLK      QC|      |  P2.4|RCLK      QC|I3 <-- Mode Select Pin - 1 = Data Input / 0 = Command Input
//            |          QD|      |      |          QD|I4 <-- R/W Pin - 1 = Read From LCD / 0 = Read LCD
//            |          QE|      |      |          QE|D4 <-- Data Bus 4
//            |          QG|      |      |          QF|D5 <-- Data Bus 5
//            |          QF|      |      |          QG|D6 <-- Data Bus 6
//            |          QH|      |      |          QH|D7 <-- Data Bus 7
//            |         QH'|------*      |         QH'|
//            +------------+             +------------+
//             TI-SN74HC                  TI-SN74HC

void sendlcddata(int data, int I3, int I4){ // Writes into SN74HC
    volatile unsigned int sck=8;//0x0008;    //SCK is on
    volatile unsigned int temp=1;//0x0001;
    int i=0;
    volatile unsigned int tmp=0;

    for(i=0;i<4;i++){ // Prepare Data to be written into SN74HC
        tmp=temp << 3-i;
        tmp=tmp&data;
        tmp=tmp >> 3-i;
        P2OUT=tmp;
        __delay_cycles(1);
        P2OUT=sck|tmp;    //SCK is on
        __delay_cycles(1);
        P2OUT=tmp;    //SCK is off
        __delay_cycles(1);
    }

    // ADD I4 Command to the end of Data Message
    P2OUT=I4;
    __delay_cycles(1);
    P2OUT=sck|I4;    //SCK is on
    __delay_cycles(1);
    P2OUT=I4;    //SCK is off
    __delay_cycles(1);

    // ADD I3 Command to the end of Data Message
    P2OUT=I3;
    __delay_cycles(1);
    P2OUT=sck|I3;    //SCK is on
    __delay_cycles(1);
    P2OUT=I3;    //SCK is off
    __delay_cycles(1);

    // Add 0 to fill 8-Bits
    P2OUT=0;
    __delay_cycles(1);
    P2OUT=sck;    //SCK is on
    __delay_cycles(1);
    P2OUT=0;    //SCK is off
    __delay_cycles(1);

    // Add 0 to fill 8-Bits
    P2OUT=0;
    __delay_cycles(1);
    P2OUT=sck;    //SCK is on
    __delay_cycles(1);
    P2OUT=0;    //SCK is off
    __delay_cycles(1);

    for(i=0;i<8;i++){ // Sends 1-Byte of 0's to fill up 1st SN74 -> Pushes Message to Second Register
        P2OUT=0;
        __delay_cycles(1);
        P2OUT=sck;    //SCK is on
        __delay_cycles(1);
        P2OUT=0;    //SCK is off
        __delay_cycles(1);
    }

    P2OUT=0b00010000;  //RCK is on -> Transmits Registered Data
    __delay_cycles(1);
    P2OUT=0;    //RCK is off -> Transmit Complete
    __delay_cycles(1);
}

#endif /* UKLCD_H_ */
