#include <stdio.h>
#include <stdlib.h>

//#include "interface.h"
#include "Comunication.h"
#include "DEBUG_Prints.h"
#include "windows.h"
#include <process.h>
#include <Winbase.h>
#include <wchar.h>
#include <math.h>

#define BUTTON

#define WIN32_LEAN_AND_MEAN

//GLOBALS
Communications myCom;
RobotStats bot;
HWND Static, Window, dialogEdit, Robotstats, RobotBat, RobotMP, RobotLocx,
RobotLocy, RobotState;

BOOL repeat = 1,sending =0;

enum {SW=9,SA=10,SS=11,SD=12};

//PROTOTYPES
void (*buttons[80])(HWND, WPARAM);
void registerDialogClass(HINSTANCE);
void DisplayDialog(HWND);
void sendLoop();
LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam,LPARAM LParam);

// CODE
void defaultbutton(HWND Parent, WPARAM WParam) {
	printf("hey\n");
	SetWindowText(Static, "NEW TEXT HAS ARIVED!");
	fflush(stdout);
}

void defaultcheckBT(HWND Parent, WPARAM WParam) {
	SetWindowText(Static, "button was checked");
	char temp[10];
	char MSG[50] = "Place ";
	sprintf(temp, "%d", WParam);
	strcat(MSG, temp);
	BOOL checked = IsDlgButtonChecked(Parent, WParam);
	if (checked) {
		CheckDlgButton(Parent, WParam, BST_UNCHECKED);
		strcpy(temp, " was unchecked");
		strcat(MSG, temp);
		SetWindowText(Static, MSG);
	} else {
		CheckDlgButton(Parent, WParam, BST_CHECKED);
		strcpy(temp, " was checked");
		strcat(MSG, temp);
		SetWindowText(Static, MSG);
	}
	fflush(stdout);
}

void UpdateDOWN(int i) {
	printf("%d is down!\n",i);
	myCom.msgBuffer[i] = 1;
}

void UpdateUP(int i) {
	printf("%d is UP!\n",i);
	myCom.msgBuffer[i] = 0;
}

void UIQuit() {
	char temp[26] = "cccccccccccccccccccccccccc";
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = temp[i];
	}
	myCom.Send(&myCom);
}

void send4tumes() {
	myCom.Send(&myCom);
	myCom.Send(&myCom);
	myCom.Send(&myCom);
	myCom.Send(&myCom);
}
void EMERGACYSTOP() {
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = 254;
	}
	send4tumes();
}


void SPINMODE() {
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = 245;
	}

	send4tumes();
}

void MANUALMODE() {
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = 240;
	}

	myCom.msgBuffer[9] = 0;
	myCom.msgBuffer[10] = 0;
	myCom.msgBuffer[11] = 0;
	myCom.msgBuffer[12] = 0;
	send4tumes();

	if (sending ==1){
		SetWindowText(Static, "MANUAL CONTROL OFF!");
		sending =0;
	}else if (sending ==0){
		SetWindowText(Static, "MANUAL CONTROL ON!");
		sending =1;
	}
	//repeat 	=0;
}

void printBits(size_t const size, void const *const ptr) {
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}

unsigned char reverseBits(unsigned char num) {
	unsigned char NO_OF_BITS = sizeof(num) * 8;
	unsigned char reverse_num = 0;
	int i;
	for (i = 0; i < NO_OF_BITS; i++) {
		if ((num & (1 << i)))
			reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
	}
	return reverse_num;
}

void updateStatsDisplay() {
	char str[4];
	sprintf(str, "%d", bot.BatLVL);
	SetWindowText(RobotBat, str);
	sprintf(str, "%d", bot.MAGproc);
	SetWindowText(RobotMP, str);
	sprintf(str, "%d", bot.LocX);
	SetWindowText(RobotLocx, str);
	sprintf(str, "%d", bot.LocY);
	SetWindowText(RobotLocy, str);
	sprintf(str, "%d", bot.State);
	SetWindowText(RobotState, str);
	if(bot.State==3){

	}else{
		//sending =0;
	}
}

