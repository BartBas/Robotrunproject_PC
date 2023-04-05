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

#define MSGLENGHT 26

#define START 010
#define STOP 101
//#define Buffer_Size 20

void closeComms(Communications *self) { // Closing the comunications to open the com-port
	CloseHandle(self->hComm);
	printf("closed the handle\n");
}



BOOLEAN sendACK(Communications *self){
	printf("SENDING ACK!");
	char tmp[3];
	tmp[0]=START;
	tmp[1]=255;
	tmp[2]=STOP;
	DWORD send;
	BOOLEAN Status = WriteFile(self->hComm,	// What port to use
			tmp,     			// Data to be written to the port
			3, 						// No of bytes to write
			&send, 		// Bytes written
			NULL);			// overlapping
if (send == 3 && Status){
	return TRUE;
}
else {
	return FALSE;
}
}
BOOLEAN Sendmsg(Communications *self) {	// Send a array of bytes over through the wixel
	self->msgBuffer[0]=START;
	self->msgBuffer[self->val-1]=STOP;
	BOOLEAN Status = WriteFile(self->hComm,	// What port to use
			self->msgBuffer,     			// Data to be written to the port
			self->val, 						// No of bytes to write
			&self->dNoOfBytesWritten, 		// Bytes written
			&self->stOverlapped_WRITE);			// overlapping

	printf("Sended bytes: ");

	for (int i = 0; i < self->val; i++) {
		printf("%d:%i ", i, self->msgBuffer[i]);
	}
	fflush(stdout);
	printf("\n");
	return Status;
}

int myfunc(Communications *self, int a) { 			// Debug function
	return self->val + a;
}

void clearbuffer(Communications *self,int Read){
	int needToClean = self->val-Read-2;
	printf("clearing buffer got %d so need to clean %d\n",Read,needToClean+2);
	DWORD read;
	for (int i=0;i<needToClean;i++)	{
		char temp;
	ReadFile(self->hComm,				// What port to use
			&temp,							// Where to write read Data
			1,									// Amount of bytes to read
			&read,								// Amount of bytes actually read
			0);
	}
}

BOOLEAN Recieve(Communications *self) { 			// Debug function
	DWORD read=0;
	for (int i = 0; i < self->val; i++) {
		self->Recieved[i] = i;
	}
	/*
	SetCommMask(self->hComm,read);
	WaitCommEvent(self->hComm,&self->Recieved,NULL);*/
	BOOLEAN status;
	char recieved;
	DWORD totalread=0;
	int i;
	int y=0;
	int temp[self->val];
	//printf("trying to recieve msg\n");
	fflush(stdout);
	for (i=0;i<self->val;i++)	{
	status = ReadFile(self->hComm,				// What port to use
			&recieved,							// Where to write read Data
			1,									// Amount of bytes to read
			&read,								// Amount of bytes actually read
			0);
	//Sleep(1L);
	self->Recieved[i]=recieved;
	if (read>0){
		temp[y]=recieved;
		y++;
	}
	totalread += read;
	}
	if (totalread==self->val) {
		self->newmsg = TRUE;
		//sendACK(self);
	        printf("File successfully read! %lu bytes read.\n", totalread);
		return TRUE;
	}else if (temp[0]==8&&temp[1]==-1&&temp[2]==101) {
		temp[0]=4;
		printf("msg send succesfully!");
		self->SendSuccesfull=TRUE;
		//clearbuffer(self,totalread);
		return FALSE;
	} else if (totalread>0) {
		//printf("Scambled msg only got %lu bytes now clearing %lu read.\n", totalread,self->val-totalread);
		for(int i=0;i<totalread;i++){
			printf("%d : %d ",i,self->Recieved[i]);
		}printf("\n");
		return FALSE;
	}else {
		//printf("mission failed we'll get them next time! %lu bytes read.\n", totalread);

		return FALSE;
	}
}

Communications commSetup(char comport[]) {		// Default INIT of the Communications struct
	//structure creation
	Communications myComms;
	myComms.newmsg=FALSE;
    OVERLAPPED oRead = { 0 };
    oRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	myComms.stOverlapped_READ= oRead;
	myComms.stOverlapped_WRITE= oRead;
	//DBprintf("made it here\n");
	// Creation of communication file

	myComms.hComm = CreateFileA(comport,	//port name
			GENERIC_READ | GENERIC_WRITE, 		//Read/Write
			0,                            		// No Sharing
			NULL,                         		// No Security
			OPEN_EXISTING,                      // Open existing port only
			NULL,            					// Non Overlapped I/O
			NULL);        						// Null for Comm Devices
    COMMTIMEOUTS timeouts = { 0, //interval timeout. 0 = not used
                              0, // read multiplier
                             10, // read constant (milliseconds)
                              0, // Write multiplier
                             10  // Write Constant
                            	};
	SetCommTimeouts(myComms.hComm,&timeouts);
	//if (myComms.hComm != INVALID_HANDLE_VALUE) {
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
	//}
	return myComms;
}

/*
 * send[8] 254 emergency stop
 * send[8] 250 reset emergency stop
 * send[8] 245 spin mode
 * send[8] 240 Manual mode OUTPUT 0 for button not pressed; 1 for button pressed. {[9] UP, [10] LEFT, [11] DOWN, [12] RIGHT}
 *
 *
 * Receive[0]=	START
 * Receive[1]= 	battery level % 	char
 * Receive[2]=	magazine process %	char
 * Receive[3]=	location x			char	Location of robot x;  set to -1 for home -2 for order completion
 * Receive[4]=	location y			char	Location of robot x;  set to -1 for home -2 for order completion
 * Receive[5]=	EmergancyStop		char	0 for clear; 1 for stop; 2 for spin; 3 for manual
 * Receive[max val] = STOP
 */

