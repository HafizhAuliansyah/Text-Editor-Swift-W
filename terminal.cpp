#include <stdio.h>
#include <windows.h>
#include "prototype.h"

HANDLE ConsoleIn,ConsoleOut;
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
char *huruf = (char*)malloc(sizeof(char));
 
void rawModeOn(){
	DWORD Mode;
	COMMTIMEOUTS timeouts;
	ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(ConsoleOut, &bufferInfo);
	GetConsoleMode(ConsoleIn,&Mode);
	GetCommTimeouts(ConsoleIn,&timeouts);
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;

	Mode &= ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
	SetConsoleMode(ConsoleIn,Mode);
	SetCommTimeouts(ConsoleIn, &timeouts);
}

DWORD readKey(){
	DWORD read,written;
	
	while(ReadFile(ConsoleIn,huruf,1,&read,NULL) != 1){
		//TODO
	}
	
	return read;
}

void keyProcess(){
	DWORD written;
	DWORD write = readKey();
	WriteFile(ConsoleOut,huruf,write,&written,NULL);
	
}

