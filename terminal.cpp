#include <stdio.h>
#include <windows.h>
#include "terminal.h"

HANDLE ConsoleIn,ConsoleOut,ConsoleInOld,ConsoleOutOld;
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
terminal terminalConfig;
char c;
void rawModeOn(){
	DWORD ModeIn,ModeOut;
	COMMTIMEOUTS timeouts;
	atexit(rawModeOff);
	ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(ConsoleOut, &bufferInfo);
	GetConsoleMode(ConsoleOut,&ModeOut);
	GetConsoleMode(ConsoleIn,&ModeIn);
	GetCommTimeouts(ConsoleIn,&timeouts);
	terminalConfig.defaultModeIn = ModeIn;
	terminalConfig.defaultModeOut = ModeOut;
	
	ModeIn &= ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
	ModeIn |= (ENABLE_VIRTUAL_TERMINAL_INPUT);
	ModeOut &= ~(ENABLE_WRAP_AT_EOL_OUTPUT);
	ModeOut |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING|DISABLE_NEWLINE_AUTO_RETURN);
	SetConsoleMode(ConsoleIn,ModeIn);
	SetConsoleMode(ConsoleOut,ModeOut);
	SetCommTimeouts(ConsoleIn, &timeouts);
}

void rawModeOff(){
	SetConsoleMode(ConsoleIn,terminalConfig.defaultModeIn);
	SetConsoleMode(ConsoleOut,terminalConfig.defaultModeOut);
}
void die(const char *s)
{
	WriteFile(ConsoleOut,"\x1b[2J", 4,NULL,NULL);
	WriteFile(ConsoleOut,"\x1b[H", 3,NULL,NULL);
    perror(s);
    exit(1);
}
void getWindowSize(int *rows, int *cols){
	*rows = bufferInfo.dwMaximumWindowSize.Y;
	*cols = bufferInfo.dwMaximumWindowSize.X; 
}
HANDLE getConsoleOut(){
    return ConsoleOut;
}
HANDLE getConsoleIn(){
    return ConsoleIn;
}
void terminalInit(){
	getWindowSize(&terminalConfig.screenrows, &terminalConfig.screencols);
}
int getScrennRows(){
	return terminalConfig.screenrows;
}
int getScrenCols(){
	return terminalConfig.screencols;
}
