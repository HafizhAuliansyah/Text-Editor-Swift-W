#ifndef text_op_h
#define text_op_h

#include <stdbool.h>
#include "terminal.h"
#include "input.h"
#include "cursor.h"
#include "buffer.h"
#include "h_queue.h"

struct selectionText{
    int x;
    int y;
    int len;
    bool isOn;
};

/* INIT */
void selectInit();

void addSelectionText(outputBuffer *ob, char *row, int len, selectionText *scanSelected);
void clearSelected();
void copyLocal(erow row[]);
void copyGlobal(teksEditor tEditor);
void pasteLocal();
void pasteGlobal(teksEditor *tEditor);
void cut(teksEditor *tEditor);

void selectShift(selectionText dest);
void selectMoveCursor(int c, teksEditor tEditor);
void findText(teksEditor tEditor,bool replaceAll,char *textPengganti,char *textDicari);
void replace (teksEditor *tEditor,char *pengganti);
void deleteSelect(teksEditor *tEditor);

/* SELECTOR */
selectionText getSelection();

/* MUTATOR */
void setSelection(selectionText new_selection);

int abs(int x);

/* Undo dan Redo*/
teksEditor getUndo(teksEditor tEditor);
teksEditor getRedo(teksEditor tEditor);
void pushToUndo(teksEditor tEditor, bool state);
void pushToRedo(teksEditor tEditor);

#endif
