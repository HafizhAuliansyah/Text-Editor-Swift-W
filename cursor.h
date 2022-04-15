#ifndef cursor_h
#define cursor_h
#include "terminal.h"

struct cursorHandler{
	int x;
	int y;
	int rx;
};

void moveCursor(int button);
void getCursorPosition(int *row,int *column);
void cursorInit();

#endif
