/* 
 * File:   embeddedShield.h
 * Author: Djibril Sall 
 *
 * Created on December 12, 2014, 5:03 PM
 */

#ifndef EMBEDDEDSHIELD_H
#define	EMBEDDEDSHIELD_H

#define LCD _RD2  // Defining LCD pin
#define LED _LATF0 // Defining LED pin

// Function prototypes
void Time(void);  // Including the time function
void IO(void);   // Including the IO function

#endif	/* EMBEDDEDSHIELD_H */


