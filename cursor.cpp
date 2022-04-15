#include "cursor.h"

CONSOLE_SCREEN_BUFFER_INFO infoScreenBuffer;
cursorHandler cursor,ms_cursor;

void moveCursor(int button){
	// Check jika baris dimana cursor berada tidak kosong
	// TODO.....
	
	// Arrow Handler
	switch(button){
		case "ARROW_LEFT":
			if(cursor.x != 0){
				// Memindahkan Cursor ke kiri 1
				cursor.x--;
			}else if(cursor.x > 0 && cursor.x == 0){
				// Cursor ke baris atas
				cursor.y--;
				int size; // TODO max size dari row sebelumnya
				cursor.x = size;
			}
			break;
		case "ARROW_RIGHT":
			if( /* baris ada && cursor.x < max size baris */){
				cursor.x++;
			}else if(/* baris ada && cursor.x == max size baris && cursor.y != jumlah baris dokumen - 1*/ ){
				// Cursor ke baris bawah
				cursor.y++;
				cursor.x = 0;
				
			}
			break;
		case "ARROW_UP":
			if(cursor.y != 0 ){
				// Cursor pindah ke baris atas
				cursor.y--;
			}
			break;
		case "ARROW_DOWN":
			if(/* cursor.y < jumlah baris - 1*/){
				// Cursor pindah ke baris bawah
				cursor.y++;
			}
			break;
	}
	
	// Error Handling, cursor melebihi jumlah kolom
	// TODO get data row
	int len // TODO get row size
	if(cursor.x > len){
		cursor.x = len;
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
    cursor.x = infoScreenBuffer.dwCursorPosition.X - 1;
    cursor.y = infoScreenBuffer.dwCursorPosition.Y - 1;
    // Mengambalikan nilai
    *row = cursor.y;
    *column = cursor.x;
}
void cursorInit(){
	cursor.x = 0;
	cursor.y = 0;
	cursor.rx = 0;
}


