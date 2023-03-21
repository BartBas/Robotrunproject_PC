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

Communications myCom;

HWND Static;

BOOL repeat = 1;
//unsigned thread1;

void (*buttons[17])(HWND,WPARAM);

void defaultbutton(HWND Parent,WPARAM WParam){
	printf("hey\n");
	SetWindowText(Static,"NEW TEXT HAS ARIVED!");
	fflush(stdout);
}

void defaultcheckBT(HWND Parent,WPARAM WParam){
	SetWindowText(Static,"button was checked");
	char temp[10];
	char MSG[50] = "Place ";
	sprintf(temp,"%d",WParam);
	strcat(MSG,temp);
	BOOL checked = IsDlgButtonChecked(Parent, WParam);
		    if (checked) {
		        CheckDlgButton(Parent, WParam, BST_UNCHECKED);
		        strcpy(temp," was unchecked");
		    	strcat(MSG,temp);
		        SetWindowText(Static,MSG);
		    } else {
			 CheckDlgButton(Parent, WParam, BST_CHECKED);
			  	strcpy(temp," was checked");
		    	strcat(MSG,temp);
		        SetWindowText(Static,MSG);
		    }
	fflush(stdout);
}

void UIsend(){
	char temp[26]="abcdefghijklmnopqrstuvwxyf";
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = temp[i];
	}
	myCom.Send(&myCom);
}

void UIQuit(){
	char temp[26]="cccccccccccccccccccccccccc";
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = temp[i];
	}
	myCom.Send(&myCom);
}

void EMERGACYSTOP(){
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = 255;
	}
	myCom.Send(&myCom);
	myCom.Send(&myCom);
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

unsigned char reverseBits(unsigned char num)
{
    unsigned char NO_OF_BITS = sizeof(num) * 8;
    unsigned char reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}

void createOrder(HWND Parent,WPARAM _){
	unsigned int send=0;
	printBits(sizeof(int),&send);
	int temp[16];
	int counter=0;
	for (int i=0;i<16;i++){
		if (IsDlgButtonChecked(Parent,i)==BST_CHECKED){
			temp[counter] = i;
			counter++;
			send++;
			send<<=1;
		}else{
			send<<=1;
			}

	}
	send>>=1;
	printBits(sizeof(int),&send);
	char totalchar[50] = "Send to places: ";
	char tempstr [10];
	for(int i=0;i<counter;i++){
		printf(" place: %d\n",temp[i]);
		sprintf(tempstr,"%d",temp[i]);
		strcat(totalchar,tempstr);
		strcat(totalchar,"  ");
	}
	unsigned char sendbyte1 = 0;
	unsigned char sendbyte2 = 0;
	sendbyte2= reverseBits(sendbyte2 | send);
	send>>=8;
	sendbyte1= reverseBits(sendbyte1 | send);
	printf("send byte1: ");
	printBits(sizeof(char),&sendbyte1);
	printf("send byte2: ");
	printBits(sizeof(char),&sendbyte2);
	SetWindowText(Static,totalchar);
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = 0;
	}
	myCom.msgBuffer[1]=sendbyte1;
	myCom.msgBuffer[2]=sendbyte2;
	myCom.Send(&myCom);
}

void createButton(HWND Parent, wchar_t Name[],int x,int y,int w,int h,int number){
	CreateWindowW(
				L"button",                	//TYPE
				Name,				//Text on button
				WS_VISIBLE | WS_CHILD,		//What is it
				x,y,						// X,Y position
				w,h,						//Width , Height
				Parent,						//Parent
				(HMENU)number,						//HNDL
				NULL,NULL);
	buttons[number]=defaultbutton;
}

void createCheckBX(HWND Parent, wchar_t Name[],int x,int y,int w,int h,int number){
	CreateWindowW(
				L"button",                	//TYPE
				Name,						//Text on button
				WS_VISIBLE | WS_CHILD | BS_CHECKBOX,		//What is it
				x,y,						// X,Y position
				w,h,						//Width , Height
				Parent,						//Parent
				(HMENU)number,						//HNDL
				NULL,NULL);
	CheckDlgButton(Parent, number, BST_UNCHECKED);
	buttons[number]=defaultcheckBT;
}

