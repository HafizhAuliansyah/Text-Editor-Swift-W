#include "text_operation.h"

selectionText selection;

char *hasil_c;
void addSelectionText(outputBuffer *ob, char *row, int len, selectionText *scanSelected)
{
    // var at, sebagai penampung koordinat kolom
    int at = 0;
    int lenAvailable = len;
    int lenSelect;
    // Memasukkan kolom sebelum kata terselect ke ab
    bufferAppend(ob, &row[at], scanSelected->x);
    lenAvailable -= scanSelected->x;
    // Select Text sesuai kolom selection.x, sejumlah selection.len ke kanan
    bufferAppend(ob, "\x1b[7m", 4);
    at = scanSelected->x;
    
    if(lenAvailable < scanSelected->len){
        lenSelect = lenAvailable;
        scanSelected->len -= lenAvailable;
        scanSelected->y++;
        scanSelected->x = 0;
    }else{
        lenSelect = scanSelected->len;
    }
    bufferAppend(ob, &row[at], lenSelect);
    bufferAppend(ob, "\x1b[m", 3);

    lenAvailable -= lenSelect;
    if(lenAvailable > 0){
        // Memasukkan kolom setelah kata terselect ke ab
        at = scanSelected->x + scanSelected->len;
        bufferAppend(ob, &row[at], len - at);
    }

}
void selectMoveCursor(int key, teksEditor tEditor)
{
    // editorSetStatusMessage("%d cx, %d size", C.x,E.row[C.y].size);
    selectionText dest;
    dest.x = getCursor().x;
    dest.y = getCursor().y;
    dest.len = selection.len;
    switch (key)
    {
    case SHIFT_ARROW_LEFT:
        if (getCursor().x == 0){
            if(getCursor().y == 0)
                return;
            
            if(selection.x == getCursor().x){
                moveCursor(ARROW_LEFT, tEditor);
                dest.x = getCursor().x;
                dest.y = getCursor().y;
            }else{
                moveCursor(ARROW_LEFT, tEditor);
                dest.x = selection.x;
                dest.y = selection.y;
            }
            selectShift(dest);
            return;
        }
        
        if((selection.x != getCursor().x || selection.y != getCursor().y) && selection.isOn){
            moveCursor(ARROW_LEFT, tEditor);
            dest.x = selection.x;
            dest.y = selection.y;
            dest.len -= 1;
        }else{
            moveCursor(ARROW_LEFT, tEditor);
            dest.x = getCursor().x;
            dest.len += 1;
        }
        break;
    case SHIFT_ARROW_RIGHT:
    {
        if (getCursor().x >= tEditor.row[getCursor().y].size){
            if(getCursor().y == tEditor.numrows - 1)
                return;
            if(selection.x == getCursor().x){
                moveCursor(ARROW_RIGHT, tEditor);
                dest.x = getCursor().x;
                dest.y = getCursor().y;
            }else{
                moveCursor(ARROW_RIGHT, tEditor);
                dest.x = selection.x;
                dest.y = selection.y;
            }
            selectShift(dest);
            return;
        }
        
        if(selection.x == getCursor().x && selection.y == getCursor().y){
            moveCursor(ARROW_RIGHT, tEditor);
            dest.x = getCursor().x;
            dest.len -= 1;
        }else{
            if(selection.isOn){
                dest.x = selection.x;
                dest.y = selection.y;
            }
            moveCursor(ARROW_RIGHT, tEditor);
            dest.len += 1;
        }
    }
        break;
    case SHIFT_ARROW_UP:
    {
        if(getCursor().y == 0)
            return;
        int sizeFirst, sizeLast;
        
        if(selection.y != getCursor().y && selection.isOn){
            moveCursor(ARROW_UP, tEditor);
            dest.x = selection.x;
            dest.y = selection.y;
            sizeFirst = tEditor.row[getCursor().y].size - getCursor().x;
            sizeLast = getCursor().x;
            dest.len -= (sizeFirst + sizeLast);
            if(dest.len < 0){
                dest.len = abs(dest.len);
                dest.x = getCursor().x;
            }
        }else{
            moveCursor(ARROW_UP, tEditor);
            dest.y = getCursor().y;
            sizeFirst = tEditor.row[dest.y].size - dest.x;
            sizeLast = dest.x;
            dest.len += (sizeFirst + sizeLast);
        }
    }
        break;
    case SHIFT_ARROW_DOWN:
    {
        if(getCursor().y == tEditor.numrows - 1)
            return;
        
        int sizeFirst, sizeLast;
        if(selection.y == getCursor().y && selection.x == getCursor().x && selection.isOn){
            sizeFirst = tEditor.row[getCursor().y].size - getCursor().x;
            sizeLast = getCursor().x;
            moveCursor(ARROW_DOWN, tEditor);
            dest.x = selection.x;
            dest.y = getCursor().y;
            dest.len -= (sizeFirst + sizeLast);
            if(dest.len < 0){
                dest.len = abs(dest.len);
                dest.x = selection.x - dest.len;
            }
        }else{
            if(selection.isOn){
                dest.x = selection.x;
                dest.y = selection.y;
            }
            sizeFirst = tEditor.row[getCursor().y].size - getCursor().x;
            sizeLast = getCursor().x;
            moveCursor(ARROW_DOWN, tEditor);
            dest.len += (sizeFirst + sizeLast);
        }
    }
        break;
    default:
        setMessage("Other");
        break;
    }
    
    if(dest.len == 0){
        clearSelected();
    }else{
        selectShift(dest);
    }
    
}
void selectShift(selectionText dest)
{
    selection.x = dest.x;
    selection.y = dest.y;
    selection.len = dest.len;
    selection.isOn = true;
}
void clearSelected()
{
    selection.x = -1;
    selection.y = -1;
    selection.len = 0;
    selection.isOn = false;
}

