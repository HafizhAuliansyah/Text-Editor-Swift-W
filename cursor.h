#ifndef cursor_h
#define cursor_h
#include "terminal.h"

struct cursorHandler{
	int x;
	int y;
	int rx;
};

void moveCursor(int key);
void getCursorPosition(int *row,int *column);

#endif