HWND createStatic(HWND Parent, wchar_t Name[],int x,int y,int w,int h){
	return CreateWindowW(
				L"static",                	//TYPE
				Name,				//Text on button
				WS_VISIBLE | WS_CHILD,		//What is it
				x,y,						// X,Y position
				w,h,						//Width , Height
				Parent,						//Parent
				NULL, NULL,NULL);

}

void AddControls(HWND Parent){
	// GENERATE CHECK BOXES
	int counter =0;
	for (int x=4;x>0;x--){ for (int y=4;y>0;y--){
	wchar_t vOut [12];
	_itow_s(counter,vOut,sizeof(vOut)/2,10);
	createCheckBX(Parent,vOut,((50*y)-40),((50*x)-40),35,13,counter);
	counter++;
	}}
	createButton(Parent,L"Send Order",250,100,100,20,counter);
	buttons[counter]=createOrder;
	counter++;

	createButton(Parent,L"EMERGANCY STOP",350,50,150,30,counter);
	buttons[counter]=EMERGACYSTOP;
	counter++;

	Static = createStatic(Parent,L"3PI ROBOT CONTROLLER THROUGH THE POWER OF THE WIXEL!",10,200,1000,50);
}



int counter=0;
LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch(Message) {
        case WM_KEYDOWN: {
            switch(WParam) {
                case 'O': {
                    DestroyWindow(Window);
                } break;
                case 'S': {
                    UIsend();
                } break;
                case 'Q': {
                    UIQuit();
                } break;
            }
        } break;
        case WM_COMMAND:{
      	buttons[WParam](Window,WParam);
      	break;
        }
        case WM_CREATE:{
        	AddControls(Window);
        	break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        default: {
            return DefWindowProc(Window, Message, WParam,  LParam);
        }
    }
    return 0;
}


void recieveloop(){
	while (repeat){
		printf("I'm inside the thread!");
	 myCom.Recieve(&myCom);
	 Sleep(1000L);
	}
	_endthread();
}

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {

	puts("Start of program");

	myCom = commSetup();
	_beginthread(recieveloop,0,NULL);
	Sleep(1000L);
	char test = 10;
	printBits(sizeof(char),&test);
	test <<=1;
	printBits(sizeof(char),&test);
	WNDCLASS WindowClass = {0};
	    const char ClassName[] = "robotControlls";
	    WindowClass.lpfnWndProc = WindowProc;
	    WindowClass.hInstance = Instance;
	    WindowClass.lpszClassName = ClassName;
	    WindowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	    WindowClass.hbrBackground = GetSysColorBrush(1);
	    if(!RegisterClass(&WindowClass)) {
	        MessageBox(0, "RegisterClass failed", 0, 0);
	        return GetLastError();
	    }
	    LPCTSTR windowName = "3PI Robot controls";

	    HWND Window = CreateWindowEx(0,
	    							 ClassName,									//ClassName
									 windowName,								//TitleName
	                                 WS_OVERLAPPEDWINDOW|WS_VISIBLE,			//Style of window
	                                 CW_USEDEFAULT,								//x Position
									 CW_USEDEFAULT,								//y Position
	                                 CW_USEDEFAULT,								//Width
									 CW_USEDEFAULT,								//Height
	                                 0,											//Parent
									 0,											//Menu
									 Instance,									//Instance (default to windows)
									 0);										//Paramaters given to window



	    if(!Window) {
	        MessageBox(0, "CreateWindowEx failed", 0, 0);
	        return GetLastError();
	    }

	    //createButton(Instance, PrevInstance, CmdLine, CmdShow,Window);

	    int Running = 1;

	    while(Running) {
	        MSG Message;

        	if (myCom.newmsg){
        		myCom.newmsg=FALSE;
        	SetWindowText(Static,myCom.Recieved);}
        	if (myCom.SendSuccesfull){
        		myCom.SendSuccesfull=FALSE;
        	SetWindowText(Static,"MSG SUCCESFULLY RECIEVED ON OTHER WIXEL!");}

	        while(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
	            if(Message.message == WM_QUIT) Running = 0;
	            TranslateMessage(&Message);
	            DispatchMessage(&Message);

	        	}
	    }
		myCom.Close(&myCom);
		printf("EXIT_SUCCESFULL\n");
	    return 0;
}
