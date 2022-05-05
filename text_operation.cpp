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
    selectionText dest;
    dest.x = getCursor().x;
    dest.y = getCursor().y;
    dest.len = selection.len;
    // Kondisional Shift Select
    
    switch (key)
    {
    case SHIFT_ARROW_LEFT:
        // HAFIZH : IF cursor berada di awal kolom
        if (getCursor().x == 0){
            // HAFIZH : IF cursor berada di baris pertama
            if(getCursor().y == 0)
                return;
            // HAFIZH : IF selection.x sama dengan posisi cursor
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
        }else{
            // HAFIZH : IF selection x dan y != posisi cursor
            if((selection.x != getCursor().x || selection.y != getCursor().y) && selection.isOn){
                moveCursor(ARROW_LEFT, tEditor);
                dest.x = selection.x;
                dest.y = selection.y;
                dest.len -= 1;
            }else{
                // HAFIZH : ELSE proses selection left normal
                moveCursor(ARROW_LEFT, tEditor);
                dest.x = getCursor().x;
                dest.y = getCursor().y;
                dest.len += 1;
            }
        }
        break;
    case SHIFT_ARROW_RIGHT:
    {
        // HAFIZH : IF cursor berada di akhir kolom
        if (getCursor().x >= tEditor.row[getCursor().y].size){
            // HAFIZH : IF cursor berada di baris paling terakhir
            if(getCursor().y == tEditor.numrows - 1)
                return;
            // HAFIZH : IF select x sama dengan cursor x
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
        }else{
            // HAFIZH : IF selection x dan y sama dengan posisi cursor -> length dikurangi. ELSE ditambah
            if(selection.x == getCursor().x && selection.y == getCursor().y){
                moveCursor(ARROW_RIGHT, tEditor);
                dest.x = getCursor().x;
                dest.len -= 1;
            }else{
                // HAFIZH : ELSE proses selection right normal
                // HAFIZH : IF sedang select -> selection x & y tetap. ELSE selection x & y diganti dengan posisi cursor
                if(selection.isOn){
                    dest.x = selection.x;
                    dest.y = selection.y;
                }
                moveCursor(ARROW_RIGHT, tEditor);
                dest.len += 1;
            }
        }
        
    }
        break;
    case SHIFT_ARROW_UP:
    {
        // HAFIZH : IF cursor berada di baris pertama -> return
        if(getCursor().y == 0)
            return;

        int sizeFirst, sizeLast;
        // HAFIZH : IF selection y != cursor y dan select nyala -> select dikurangi, selection x & y tetap
        if(selection.y != getCursor().y && selection.isOn){
            moveCursor(ARROW_UP, tEditor);
            dest.x = selection.x;
            dest.y = selection.y;
            sizeFirst = tEditor.row[getCursor().y].size - getCursor().x;
            sizeLast = getCursor().x;
            dest.len -= (sizeFirst + sizeLast);
            // HAFIZH : IF pengurangan negatif
            if(dest.len < 0){
                dest.len = abs(dest.len);
                dest.x = getCursor().x;
            }
        }else{
            // HAFIZH : ELSE Proses selection up normal
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
        // HAFIZH : IF cursor berada di baris paling akhir -> return
        if(getCursor().y == tEditor.numrows - 1)
            return;
        
        int sizeFirst, sizeLast;
        // HAFIZH : IF posisi x & y selection dengan cursor sama -> select dikurangi, selection x & y mengikuti cursor
        if(selection.y == getCursor().y && selection.x == getCursor().x && selection.isOn){
            sizeFirst = tEditor.row[getCursor().y].size - getCursor().x;
            sizeLast = getCursor().x;
            moveCursor(ARROW_DOWN, tEditor);
            dest.x = getCursor().x;
            dest.y = getCursor().y;
            dest.len -= (sizeFirst + sizeLast);
            // HAFIZH : IF hasil pengurangan negatif
            if(dest.len < 0){
                dest.len = abs(dest.len);
                dest.x = selection.x - dest.len;
            }
        }else{
            // HAFIZH : ELSE proses selection down normal
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
        setMessage("\x1b[31m ERROR SELECTION PROSES !! \x1b[m");
        break;
    }

    // HAFIZH : IF panjang selection 0 -> clear selection
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
    // HAFIZH : Deklarasi tracker variabel
    int currentY = selection.y;
    int currentX = selection.x;
    int EOL = 0;
    // HAFIZH : Looping mencari jumlah End Of line 
    for(int s = 0; s < selection.len; s++){
        if(currentX < row[currentY].size){
            currentX++;
        }else{
            EOL++;
            currentX = 0;
            currentY++;
        }
    }
    // HAFIZH : Refresh nilai tracker
    currentX = selection.x;
    currentY = selection.y;
    // HAFIZH : Jumlah end of line dikali 2 karena end of line windows \r\n
    EOL *= 2;
    // HAFIZH : panjang karakter yang akan di copy
    int len_cpy = selection.len + EOL + 1;
    // HAFIZH : realokasi panjang hasil_c
    hasil_c = (char*) realloc(hasil_c, len_cpy);
    // HAFIZH : Memasukkan hasil copy ke variabel hasil_c
    for(int x = 0; x < selection.len; x++){
        if(currentX < row[currentY].size){
            hasil_c[x] = row[currentY].chars[currentX];
            currentX++;
        }else{
            hasil_c[x] = '\r';
            hasil_c[x+1] = '\n';
            currentX = 0;
            currentY += 1;
        }
    }
    hasil_c[selection.len] = '\0';
    // HAFIZH : Proses memasukkan ke clipboard
    HGLOBAL clipboardText = GlobalAlloc(GMEM_MOVEABLE, len_cpy);
    memcpy(GlobalLock(clipboardText), hasil_c, len_cpy);
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
    // HAFIZH : Handle paste saat select nyala
    if(selection.isOn){
        int currentX = selection.x + selection.len;
        while(currentX != selection.x){
            rowDelChar(&tEditor->row[selection.y], currentX);
            currentX--;
        }
    }
    OpenClipboard(0);
    HANDLE clipboardText = GetClipboardData(CF_TEXT);
    int len_paste = strlen((char*) clipboardText);
    for (int x = 0; x < len_paste; x++){
        if(((char*) clipboardText)[x] == '\n'){
            len_paste--;
            continue;
        }
        insertChar( ((char*) clipboardText)[x]);
    }

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