void copyLocal(erow row[])
{
    hasil_c = (char*) realloc(hasil_c, selection.len + 1);
    memmove(hasil_c, &row[selection.y].chars[selection.x], selection.len);
    hasil_c[selection.len] = '\0';
}
void copyGlobal(erow row[]){
    OpenClipboard(0);
    EmptyClipboard();
    hasil_c = (char*) realloc(hasil_c, selection.len + 1);
    memcpy(hasil_c, &row[selection.y].chars[selection.x], selection.len);
    hasil_c[selection.len] = '\0';
    HGLOBAL clipboardText = GlobalAlloc(GMEM_MOVEABLE, selection.len + 1);
    memcpy(GlobalLock(clipboardText), hasil_c, selection.len + 1);
    GlobalUnlock(clipboardText);
    SetClipboardData(CF_TEXT, clipboardText);
    CloseClipboard();
}
void pasteLocal()
{
    int column_len = MAX_COLUMN - getCursor().x;
    
    for (int x = 0; x < strlen(hasil_c); x++)
        insertChar(hasil_c[x]);teksEditor *tEditor;
}
void pasteGlobal(teksEditor *tEditor){
    if(selection.isOn){
        int currentX = selection.x + selection.len;
        while(currentX != selection.x){
            rowDelChar(&tEditor->row[selection.y], currentX);
            currentX--;
        }
    }
    OpenClipboard(0);
    HANDLE clipboardText = GetClipboardData(CF_TEXT);
    int column_len = MAX_COLUMN - getCursor().x;
    for (int x = 0; x < strlen((char*) clipboardText); x++)
        insertChar( ((char*) clipboardText)[x] );
    CloseClipboard();
}
void cut(teksEditor *tEditor){
    copyGlobal(tEditor->row);
    int currentX = selection.x + selection.len;
    while(currentX != selection.x){
        rowDelChar(&tEditor->row[selection.y], currentX);
        currentX--;
    }
}
/** Find **/
void findText(teksEditor tEditor)
{
    char *query = setInputMassage("Cari : %s (Tekan ESC Untuk Batalkan)", 7);
    if (query == NULL)
        return;
    int ketemu = 1;
    int i;
    for (i = 0; i < tEditor.numrows; i++)
    {
        erow *row = &tEditor.row[i];
        char *match = strstr(row->render, query);
        if (match)
        {
            setCursorY(i);
            setCursorX(renderXToCursorX(row, match - row->render));

            // Untuk select text
            selection.y = getCursor().y;
            selection.x = getCursor().x;
            selection.len = strlen(query);
            selection.isOn = true;
            int screenrows = getScreenRows(); // TODO get screenrows
            if (i >= screenrows)
            {
                setStartRow(getCursor().y);
            }

            ketemu = 0;
            break;
        }
    }
    if (ketemu)
    {
        // COMMENTED editorSetStatusMessage("Teks Tidak Ada!");
    }

    free(query);
}
selectionText getSelection(){
    return selection;
}
void setSelection(selectionText new_selection){
    selection = new_selection;
}
void selectInit(){
    clearSelected();
    free(hasil_c);
    
}
int abs(int x){
    if(x < 0){
        return x * -1;
    }else{
        return x;
    }
}