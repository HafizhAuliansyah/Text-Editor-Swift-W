#include <stdio.h>
#include <windows.h>
#include "terminal.h"

HANDLE ConsoleIn,ConsoleOut;
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
char c;
void rawModeOn(){
	DWORD ModeIn,ModeOut;
	COMMTIMEOUTS timeouts;
	ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(ConsoleOut, &bufferInfo);
	GetConsoleMode(ConsoleOut,&ModeOut);
	GetConsoleMode(ConsoleIn,&ModeIn);
	GetCommTimeouts(ConsoleIn,&timeouts);

	ModeIn &= ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
	ModeIn |= (ENABLE_VIRTUAL_TERMINAL_INPUT);
	ModeOut |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING|DISABLE_NEWLINE_AUTO_RETURN);
	SetConsoleMode(ConsoleIn,ModeIn);
	SetConsoleMode(ConsoleOut,ModeOut);
	SetCommTimeouts(ConsoleIn, &timeouts);
}
void die(const char *s)
{
	WriteFile(ConsoleOut,"\x1b[2J", 4,NULL,NULL);
	WriteFile(ConsoleOut,"\x1b[H", 3,NULL,NULL);
    perror(s);
    exit(1);
}

int readKey(){
	DWORD read,written;
	char c;
	ReadFile(ConsoleIn,&c,1,&read,NULL);
	if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
	}

	return c;
}

void keyProcess(){
	int key = readKey();
	DWORD written;
	DWORD write;
	if(key == CTRL_KEY('q'))
		die("EXIT");
	WriteFile(ConsoleOut,&key, 1,&written,NULL);

}

HANDLE getConsoleOut(){
    return ConsoleOut;
}
HANDLE getConsoleIn(){
    return ConsoleIn;
}

