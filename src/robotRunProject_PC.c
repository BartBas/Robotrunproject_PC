#include <stdio.h>
#include <stdlib.h>

//#include "interface.h"
#include "Comunication.h"
#include "DEBUG_Prints.h"
#include "windows.h"
#include <process.h>
#include <Winbase.h>

#define BUTTON

#define WIN32_LEAN_AND_MEAN

Communications myCom;

HWND Static;

BOOL repeat = 1;
unsigned thread1;

void (*buttons[9])();

void defaultbutton(){
	printf("hey");
	SetWindowText(Static,"NEW TEXT HAS ARIVED!");
	fflush(stdout);
}

void UIsend(){
	char temp[26]="abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = temp[i];
	}
	CancelIo(myCom.hComm);
	myCom.Send(&myCom);
}

void UIQuit(){
	repeat = 0;
	char temp[26]="cccccccccccccccccccccccccc";
	for (int i = 0; i < myCom.val; i++) {
		myCom.msgBuffer[i] = temp[i];
	}
	CancelIo(myCom.hComm);
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
	createButton(Parent,L"Send msg",10,10,100,20,1);
	Static = createStatic(Parent,L"Text",10,90,1000,50);
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
        	buttons[WParam]();
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

	WNDCLASS WindowClass = {0};
	    const char ClassName[] = "robotControlls";
	    WindowClass.lpfnWndProc = WindowProc;
	    WindowClass.hInstance = Instance;
	    WindowClass.lpszClassName = ClassName;
	    WindowClass.hCursor = LoadCursor(NULL,IDC_ARROW);

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




/*
int main(void) {




	BOOLEAN status = myCom.Send(&myCom);

	DBprintf("hey there I'm here\n");


	myCom.Recieve(&myCom);
	printf("Recieved: ");
	int size = sizeof(myCom.Recieved)/sizeof(char);
	for (int i = 0; i < size ; i++) {
		printf("%d:%c ", i, myCom.Recieved[i]);
	}
	printf("\n\n");



	return EXIT_SUCCESS;
}
*/
