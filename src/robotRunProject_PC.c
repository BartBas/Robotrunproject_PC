#include <stdio.h>
#include <stdlib.h>

//#include "interface.h"
#include "Comunication.h"
#include "DEBUG_Prints.h"
#include "windows.h"

#define WIN32_LEAN_AND_MEAN

Communications myCom;

void UIsend(){
	char temp[26]="abcdefghijklmnopqrstuvwxyz";
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

void createButton(HWND Parent, wchar_t Name[],int x,int y,int w,int h){
	CreateWindowW(
				L"button",                	//TYPE
				Name,				//Text on button
				WS_VISIBLE | WS_CHILD,		//What is it
				x,y,						// X,Y position
				w,h,						//Width , Height
				Parent,						//Parent
				NULL, NULL,NULL);
}

void AddControls(HWND Parent){
	createButton(Parent,L"Send msg",10,10,100,20);
}



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

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {

	puts("Start of program");

	myCom = commSetup();



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
