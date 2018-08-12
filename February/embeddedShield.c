#include <p24EP512GU810.h>
#include "embeddedShield.h"

_FOSCSEL(FNOSC_PRIPLL)          // Primary clock is 8MHz Crystal with PLL
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_XT)
  // Clock Switching is enabled and Fail Safe Clock Monitor is disabled
  // OSC2 Pin Function: OSC2 is Clock Output
  // Primary Oscillator Mode: XT Crystal
_FWDT(FWDTEN_OFF) 		 // Watchdog Timer Enabled/disabled by user software
_FICD(JTAGEN_OFF & ICS_PGD1)    // Disable JTAG pins & Setup program pins
void Time(void)  // Timer functin 
{
    RCONbits.SWDTEN=0;                  // Disable Watch Dog Timer
    // Configure Oscillator to operate the device at 40Mhz
    // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    // Fosc= 8M*40/(2*2)=80Mhz for 8M input clock
    PLLFBD=38;				// M=40
    CLKDIVbits.PLLPOST=0;		// N1=2
    CLKDIVbits.PLLPRE=0;		// N2=2
    OSCTUN=0;				// Tune FRC oscillator, if FRC is used
    // Clock switching to incorporate PLL
    __builtin_write_OSCCONH(0x01);	// Initiate Clock Switch to Primary
					// Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01);	// Start clock switching
    while (OSCCONbits.COSC != 0b001);	// Wait for Clock switch to occur
    while(OSCCONbits.LOCK!=1) {};       // Wait for PLL to lock
}
void IO(void){  // IO function to define the IO's as well as their initial state
    _TRISD2 = 0x0;                      
    _TRISF0 = 0x0;
    LED=0;
    LCD=0;
}


