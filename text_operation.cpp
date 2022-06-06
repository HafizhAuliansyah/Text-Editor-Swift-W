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
    // Handle select multiline, mengubah awal select Y ke baris setelahnya
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
    // HAFIZH : Kondisional Shift Select
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
        if (getCursor().x >= searchByIndex(tEditor.first_row, getCursor().y)->info.size){
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
        if(selection.x != getCursor().x || selection.y != getCursor().y && selection.isOn){
            sizeFirst = getCursor().x;
            moveCursor(ARROW_UP, tEditor);
            dest.x = selection.x;
            dest.y = selection.y;
            sizeLast =  searchByIndex(tEditor.first_row, getCursor().y)->info.size - getCursor().x;
            dest.len -= (sizeFirst + sizeLast);
            // HAFIZH : IF pengurangan negatif
            if(dest.len < 0){
                dest.len = abs(dest.len);
                dest.x = getCursor().x;
                dest.y = getCursor().y;
            }
        }else{
            // HAFIZH : ELSE Proses selection up normal
            sizeFirst = dest.x;
            moveCursor(ARROW_UP, tEditor);
            dest.x = getCursor().x;
            dest.y = getCursor().y;
            sizeLast = searchByIndex(tEditor.first_row, dest.y)->info.size - dest.x;
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
            sizeFirst = searchByIndex(tEditor.first_row, getCursor().y)->info.size - getCursor().x;
            sizeLast = getCursor().x;
            moveCursor(ARROW_DOWN, tEditor);
            dest.x = getCursor().x;
            dest.y = getCursor().y;
            dest.len -= (sizeFirst + sizeLast);
            // HAFIZH : IF hasil pengurangan negatif
            if(dest.len < 0){
                dest.len = abs(dest.len);
                if(selection.x + selection.len < searchByIndex(tEditor.first_row, selection.y)->info.size){
                    dest.x = selection.x + selection.len;
                    dest.y = selection.y;
                }else{
                    dest.x = selection.x - dest.len;
                    dest.y = getCursor().y;
                }
            }
        }else{
            // HAFIZH : ELSE proses selection down normal
            if(selection.isOn){
                dest.x = selection.x;
                dest.y = selection.y;
            }
            sizeFirst = searchByIndex(tEditor.first_row, getCursor().y)->info.size - getCursor().x;
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
    // HAFIZH : Proses mengisi hasil_c, untuk digunakan secara lokal
    hasil_c = (char *)realloc(hasil_c, selection.len + 1);
    memmove(hasil_c, &row[selection.y].chars[selection.x], selection.len);
    hasil_c[selection.len] = '\0';
}
void copyGlobal(teksEditor tEditor)
{
    // HAFIZH : Membuka clipboard berdasarkan proses/task saat ini
    OpenClipboard(0);
    // HAFIZH : Mengosongkan isi clipboard
    EmptyClipboard();
    // HAFIZH : Deklarasi tracker variabel
    int currentY = selection.y;
    int currentX = selection.x;
    int EOL = 0;
    // HAFIZH : Looping mencari jumlah End Of line
    for(int s = 0; s < selection.len; s++){
        int size_row = searchByIndex(tEditor.first_row, currentY)->info.size;
        if(currentX < size_row){
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
    for(int x = 0; x < len_cpy - 1; x++){
        address_row row = searchByIndex(tEditor.first_row, currentY);
        if(currentX < row->info.size){
            char c = SearchCharByIndex(row->info.chars, currentX)->info;
            hasil_c[x] = c;
            currentX++;
        }else{
            // Handle MultiLine
            hasil_c[x] = '\r';
            x += 1;
            hasil_c[x] = '\n';
            currentX = 0;
            currentY += 1;
        }
    }
    hasil_c[len_cpy] = '\0';
    // HAFIZH : Alokasi ClipboardText sesuai panjang hasil_c
    HGLOBAL clipboardText = GlobalAlloc(GMEM_MOVEABLE, len_cpy);
    // HAFIZH : clopboardText diisi hasil_c
    memcpy(GlobalLock(clipboardText), hasil_c, len_cpy);
    // HAFIZH : Mengubah clopboardTetxt yang asalnya pointer menjadi tidak
    GlobalUnlock(clipboardText);
    // HAFIZH : Mengubah isi clipboard text
    SetClipboardData(CF_TEXT, clipboardText);
    // HAFIZH : Tutup Clipboard
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
    int currentY = selection.isOn? selection.y : getCursor().y;
    int currentX = selection.isOn? selection.x : getCursor().x;
    address_row currentRow;
    // HAFIZH : Looping menghapus teks yang terselect terlebih dahulu
    if(selection.isOn){
         // HAFIZH : Looping mencari awal index untuk penhapusan
        for(int x = 0; x < selection.len; x++){
            currentRow = searchByIndex(tEditor->first_row, currentY);
            if(currentX < currentRow->info.size){
                currentX++;
            }else{
                currentX = 1;
                currentY++;
            }
        }
         // HAFIZH : Memindahkan cursor ke posisi tersebut
        setCursorX(currentX);
        setCursorY(currentY);
        // HAFIZH : Mulai penghapusan
        while(getCursor().x != selection.x || getCursor().y != selection.y){
            deleteChar();
        }
        currentY = selection.y;
    }
    // HAFIZH : Membuka clipboard
    OpenClipboard(0);
    // HAFIZH : Mengisi isi clipboard kedalam clipboardText, dalam format teks
    HANDLE clipboardText = GetClipboardData(CF_TEXT);
    int len_paste = strlen((char*) clipboardText);
    // HAFIZH : Proses insert karakter ke teks editor
    for (int x = 0; x < len_paste; x++){
        if(((char*) clipboardText)[x] == '\n'){
            continue;
        }
        if(((char*) clipboardText)[x] == '\r'){
            insertNewline();
            continue;
        }
        insertChar( ((char*) clipboardText)[x]);
    }
    // HAFIZH : menutup clipboard
    CloseClipboard();
}
void cut(teksEditor *tEditor){
    // HAFIZH : Memindahkan isi teks ke clipboard terlebih dahulu
    copyGlobal(*tEditor);
    // HAFIZH : Proses menghapus teks nya
    int currentY = selection.isOn? selection.y : getCursor().y;
    int currentX = selection.isOn? selection.x : getCursor().x;
    address_row currentRow;
    if(selection.isOn){
        // HAFIZH : Looping mencari awal index untuk penhapusan
        for(int x = 0; x < selection.len; x++){
            currentRow = searchByIndex(tEditor->first_row, currentY);
            if(currentX < currentRow->info.size){
                currentX++;
            }else{
                currentX = 1;
                currentY++;
            }
        }
        // HAFIZH : Memindahkan cursor ke posisi tersebut
        setCursorX(currentX);
        setCursorY(currentY);
        // HAFIZH : Mulai penghapusan
        while(getCursor().x != selection.x || getCursor().y != selection.y){
            deleteChar();
        }

    }
}

void deleteSelect(teksEditor *tEditor){
    int currentY = selection.y;
    int currentX = selection.x;
    address_row currentRow;
    for(int x = 0; x < selection.len; x++){
        // HAFIZH : Looping mencari awal index untuk penhapusan
            currentRow = searchByIndex(tEditor->first_row, currentY);
            if(currentX < currentRow->info.size){
                currentX++;
            }else{
                currentX = 1;
                currentY++;
            }
        }
        // HAFIZH : Memindahkan cursor ke posisi tersebut
        setCursorX(currentX);
        setCursorY(currentY);
        // HAFIZH : Mulai penghapusan
        while(getCursor().x != selection.x || getCursor().y != selection.y){
            deleteChar();
        }
}
/** Find **/
void findText(teksEditor tEditor,bool replaceAll,char *textPengganti,char *textDicari)
{
    char *query;
    if(replaceAll){
        query = (char*)malloc(sizeof(textDicari));
        query = textDicari;
    } else {
        char *temp = setInputMassage("Cari : %s (Tekan ESC Untuk Batalkan)", 7);
        query = (char*)malloc(sizeof(temp));
        query = temp;
    }
    if (query == NULL)
        return;
    int ketemu = 0;
    int i = 0;
    while (i < tEditor.numrows)
    {
        // Konversi list ke char*
        infotype row = searchByIndex(tEditor.first_row, i)->info;
        address_column column = row.render;
        char *tempRow = (char *)malloc((row.rsize + 1) * sizeof(char));
        tempRow[row.rsize] = '\0';
        int index = 0;
        while (index < row.rsize)
        {
            tempRow[index] = Info(column);
            column = NextColumn(column);
            index++;
        }
        char *match = strstr(tempRow, query);
        if (match)
        {
            setCursorY(i);
            setCursorX(renderXToCursorX(&row, match - tempRow));

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

            ketemu = 1;
            if(replaceAll){
                replace(&tEditor,textPengganti);
            }else{
                char *pilih = setInputMassage("Ubah Text? : %s (Y/T)", 13);
                if(toupper(pilih[0]) == 'Y'){
                    char *pengganti = setInputMassage("Text Pengganti : %s ", 17);
                    char *isReplaceAll = setInputMassage("Ubah Semua? : %s (Y/T)",14);
                    if(toupper(isReplaceAll[0]) == 'Y'){
                        replace(&tEditor,pengganti);
                        findText(tEditor,true,pengganti,query);
                        break;
                    } else if(toupper(isReplaceAll[0]) == 'T'){
                        replace(&tEditor,pengganti);
                        break;
                    }
                } else if(toupper(pilih[0]) == 'T') break;
                
            }
        } else {
            i++;
        }
    }
    if (!ketemu)
    {
        setMessage("Teks Tidak Ada!");
    }

    free(query);
    
}

void replace (teksEditor *tEditor,char *pengganti){
    deleteSelect(tEditor);
    clearSelected();
    for(int i = 0; i<strlen(pengganti); i++){
        insertChar(pengganti[i]);
    }
}

selectionText getSelection()
{
    return selection;
}
void setSelection(selectionText new_selection)
{
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
