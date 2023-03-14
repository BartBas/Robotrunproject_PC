/*
 * Communication.c
 *
 *  Created on: 14 Mar 2023
 *      Author: Bart Bas Elsenaar
 */
#include "Comunication.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DEBUG_Prints.h"
//#include <windows.h>

//#define Buffer_Size 20

void closeComms(Communications *self) { // Closing the comunications to open the com-port
	CloseHandle(self->hComm);
	printf("closed the handle\n");
}

BOOLEAN Sendmsg(Communications *self) {	// Send a array of bytes over through the wixel
	BOOLEAN Status = WriteFile(self->hComm,	// What port to use
			self->msgBuffer,     			// Data to be written to the port
			self->val, 						// No of bytes to write
			&self->dNoOfBytesWritten, 		// Bytes written
			NULL);

	printf("Sended bytes: ");

	for (int i = 0; i < self->val; i++) {
		printf("%d:%x ", i, self->msgBuffer[i]);
	}

	printf("\n");
	return Status;
}

int myfunc(Communications *self, int a) { 			// Debug function
	return self->val + a;
}
BOOLEAN Recieve(Communications *self) { 			// Debug function
	char Dump[self->val];
	DWORD read;
	BOOLEAN status;
	status = ReadFile(self->hComm,		//What port to use
			Dump,								// Where to write read Data
			self->val,							// Amount of bytes to read
			&read,								// Amount of bytes actually read
			NULL);

		if (status) {
		self->Recieve = Dump;
		return TRUE;
	} else {
		return FALSE;
	}
}

Communications commSetup() {		// Default INIT of the Communications struct
	//structure creation
	Communications myComms;

	DBprintf("made it here\n");
	// Creation of communication file
	myComms.hComm = CreateFileA("\\\\.\\COM7",	//port name
			GENERIC_READ | GENERIC_WRITE, 		//Read/Write
			0,                            		// No Sharing
			NULL,                         		// No Security
			OPEN_EXISTING,                      // Open existing port only
			0,            						// Non Overlapped I/O
			NULL);        						// Null for Comm Devices

	if (myComms.hComm == INVALID_HANDLE_VALUE) {

		printf("Error in opening serial port\n");

	} else {
		printf("opening serial port successful\n");

		/*
		 * Setting pointers and values inside the structure
		 */
		myComms.val = sizeof(myComms.msgBuffer) / sizeof(char);	// Size of buffer array
		myComms.Close = closeComms;						// Close Comms function
		myComms.Send = Sendmsg;							// Send array function
		myComms.Recieve = Recieve;						// Recieved msg

		memset(&myComms.dcb, 0, sizeof(DCB)); //mem inside the dcb

		DBprintf("%d\n", myComms.val);
	}
	return myComms;
}