void createOrder(HWND Parent, WPARAM _) {
	unsigned int send = 0;
	printBits(sizeof(int), &send);
	int temp[25];
	int counter = 0;
	for (int i = 0; i < 25; i++) {
		if (IsDlgButtonChecked(Parent, i) == BST_CHECKED) {
			temp[counter] = i;
			counter++;
			send++;
			send <<= 1;
		} else {
			send <<= 1;
		}

	}
	send >>= 1;
	printBits(sizeof(int), &send);
	char totalchar[50] = "Send to places: ";
	char tempstr[10];
	for (int i = 0; i < counter; i++) {
		printf(" place: %d\n", temp[i]);
		sprintf(tempstr, "%d", temp[i]);
		strcat(totalchar, tempstr);
		strcat(totalchar, "  ");
	}
	unsigned char sendbyte1 = 0;
	unsigned char sendbyte2 = 0;
	unsigned char sendbyte3 = 0;
	unsigned char sendbyte4 = 0;
	sendbyte4 = reverseBits(sendbyte4 | send);
	send >>= 8;
	sendbyte3 = reverseBits(sendbyte3 | send);
	send >>= 8;
	sendbyte2 = reverseBits(sendbyte2 | send);
	send >>= 8;
	sendbyte1 = sendbyte1 | send;
	printf("send byte1: ");
	printBits(sizeof(char), &sendbyte1);
	printf("send byte2: ");
	printBits(sizeof(char), &sendbyte2);
	printf("send byte3: ");
	printBits(sizeof(char), &sendbyte3);
	printf("send byte4: ");
	printBits(sizeof(char), &sendbyte4);
	if (sendbyte1 == 0 && sendbyte2 == 0 && sendbyte3 == 0 && sendbyte4 == 0) {
		SetWindowText(Static, "Order Canceled due no places where set");
		return;
	}
	SetWindowText(Static, totalchar);
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = 0;
	}
	myCom.msgBuffer[1] = sendbyte1;
	myCom.msgBuffer[2] = sendbyte2;
	myCom.msgBuffer[3] = sendbyte3;
	myCom.msgBuffer[4] = sendbyte4;
	myCom.Send(&myCom);
}

void createButton(HWND Parent, wchar_t Name[], int x, int y, int w, int h,
		int number) {
	HWND button = CreateWindowW(L"button",                	//TYPE
			Name,//Text on button
			WS_VISIBLE | WS_CHILD,//What is it
			x, y,						// X,Y position
			w, h,						//Width , Height
			Parent,//Parent
			(HMENU)number,//HNDL
			NULL, NULL);
	buttons[number] = defaultbutton;
}

void createCheckBX(HWND Parent, wchar_t Name[], int x, int y, int w, int h,
		int number) {
	CreateWindowW(L"button",                	//TYPE
			Name,//Text on button
			WS_VISIBLE | WS_CHILD | BS_CHECKBOX,//What is it
			x, y,						// X,Y position
			w, h,						//Width , Height
			Parent,//Parent
			(HMENU)number,//HNDL
			NULL, NULL);
	CheckDlgButton(Parent, number, BST_UNCHECKED);
	buttons[number] = defaultcheckBT;
}

HWND createStatic(HWND Parent, wchar_t Name[], int x, int y, int w, int h) {
	return CreateWindowW(L"static",                	//TYPE
			Name,//Text on button
			WS_VISIBLE | WS_CHILD,//What is it
			x, y,						// X,Y position
			w, h,						//Width , Height
			Parent,//Parent
			NULL, NULL, NULL);

}

void AddControls(HWND Parent) {
	// GENERATE CHECK BOXES
	int counter = 0;
	for (int x = 5; x > 0; x--) {
		for (int y = 5; y > 0; y--) {
			wchar_t vOut[12];
			_itow_s(counter, vOut, sizeof(vOut) / 2, 10);
			createCheckBX(Parent, vOut, ((50 * y) - 40), ((50 * x) - 40), 35,
					13, counter);
			counter++;
		}
	}
	// GENERATE BUTTONS
	createButton(Parent, L"Send Order", 350, 100, 100, 20, counter);
	buttons[counter] = createOrder;
	counter++;

	createButton(Parent, L"EMERGENCY STOP", 350, 50, 150, 30, counter);
	buttons[counter] = EMERGACYSTOP;
	counter++;

	createButton(Parent, L"SPIN MODE", 350, 200, 100, 20, counter);
	buttons[counter] = SPINMODE;
	counter++;

	// GENERATE STATICS
	Static = createStatic(Parent,
			L"3PI ROBOT CONTROLLER THROUGH THE POWER OF THE WIXEL!", 10, 300,
			650, 50);

	int LOCHeight = 100, LOCWidth = 500,

			//these numbers have to be a equal number
			statwidth = 75, statheight = 20, nrWidth = 20;

	Robotstats = createStatic(Parent, L"    ROBOT STATS", LOCWidth, LOCHeight,
			statwidth * 2, statheight);
	LOCHeight += statheight;
	createStatic(Parent, L"Bat lvl:", LOCWidth, LOCHeight, statwidth - nrWidth,
			statheight);
	createStatic(Parent, L"Done:", LOCWidth + statwidth, LOCHeight,
			statwidth - nrWidth, statheight);

	RobotBat = createStatic(Parent, L"99", LOCWidth + statwidth - nrWidth,
			LOCHeight, nrWidth, statheight);
	RobotMP = createStatic(Parent, L"99",
			LOCWidth + statwidth + statwidth - nrWidth, LOCHeight, nrWidth,
			statheight);

	LOCHeight += statheight;
	createStatic(Parent, L"Loc x:", LOCWidth, LOCHeight, statwidth - nrWidth,
			statheight);
	createStatic(Parent, L"Loc y:", LOCWidth + statwidth, LOCHeight,
			statwidth - nrWidth, statheight);

	RobotLocx = createStatic(Parent, L"99", LOCWidth + statwidth - nrWidth,
			LOCHeight, nrWidth, statheight);
	RobotLocy = createStatic(Parent, L"99",
			LOCWidth + statwidth + statwidth - nrWidth, LOCHeight, nrWidth,
			statheight);

	LOCHeight += statheight;
	RobotState = createStatic(Parent, L"State", LOCWidth, LOCHeight,
			statwidth * 2, statheight);

	createButton(Parent, L"MANUAL CONTROL", 500, 260, 200, 20, counter);
	buttons[counter] = MANUALMODE;
	counter++;
}

