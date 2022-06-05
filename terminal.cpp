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
	HWND windowHandle;

	// system("mode 650");
	windowHandle = GetConsoleWindow();
	ShowWindow( windowHandle , SW_MAXIMIZE);
	atexit(rawModeOff);
	ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(ConsoleOut, &bufferInfo);
	GetConsoleTitle(terminalConfig.oldTitle,MAX_PATH);
	GetConsoleMode(ConsoleOut,&ModeOut);
	GetConsoleMode(ConsoleIn,&ModeIn);
	GetCommTimeouts(ConsoleIn,&timeouts);
	terminalConfig.defaultModeIn = ModeIn;
	terminalConfig.defaultModeOut = ModeOut;

	ModeIn &= ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
	ModeIn |= (ENABLE_VIRTUAL_TERMINAL_INPUT);
	ModeOut &= ~(ENABLE_WRAP_AT_EOL_OUTPUT);
	ModeOut |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING|DISABLE_NEWLINE_AUTO_RETURN);

	// SetConsoleDisplayMode(ConsoleOut,CONSOLE_FULLSCREEN_MODE,0);
	// SetWindowPos( windowHandle, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	SetConsoleMode(ConsoleIn,ModeIn);
	SetConsoleMode(ConsoleOut,ModeOut);
	SetCommTimeouts(ConsoleIn, &timeouts);
}

void changeWindowSize(COORD nSize){


}

void rawModeOff(){
	SetConsoleMode(ConsoleIn,terminalConfig.defaultModeIn);
	SetConsoleMode(ConsoleOut,terminalConfig.defaultModeOut);
	SetConsoleTitle(terminalConfig.oldTitle);
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
	COORD newScreenBuffer;
	newScreenBuffer.X = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
	newScreenBuffer.Y = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
	SetConsoleScreenBufferSize(ConsoleOut,newScreenBuffer);
	SetConsoleTitle("Text Editor Swift");
	terminalConfig.screenrows -= 5;

}
int getScreenRows(){
	return terminalConfig.screenrows;
}
int getScrenCols(){
	return terminalConfig.screencols;
}
