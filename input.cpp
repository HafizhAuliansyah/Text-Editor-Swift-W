#include "input.h"

void editorUpdateRow(erow *row)
{
    int tabs = 0;
    int j;
    for (j = 0; j < row->size; j++)
        if (row->chars[j] == '\t')
            tabs++;
    int idx = 0;
    for (j = 0; j < row->size; j++)
    {
        if (row->chars[j] == '\t')
        {
            row->render[idx++] = ' ';
            while (idx % SWIFT_TAB_STOP != 0 && idx < MAX_COLUMN)
                row->render[idx++] = ' ';
        }
        else
        {
            row->render[idx++] = row->chars[j];
        }
        if (idx >= MAX_COLUMN)
            break;
    }
    row->render[idx] = '\0';
    row->rsize = idx;
}

void editorInsertRow(int at, char *s, size_t len)
{
    if (teks_editor.numrows >= MAX_ROW)
        return;
    memmove(&teks_editor.row[at + 1], &teks_editor.row[at], sizeof(erow) * (teks_editor.numrows - at));
    teks_editor.row[at].size = len;
    memcpy(&teks_editor.row[at].chars, s, len);
    teks_editor.row[at].chars[len] = '\0';
    teks_editor.row[at].rsize = 0;
    teks_editor.row[at].render[0] = '\0';
    editorUpdateRow(&teks_editor.row[at]);

    teks_editor.numrows++;
//    teks_editor.dirty++;
}

void editorDelRow(int at)
{
    // if (at < 0 || at >= E.numrows)
    //     return;
    // editorFreeRow(&E.row[at]);
    memmove(&teks_editor.row[at], &teks_editor.row[at + 1], sizeof(erow) * (teks_editor.numrows - at - 1));
    teks_editor.numrows--;
//    teks_editor.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c)
{
    // if (at < 0 || at > row->size)
    //     at = row->size;
    // char *dest = row->chars;
    // char *src = row->chars;
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;
    editorUpdateRow(&(*row));
//    teks_editor.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len)
{
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    editorUpdateRow(&(*row));
//    teks_editor.dirty++;
}

void editorRowDelChar(erow *row, int at)
{
    // if (at < 0 || at >= row->size)
    //     return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    editorUpdateRow(row);
//    teks_editor.dirty++;
}

/*** editor operations ***/
void editorInsertChar(int c)
{
    // editorSetStatusMessage(E.row->render);
    if (C.y == teks_editor.numrows)
    {
        editorInsertRow(teks_editor.numrows, "", 0);
    }

    if (teks_editor.row[C.y].size < MAX_COLUMN)
    {
        editorRowInsertChar(&teks_editor.row[C.y], C.x, c);
        C.x++;
    }
    else
    {
//        editorSetStatusMessage("PERINGATAN ! MENCAPAI BATAS COLUMN");
    }
}

void editorDelChar()
{

    if (C.x == 0 && C.y == 0)
        return;
    if (C.x > 0)
    {
        editorRowDelChar(&teks_editor.row[C.y], C.x - 1);
        C.x--;
    }
    else
    {
        C.x = teks_editor.row[C.y - 1].size;
        if (teks_editor.row[C.y - 1].size + teks_editor.row[C.y].size <= MAX_COLUMN)
        {
            editorRowAppendString(&teks_editor.row[C.y - 1], teks_editor.row[C.y].chars, teks_editor.row[C.y].size);
            editorDelRow(C.y);
            C.y--;
        }
        else
        {
//            editorSetStatusMessage("PERINGATAN ! TIDAK BISA MENGHAPUS, KOLOM TIDAK MEMADAI");
            C.x = teks_editor.row[C.y].size;
        }
    }
}

void editorInsertNewline()
{
    if (teks_editor.numrows < MAX_ROW)
    {
        if (C.x == 0)
        {
            editorInsertRow(C.y, "", 0);
        }
        else
        {
            erow *row = &teks_editor.row[C.y];
            editorInsertRow(C.y + 1, &row->chars[C.x], row->size - C.x);
            row = &teks_editor.row[C.y];
            row->size = C.x;
            row->chars[row->size] = '\0';
            editorUpdateRow(&(*row));
        }
        C.y++;
        C.x = 0;
    }
    else
    {
//        editorSetStatusMessage("PERINGATAN ! MENCAPAI BATAS BARIS");
    }
}
