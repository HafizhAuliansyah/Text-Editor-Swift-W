#include "text_operation.h"

selectionText selection;

char *hasil_c;
void addSelectionText(struct abuf *ab, char *row, int len)
{
    // var at, sebagai penampung koordinat kolom
    int at = 0;
    // Memasukkan kolom sebelum kata terselect ke ab
    // COMMENTED abAppend(ab, &row[at], selection.x);
    // Select Text sesuai kolom selection.x, sejumlah selection.len ke kanan
    // COMMENTED abAppend(ab, "\x1b[7m", 4);
    at = selection.x;
    // COMMENTED abAppend(ab, &row[at], selection.len);
    // COMMENTED abAppend(ab, "\x1b[m", 3);
    // Memasukkan kolom setelah kata terselect ke ab
    at = selection.x + selection.len;
    // COMMENTED abAppend(ab, &row[at], len - at);
}
void selectMoveCursor(int c, teksEditor tEditor)
{
    // editorSetStatusMessage("%d cx, %d size", C.x,E.row[C.y].size);
    selectionText dest;
    cursorHandler C = getCursor();
    dest.x = C.x;
    dest.y = C.y;
    dest.len = selection.len;
    switch (c)
    {
    case SHIFT_ARROW_LEFT:
        if (C.x == 0)
            return;
        moveCursor(ARROW_LEFT, tEditor);
        dest.x = C.x;
        dest.len++;
        break;
    case SHIFT_ARROW_RIGHT:
        if (C.x >= tEditor.row[C.y].size)
            return;
        dest.x = C.x - dest.len;
        moveCursor(ARROW_RIGHT, tEditor);
        dest.len++;
        break;
    case SHIFT_ARROW_UP:
    case SHIFT_ARROW_DOWN:
        // COMMENTED editorSetStatusMessage("FITUR INI BELUM TERSEDIA");
        break;
    default:
        // COMMENTED editorSetStatusMessage("Other");
        break;
    }
    if (dest.y != getCursor().y)
    {
        // COMMENTED editorSetStatusMessage("FITUR INI BELUM TERSEDIA");
    }
    setCursor(C);
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

void copy(erow row[])
{
    hasil_c = (char*) realloc(hasil_c, selection.len + 1);
    memmove(hasil_c, &row[selection.y].chars[selection.x], selection.len);
    hasil_c[selection.len] = '\0';
}

void paste()
{
    int column_len = MAX_COLUMN - getCursor().x;
    for (int x = 0; x < strlen(hasil_c); x++)
        InsertChar(hasil_c[x]);
}
/** Find **/
void findText(teksEditor tEditor)
{
    char *query;
    // COMMENTED *query = editorPrompt("Cari : %s (Tekan ESC Untuk Batalkan)", 7);
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
            int screenrows; // TODO get screenrows
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