#ifndef text_op_h
#define text_op_h

#include <stdbool.h>
#include "terminal.h"
#include "input.h"
#include "cursor.h"

struct selectionText{
    int x;
    int y;
    int len;
    bool isOn;
};


void addSelectionText(struct abuf *ab, char *row, int len);
void clearSelected();
void copy(erow row[]);
void paste();
void selectShift(selectionText dest);
void selectMoveCursor(int c, teksEditor tEditor);
void findText(teksEditor tEditor);

#endif