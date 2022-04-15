#include "input.h"
cursorHandler C;

int readKey()
{
    DWORD read;
    char key;
    HANDLE console_in = getConsoleIn();
    // READ KEYBOARD
    ReadFile(console_in, &key, 1, &read, NULL);

    if (key == '\x1b')
    {
        char seq[3];
        if (ReadFile(console_in, &seq[0], 1, &read, NULL) != 1)
            return '\x1b';
        if (ReadFile(console_in, &seq[1], 1, &read, NULL) != 1)
            return '\x1b';

        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                if (ReadFile(console_in, &seq[2], 1, &read, NULL) != 1)
                    return '\x1b';
                if (seq[2] == '~')
                {
                    switch (seq[1])
                    {
                    case '1':
                        return HOME_KEY;
                    case '3':
                        return DEL_KEY;
                    case '4':
                        return END_KEY;
                    case '5':
                        return PAGE_UP;
                    case '6':
                        return PAGE_DOWN;
                    case '7':
                        return HOME_KEY;
                    case '8':
                        return END_KEY;
                    }
                }
                if (seq[2] == ';')
                {
                    char shf[2];
                    if (ReadFile(console_in, &shf[0], 1, &read, NULL) != 1)
                        return '\x1b';
                    if (ReadFile(console_in, &shf[1], 1, &read, NULL) != 1)
                        return '\x1b';
                    switch (shf[1])
                    {
                    case 'C':
                        return SHIFT_ARROW_RIGHT;
                    case 'D':
                        return SHIFT_ARROW_LEFT;
                    case 'A':
                        return SHIFT_ARROW_UP;
                    case 'B':
                        return SHIFT_ARROW_DOWN;
                    }
                }
            }
            else
            {
                switch (seq[1])
                {
                case 'A':
                    return ARROW_UP;
                case 'B':
                    return ARROW_DOWN;
                case 'C':
                    return ARROW_RIGHT;
                case 'D':
                    return ARROW_LEFT;
                case 'H':
                    return HOME_KEY;
                case 'F':
                    return END_KEY;
                }
            }
        }
        else if (seq[0] == 'O')
        {
            switch (seq[1])
            {
            case 'H':
                return HOME_KEY;
            case 'F':
                return END_KEY;
            }
        }
        return '\x1b';
    }
    else
    {
        return c;
    }
}

void keyProcess()
{
    bool skipClearSelect = false;
    static int quit_times = SWIFT_QUIT_TIMES;
    HANDLE console_out = getConsoleOut();

    int c = readKey();

    switch (c)
    {
    case '\r':
        insertNewline();
        break;
    case CTRL('q'):
        //        if (E.dirty && quit_times > 0)
        //        {
        //            editorSetStatusMessage("PERINGATAN !! FILE BELUM DISIMPAN TEKAN Ctrl + s UNTUK SIMPAN, Ctrl + q UNTUK KELUAR", quit_times);
        //            quit_times--;
        //            return;
        //        }
        WriteFile(console_out, "\x1b[2J", 4, NULL, NULL);
        WriteFile(console_out, "\x1b[H", 3, NULL, NULL);
        exit(0);
        break;
    case CTRL_KEY('s'):
        //        editorSave();
        //        break;
    case HOME_KEY:
        C.x = 0;
        break;
    case END_KEY:
        if (C.y < teks_editor.numrows)
            C.x = teks_editor.row[C.y].size;
        break;
    case CTRL_KEY('f'):
        //        editorFind();
        //        skipClearSelect = true;
        //        break;
    case BACKSPACE:
    case DEL_KEY:
        //        if (c == DEL_KEY)
        //            editorMoveCursor(ARROW_RIGHT);
        //        deleteChar();
        //        break;
    case PAGE_UP:
    case PAGE_DOWN:
    {
        //        if (c == PAGE_UP)
        //        {
        //            // Pindah ke baris paling atas di layar
        //            C.y = E.rowoff;
        //        }
        //        else if (c == PAGE_DOWN)
        //        {
        //            // Pindah ke baris paling bawah di layar
        //            C.y = E.rowoff + E.screenrows - 1;
        //            if (C.y > E.numrows)
        //                C.y = E.numrows - 1;
        //        }
        //        int times = E.screenrows;
        //        while (times--)
        //            editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
    }
    // Arrow untuk memindahkan cursor
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
        //        editorMoveCursor(c);
        //        break;
    case CTRL_KEY('h'):
        isInHelp = true;
    // HANDLE COPY PASTE
    case CTRL_KEY('c'):
        //        copy(teks_editor.row);
        //        break;
    case CTRL_KEY('v'):
        //        paste();
        //        break;
        // SELECT
    case SHIFT_ARROW_RIGHT:
    case SHIFT_ARROW_LEFT:
    case SHIFT_ARROW_UP:
    case SHIFT_ARROW_DOWN:
        //        selectMoveCursor(c);
        //        skipClearSelect = true;
        //        break;
    case CTRL_KEY('l'):
    case '\x1b':
    {
        if (isInHelp)
            isInHelp = false;
    }
    break;
    default:
        if ((c > 26 || c == 9) && !isInHelp)
        {
            insertChar(c);
        }
        break;
    }
    //    if (!skipClearSelect)
    // Matikan selection text
    //        clearSelected(&selection);
    quit_times = SWIFT_QUIT_TIMES;
}