int counter = 0;
LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam,
		LPARAM LParam) {
	switch (Message) {
	case WM_KEYDOWN: {
	        switch (WParam) {
	        case VK_UP:
	        case 'W': {
	            UpdateDOWN(SW);
	            break;
	        }
	        case VK_LEFT:
	        case 'A': {
	            UpdateDOWN(SA);
	            break;
	        }
	        case VK_DOWN:
	        case 'S': {
	            UpdateDOWN(SS);
	            break;
	        }
	        case VK_RIGHT:
	        case 'D': {
	            UpdateDOWN(SD);
	            break;
	        }
	            break;
	        }
	    }
	        break;
	    case WM_KEYUP: {
	        switch (WParam) {
	        case VK_SPACE: {
	            createOrder(Window,WParam);
	        }
	            break;
	        case VK_UP:
	        case 'W': {
	            UpdateUP(SW);
	            break;
	        }
	        case VK_LEFT:
	        case 'A': {
	            UpdateUP(SA);
	            break;
	        }
	        case VK_DOWN:
	        case 'S': {
	            UpdateUP(SS);
	            break;
	        }
	        case VK_RIGHT:
	        case 'D': {
	            UpdateUP(SD);
	            break;
	        }
	            break;
	        }
	    }
	        break;
	case WM_COMMAND: {
		buttons[WParam](Window, WParam);
		SetFocus(Window);
		break;
	}
	case WM_CREATE: {
		AddControls(Window);
		DisplayDialog(Window);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
	}
	break;
	default: {
		return DefWindowProc(Window, Message, WParam, LParam);
	}
	}
	return 0;
}

int DisplayNoComportMessageBox() {
	int msgboxID =
			MessageBox(
					NULL, "Filled in COM port was incorrect", "COM port not found",
					MB_ICONEXCLAMATION | MB_RETRYCANCEL | MB_DEFBUTTON2);

	switch (msgboxID) {
	case IDCANCEL:
		return 0;
		break;
	case IDTRYAGAIN:
		return 1;
		break;
	}
	return msgboxID;
}

