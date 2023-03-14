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
#include <windows.h>

//#define Buffer_Size 20




void closeComms(Comunications *self){
	CloseHandle(self->hComm);
	printf("closed the handle\n");
}


int myfunc(Comunications *self, int a) {
	return self->val + a;
}

Comunications commSetup() {
	Comunications myComms;
	myComms.hComm = CreateFileA("\\\\.\\COM7",                //port name
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
		myComms.val = 9;
		myComms.Close = closeComms;

		memset(&myComms.dcb, 0, sizeof(DCB));

		printf("%d\n", myComms.val);
		printf("%d\n", myComms.a(&myComms, 2));

	};
	return myComms;
}