void updateRow(erow *row)
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

void insertRow(int at, char *s, size_t len)
{
    if (teks_editor.numrows >= MAX_ROW)
        return;
    memmove(&teks_editor.row[at + 1], &teks_editor.row[at], sizeof(erow) * (teks_editor.numrows - at));
    teks_editor.row[at].size = len;
    memcpy(&teks_editor.row[at].chars, s, len);
    teks_editor.row[at].chars[len] = '\0';
    teks_editor.row[at].rsize = 0;
    teks_editor.row[at].render[0] = '\0';
    updateRow(&teks_editor.row[at]);

    teks_editor.numrows++;
    //    teks_editor.dirty++;
}

void deleteRow(int at)
{
    // if (at < 0 || at >= E.numrows)
    //     return;
    // editorFreeRow(&E.row[at]);
    memmove(&teks_editor.row[at], &teks_editor.row[at + 1], sizeof(erow) * (teks_editor.numrows - at - 1));
    teks_editor.numrows--;
    //    teks_editor.dirty++;
}

void rowInsertChar(erow *row, int at, int c)
{
    // if (at < 0 || at > row->size)
    //     at = row->size;
    // char *dest = row->chars;
    // char *src = row->chars;
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;
    updateRow(&(*row));
    //    teks_editor.dirty++;
}

void rowAppendString(erow *row, char *s, size_t len)
{
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    updateRow(&(*row));
    //    teks_editor.dirty++;
}

void rowDelChar(erow *row, int at)
{
    // if (at < 0 || at >= row->size)
    //     return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    updateRow(row);
    //    teks_editor.dirty++;
}

/*** editor operations ***/
void insertChar(int c)
{
    // editorSetStatusMessage(E.row->render);
    if (C.y == teks_editor.numrows)
    {
        insertRow(teks_editor.numrows, "", 0);
    }

    if (teks_editor.row[C.y].size < MAX_COLUMN)
    {
        rowInsertChar(&teks_editor.row[C.y], C.x, c);
        C.x++;
    }
    else
    {
        //        editorSetStatusMessage("PERINGATAN ! MENCAPAI BATAS COLUMN");
    }
}

void deleteChar()
{

    if (C.x == 0 && C.y == 0)
        return;
    if (C.x > 0)
    {
        rowDelChar(&teks_editor.row[C.y], C.x - 1);
        C.x--;
    }
    else
    {
        C.x = teks_editor.row[C.y - 1].size;
        if (teks_editor.row[C.y - 1].size + teks_editor.row[C.y].size <= MAX_COLUMN)
        {
            rowAppendString(&teks_editor.row[C.y - 1], teks_editor.row[C.y].chars, teks_editor.row[C.y].size);
            deleteRow(C.y);
            C.y--;
        }
        else
        {
            //            editorSetStatusMessage("PERINGATAN ! TIDAK BISA MENGHAPUS, KOLOM TIDAK MEMADAI");
            C.x = teks_editor.row[C.y].size;
        }
    }
}

void insertNewline()
{
    if (teks_editor.numrows < MAX_ROW)
    {
        if (C.x == 0)
        {
            insertRow(C.y, "", 0);
        }
        else
        {
            erow *row = &teks_editor.row[C.y];
            insertRow(C.y + 1, &row->chars[C.x], row->size - C.x);
            row = &teks_editor.row[C.y];
            row->size = C.x;
            row->chars[row->size] = '\0';
            updateRow(&(*row));
        }
        C.y++;
        C.x = 0;
    }
    else
    {
        //        editorSetStatusMessage("PERINGATAN ! MENCAPAI BATAS BARIS");
    }
}
