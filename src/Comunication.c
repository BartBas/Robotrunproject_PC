/*
 * Comunication.c
 *
 *  Created on: 14 Mar 2023
 *      Author: Bart Bas Elsenaar
 */
#include "Comunication.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>

//#define Buffer_Size 20




void closeComms(Comunications *self){
	CloseHandle(self->hComm);
	printf("closed the handle\n");
}

BOOLEAN Sendmsg(Comunications *self){
	BOOLEAN Status = WriteFile(self->hComm,        // Handle to the Serial port
			self->lpBuffer,     // Data to be written to the port
			self->val,  //No of bytes to write
			&self->dNoOfBytesWritten, //Bytes written
			NULL);
	printf("Sended : ");
	for (int i = 0; i < self->val; i++)
	{
	    printf("%x ", self->lpBuffer[i]);
	}
	printf("\n");
	return Status;
}

int myfunc(Comunications *self, int a) {
	return self->val + a;
}

Comunications commSetup() {
	Comunications myComms;
	printf("made it here\n");

	myComms.hComm = CreateFileA("\\\\.\\COM5",                //port name
			GENERIC_READ | GENERIC_WRITE, //Read/Write
			0,                            // No Sharing
			NULL,                         // No Security
			OPEN_EXISTING,                         // Open existing port only
			0,            // Non Overlapped I/O
			NULL);        // Null for Comm Devices
	if (myComms.hComm == INVALID_HANDLE_VALUE) {

		printf("Error in opening serial port\n");

	} else {
		printf("opening serial port successful\n");
		myComms.val = 21;
		myComms.Close = closeComms;
		myComms.Send = Sendmsg;

		memset(&myComms.dcb, 0, sizeof(DCB));

		printf("%d\n", myComms.val);
	}
	return myComms;
}
