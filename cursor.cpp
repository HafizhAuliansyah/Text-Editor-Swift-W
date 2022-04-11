#include "cursor.h"

CONSOLE_SCREEN_BUFFER_INFO infoScreenBuffer;
cursorHandler cursor;

void moveCursor(int key){
	switch(key){
	}
}
void getCursorPosition(int *row,int *column){
    HANDLE ConsoleOut = getConsoleOut();
    if(!GetConsoleScreenBufferInfo(ConsoleOut, &infoScreenBuffer)){
        MessageBox(NULL, TEXT("Error Pengambilan Info Screen Buffer"), TEXT("ERROR"), MB_OK);
    }
    cursor.x = infoScreenBuffer.dwCursorPosition.X;
    cursor.y = infoScreenBuffer.dwCursorPosition.Y;
    *row = cursor.Y;
    *column = cursor.x;
}


