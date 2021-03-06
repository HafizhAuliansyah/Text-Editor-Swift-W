#ifndef cursor_h
#define cursor_h
#include "terminal.h"
#include "input.h"

struct cursorHandler
{
	int x;
	int y;
	int rx;
	int start_row;
	int start_col;
};

/* Mengambil posisi cursor */
void getCursorPosition(int *row, int *column);

/* Initialize nilai cursor awal */
void cursorInit();

/* Perpindahan cursor dengan arrow */
void moveCursor(int button, teksEditor tEditor);

/* Converter cursor X to RX */
int cursorXToRenderX(erow *row, int x);

/* Converter cursor RX to X */
int renderXToCursorX(erow *row, int rx);

/* Modifier nilai start_row dan start_row untuk scrolling */
void cursorScroll(teksEditor tEditor);

/* SELECTOR */
cursorHandler getCursor();
int getStartRow();
int getStartCol();
cursorHandler getMessageCursor();

/* MUTATOR */
void setCursor(cursorHandler c);
void setCursorX(int x);
void setCursorY(int y);
void addCursorX();
void addCursorY();
void setStartRow(int sr);
void setStartCol(int sc);
void addStartRow();
void addStartCol();

void setMessageCursor(cursorHandler new_ms);

#endif
