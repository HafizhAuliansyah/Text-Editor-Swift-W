
#include "terminal.h"

HANDLE ConsoleIn,ConsoleOut;
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
char *huruf = (char*)malloc(13);
 
void rawModeOn(){
	DWORD ModeIn,ModeOut;
	COMMTIMEOUTS timeouts;
	ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(ConsoleOut, &bufferInfo);
	GetConsoleMode(ConsoleOut,&ModeOut);
	GetConsoleMode(ConsoleIn,&ModeIn);
	GetCommTimeouts(ConsoleIn,&timeouts);
//	timeouts.ReadIntervalTimeout = 1;
//	timeouts.ReadTotalTimeoutConstant = 1;
//	timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
//	timeouts.WriteTotalTimeoutConstant = 0;
//	timeouts.WriteTotalTimeoutMultiplier = 0;

	ModeIn &= ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
	ModeIn |= (ENABLE_VIRTUAL_TERMINAL_INPUT);
	ModeOut |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING|DISABLE_NEWLINE_AUTO_RETURN);
	SetConsoleMode(ConsoleIn,ModeIn);
	SetConsoleMode(ConsoleOut,ModeOut);
	SetCommTimeouts(ConsoleIn, &timeouts);
}

DWORD readKey(){
	DWORD read,written;
	
	ReadFile(ConsoleIn,huruf,1,&read,NULL);

	return read;
}

void keyProcess(){
	DWORD written;
	DWORD write = readKey();

	WriteFile(ConsoleOut,huruf,write,&written,NULL);
}

