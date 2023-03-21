/*
 * Comunication.h
 *
 *  Created on: 14 Mar 2023
 *      Author: Bart Bas Elsenaar
 */

#ifndef COMUNICATION_H_
#define COMUNICATION_H_

#include <windows.h>

typedef struct Communications {
	void (*Close)(struct Communications *); 		// close handle
	BOOLEAN (*Send)(struct Communications *); 	// send msg
	int val;										// Lenght of array
	int (*a)(struct Communications *self, int val);	// debug function for val + val
	HANDLE hComm;									// storage of hComm handle
	DWORD dNoOfBytesWritten;						// Number of bytes send out
	char msgBuffer[26];								// Buffer for msg to be send
	DCB dcb;										// storage for DCB
	BOOLEAN (*Recieve)(struct Communications *self);
	char Recieved[26];

} Communications;

Communications commSetup();			// Default INIT of the Communications struct

#endif /* COMUNICATION_H_ */
