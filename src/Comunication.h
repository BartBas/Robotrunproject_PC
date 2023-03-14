/*
 * Comunication.h
 *
 *  Created on: 14 Mar 2023
*      Author: Bart Bas Elsenaar
 */

#ifndef COMUNICATION_H_
#define COMUNICATION_H_

#include <windows.h>

typedef struct Comunications{
	void (*Close)(struct Comunications *self);
	int val;
	int (*a)(struct Comunications *self, int val);
	HANDLE hComm;
	DWORD dNoOfBytesWritten;
	char lpBuffer[20];
	DCB dcb;

}Comunications;

Comunications commSetup();

void closeComms();

#endif /* COMUNICATION_H_ */
