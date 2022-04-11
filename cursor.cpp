#include "cursor.h"

CONSOLE_SCREEN_BUFFER_INFO infoScreenBuffer;
cursorHandler cursor;

void moveCursor(int key){
	switch(key){
	}
}
void getCursorPosition(int *row,int *column){
    // Mengambil konfigurasi console out
    HANDLE ConsoleOut = getConsoleOut();
    // Mengambil konfigurasi screen buffer info
    if(!GetConsoleScreenBufferInfo(ConsoleOut, &infoScreenBuffer)){
        MessageBox(NULL, TEXT("Error Pengambilan Info Screen Buffer"), TEXT("ERROR"), MB_OK);
    }
    // Memasukkan koordinat cursor ke struct cursor handler
    cursor.x = infoScreenBuffer.dwCursorPosition.X;
    cursor.y = infoScreenBuffer.dwCursorPosition.Y;
    // Mengambalikan nilai
    *row = cursor.y;
    *column = cursor.x;
}


