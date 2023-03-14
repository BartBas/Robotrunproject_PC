#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Comunication.h"
//#include "Comunication.c"

int main(void) {
	puts("Start of program");

	Comunications myCom = commSetup();

	for (int i = 0; i < myCom.val; i++) {
		myCom.lpBuffer[i] = i + 1;
	}
	BOOLEAN status = myCom.Send(&myCom);

	// Create all variables required

	//HANDLE hComm;
/*
	int Buffer_Size = 20;
	DWORD dNoOfBytesWritten;
	char lpBuffer[Buffer_Size];
	BOOLEAN Status;

	DCB dcb;

	memset(&dcb, 0, sizeof(DCB));

	hComm = CreateFileA("\\\\.\\COM7",                //port name
			GENERIC_READ | GENERIC_WRITE, //Read/Write
			0,                            // No Sharing
			NULL,                         // No Security
			OPEN_EXISTING,                         // Open existing port only
			0,            // Non Overlapped I/O
			NULL);        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE) {

		printf("Error in opening serial port\n");
	}

	else {

		printf("opening serial port successful\n");

		GetCommState(hComm, &dcb);
		dcb.StopBits = 1;
		SetCommState(hComm, &dcb);
		printf("baud rate = %lu\n", dcb.BaudRate);
		printf("Byte Size = %d\n", dcb.ByteSize);
		printf("Parity = %d\n", dcb.Parity);
		printf("Stop Bit = %d\n", dcb.StopBits);

		//strncpy_s(lpBuffer,"hallo there world");

		for (int i = 0; i < 20; i++) {
			lpBuffer[i] = i + 1;
		}

		Status = WriteFile(hComm,        // Handle to the Serial port
				lpBuffer,     // Data to be written to the port
				Buffer_Size,  //No of bytes to write
				&dNoOfBytesWritten, //Bytes written
				NULL);
		if (Status) {
			printf("writen succesfully\n");
		} else {
			printf("didnt write\n");
		}

		printf("Made it to this place\n");
	}
	CloseHandle(hComm); //Closing the Serial Port
*/
	myCom.Close(&myCom);
	printf("EXIT_SUCCESFULL\n");
	return EXIT_SUCCESS;
}
