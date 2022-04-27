#include "text_operation.h"

selectionText selection;

char *hasil_c;
void addSelectionText(outputBuffer *ob, char *row, int len)
{
    // var at, sebagai penampung koordinat kolom
    int at = 0;
    // Memasukkan kolom sebelum kata terselect ke ab
    bufferAppend(ob, &row[at], selection.x);
    // Select Text sesuai kolom selection.x, sejumlah selection.len ke kanan
    bufferAppend(ob, "\x1b[7m", 4);
    at = selection.x;
    bufferAppend(ob, &row[at], selection.len);
    bufferAppend(ob, "\x1b[m", 3);
    // Memasukkan kolom setelah kata terselect ke ab
    at = selection.x + selection.len;
    bufferAppend(ob, &row[at], len - at);
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
        if (getCursor().x == 0)
            return;
        moveCursor(ARROW_LEFT, tEditor);
        dest.x = getCursor().x;
        dest.len++;
        break;
    case SHIFT_ARROW_RIGHT:
        // if (getCursor().x >= tEditor.row[getCursor().y].size)
        //   return;
        dest.x = getCursor().x - dest.len;
        moveCursor(ARROW_RIGHT, tEditor);
        dest.len++;
        break;
    case SHIFT_ARROW_UP:
    case SHIFT_ARROW_DOWN:
        setMessage("FITUR INI BELUM TERSEDIA");
        break;
    default:
        setMessage("Other");
        break;
    }
    if (dest.y != getCursor().y)
    {
        setMessage("FITUR INI BELUM TERSEDIA");
    }
    selectShift(dest);
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
    hasil_c = (char *)realloc(hasil_c, selection.len + 1);
    memmove(hasil_c, &row[selection.y].chars[selection.x], selection.len);
    hasil_c[selection.len] = '\0';
}
void copyGlobal(erow row[])
{
    OpenClipboard(0);
    EmptyClipboard();
    hasil_c = (char *)realloc(hasil_c, selection.len + 1);
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
        insertChar(hasil_c[x]);
}
void pasteGlobal()
{
    OpenClipboard(0);
    HANDLE clipboardText = GetClipboardData(CF_TEXT);
    int column_len = MAX_COLUMN - getCursor().x;
    for (int x = 0; x < strlen((char *)clipboardText); x++)
        insertChar(((char *)clipboardText)[x]);
    CloseClipboard();
}
/** Find **/
// void findText(teksEditor tEditor)
// {
//     char *query = setInputMassage("Cari : %s (Tekan ESC Untuk Batalkan)", 7);
//     if (query == NULL)
//         return;
//     int ketemu = 1;
//     int i;
//     for (i = 0; i < tEditor.numrows; i++)
//     {
//         erow *row = &tEditor.row[i];
//         char *match = strstr(row->render, query);
//         if (match)
//         {
//             setCursorY(i);
//             setCursorX(renderXToCursorX(row, match - row->render));

//             // Untuk select text
//             selection.y = getCursor().y;
//             selection.x = getCursor().x;
//             selection.len = strlen(query);
//             selection.isOn = true;
//             int screenrows = getScreenRows(); // TODO get screenrows
//             if (i >= screenrows)
//             {
//                 setStartRow(getCursor().y);
//             }

//             ketemu = 0;
//             break;
//         }
//     }
//     if (ketemu)
//     {
//         // COMMENTED editorSetStatusMessage("Teks Tidak Ada!");
//     }

//     free(query);
// }
selectionText getSelection()
{
    return selection;
}
void setSelection(selectionText new_selection)
{
    selection = new_selection;
}