LRESULT CALLBACK DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CLOSE:
		EnableWindow(Window, TRUE);
		DestroyWindow(hWnd);
		exit(1);
		break;
	case WM_COMMAND: {
		switch (wp) {
		case 1: {
			printf("1\n");
			exit(0);
			break;
		}
		case 2: {
			printf("2\n");
			char inputComPort[4];
			GetWindowText(dialogEdit, inputComPort, 4);
			printf("\n");
			printf(inputComPort);
			printf("\n");
			char CommPortstr[] = "\\\\.\\COM";
			strcat(CommPortstr, inputComPort);
			printf("\n");
			printf(CommPortstr);
			printf("\n");
			fflush(stdout);
			myCom = commSetup(CommPortstr);
			if (myCom.hComm == INVALID_HANDLE_VALUE) {
				int check = DisplayNoComportMessageBox();
				if (check == 0) {
					exit(2);
				}
			} else {
				EnableWindow(Window, TRUE);
				DestroyWindow(hWnd);
			}
			break;
		}

		default:
			break;
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

void registerDialogClass(HINSTANCE hInst) {
	WNDCLASS DialogBox = { 0 };
	DialogBox.lpfnWndProc = DialogProcedure;
	DialogBox.hInstance = hInst;
	DialogBox.lpszClassName = L"RRPDialogClass";
	//DialogBox.hCursor = LoadCursor(NULL, IDC_CROSS);

	RegisterClassW(&DialogBox);
}

void DisplayDialog(HWND hWnd) {
	int width = 300, height = 200;
	HWND dialogbox = CreateWindowW(L"RRPDialogClass", " COM PORT",
			WS_VISIBLE | WS_OVERLAPPEDWINDOW | SS_CENTER, 400, 400, width,
			height, hWnd, NULL, NULL, NULL);

	// CREATE TEXT
	createStatic(dialogbox,
			L"           Please insert COM Port Number\n                Where wixel is connected",
			0, 0, width, 50);

	// CREATE INPUT
	int edit_width = 30;
	dialogEdit = CreateWindowW(L"Edit",                	//TYPE
			L"",//Text on button
			WS_VISIBLE | WS_CHILD | WS_BORDER,//What is it
			width/2-(edit_width/2), 50,						// X,Y position
			edit_width, 20,						//Width , Height
			dialogbox,//Parent
			NULL, NULL, NULL);

	// CREATE BUTTONS
	CreateWindowW(L"button",                	//TYPE
			L"ABORT",//Text on button
			WS_VISIBLE | WS_CHILD,//What is it
			0, height - 78,						// X,Y position
			width/2, 40,						//Width , Height
			dialogbox,//Parent
			(HMENU)1,//HNDL
			NULL, NULL);

	CreateWindowW(L"button",                	//TYPE
			L"SET",//Text on button
			WS_VISIBLE | WS_CHILD,//What is it
			width/2, height - 78,							// X,Y position
			width/2, 40,						//Width , Height
			dialogbox,//Parent
			(HMENU)2,//HNDL
			NULL, NULL);
	EnableWindow(hWnd, FALSE);
}

void recieveloop() {
	while (repeat) {
		//printf("I'm inside the thread!");
		myCom.Recieve(&myCom);
		if (myCom.newmsg) {
			myCom.newmsg = FALSE;
			bot.Update(&myCom, &bot);
			updateStatsDisplay();
		}
		Sleep(200L);
	}
	_endthread();
}

void sendLoop() {
	while (TRUE) {
		//printf("I'm inside the thread!");
		if (sending==1){
			myCom.Send(&myCom);
			printf("I'm inside this thread now\n");
			Sleep(800L);
		}
	}
	_endthread();
}

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine,
		int CmdShow) {

	puts("Start of program");
	ROBOT_INIT(&bot);
	myCom = commSetup("\\\\.\\COM999");

	registerDialogClass(Instance);
	_beginthread(sendLoop, 0, NULL);
	_beginthread(recieveloop, 0, NULL);

	Sleep(1000L);
	char test = 10;
	printBits(sizeof(char), &test);
	test <<= 1;
	printBits(sizeof(char), &test);
	WNDCLASS WindowClass = { 0 };
	const char ClassName[] = "robotControlls";
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = ClassName;
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = GetSysColorBrush(1);
	if (!RegisterClass(&WindowClass)) {
		MessageBox(0, "RegisterClass failed", 0, 0);
		return GetLastError();
	}
	LPCTSTR windowName = "3PI Robot controls";

	Window = CreateWindowEx(0, ClassName,						//ClassName
			windowName,								//TitleName
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//Style of window
			CW_USEDEFAULT,								//x Position
			CW_USEDEFAULT,								//y Position
			CW_USEDEFAULT,								//Width
			CW_USEDEFAULT,								//Height
			0,											//Parent
			0,											//Menu
			Instance,							//Instance (default to windows)
			0);										//Paramaters given to window

	if (!Window) {
		MessageBox(0, "CreateWindowEx failed", 0, 0);
		return GetLastError();
	}

	//createButton(Instance, PrevInstance, CmdLine, CmdShow,Window);

	int Running = 1;

	while (Running) {
		MSG Message;
		if (bot.newMsg) {
			bot.newMsg = FALSE;
			//SetWindowText(Static, myCom.Recieved);
		}
		if (myCom.SendSuccesfull) {
			myCom.SendSuccesfull = FALSE;
			SetWindowText(Static, "MSG was send succesfully");
		}

		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				Running = 0;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	myCom.Close(&myCom);
	printf("EXIT_SUCCESFULL\n");
	return 0;
}
