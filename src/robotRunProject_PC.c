#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Comunication.h"
#include "DEBUG_Prints.h"
//#include <time.h>

int main(void) {
	puts("Start of program");

	Communications myCom = commSetup();

	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = i + 1;
	}

	BOOLEAN status = myCom.Send(&myCom);

	DBprintf("hey there\n");
	myCom.Recieve(&myCom);
	printf("Recieved: ");
	for (int i = 0; i < myCom.val; i++) {
		printf("%d:%x ", i, myCom.Recieved[i]);
	}
	printf("\n");
	myCom.Close(&myCom);

	printf("EXIT_SUCCESFULL\n");
	return EXIT_SUCCESS;
}
