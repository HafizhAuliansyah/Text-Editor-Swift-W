#include "input.h"
teksEditor teks_editor;
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
        return key;
    }
}

void keyProcess()
{
    bool skipClearSelect = false;
    static int quit_times = SWIFT_QUIT_TIMES;
    HANDLE console_out = getConsoleOut();
    cursorHandler cursor = getCursor();
    outputHandler output = getOutputHandler();

    int c = readKey();

    switch (c)
    {
    case '\r':
        insertNewline();
        break;
    case CTRL('q'):
        if (getFileHandler().modified && quit_times > 0)
        {
            setMessage("PERINGATAN !! FILE BELUM DISIMPAN TEKAN Ctrl + s UNTUK SIMPAN, Ctrl + q UNTUK KELUAR", quit_times);
            quit_times--;
            return;
        }
        WriteFile(console_out, "\x1b[2J", 4, NULL, NULL);
        WriteFile(console_out, "\x1b[H", 3, NULL, NULL);
        exit(0);
        break;
    case CTRL('s'):
        saveFile();
        break;
    case HOME_KEY:
        setCursorX(0);
        break;
    case END_KEY:

        if (cursor.y < teks_editor.numrows)
            setCursorX(teks_editor.row[cursor.y].size);
        break;
    case CTRL('f'):
        findText(teks_editor);
        skipClearSelect = true;
        break;
    case BACKSPACE:
    case DEL_KEY:
        if (c == DEL_KEY)
            moveCursor(ARROW_RIGHT, teks_editor);
        deleteChar();
        break;
    case PAGE_UP:
    case PAGE_DOWN:
    {
        if (c == PAGE_UP)
        {
            // Pindah ke baris paling atas di layar
            setCursorY(getStartRow());
        }
        else if (c == PAGE_DOWN)
        {
            cursorHandler cursor = getCursor();
            // Pindah ke baris paling bawah di layar
            cursor.y = cursor.start_row + getScreenRows() - 1;
            setCursorY(getStartRow());
            if (cursor.y > teks_editor.numrows)
                setCursorY(teks_editor.numrows - 1);
        }
        int times = getScreenRows();
        while (times--)
            moveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN, teks_editor);
    }
    break;
    // Arrow untuk memindahkan cursor
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
        moveCursor(c, teks_editor);
        break;
    case CTRL('h'):
        setInHelp(true);
    // HANDLE COPY PASTE
    case CTRL('c'):
        copyGlobal(teks_editor.row);
        break;
    case CTRL('v'):
        pasteGlobal();
        break;
        // SELECT
    case SHIFT_ARROW_RIGHT:
    case SHIFT_ARROW_LEFT:
    case SHIFT_ARROW_UP:
    case SHIFT_ARROW_DOWN:
        selectMoveCursor(c, teks_editor);
        skipClearSelect = true;
        break;
    case CTRL('l'):
    case '\x1b':
    {
        if (output.isInHelp)
            setInHelp(false);
    }
    break;
    default:
        if ((c > 26 || c == 9) && !output.isInHelp)
        {
            insertChar(c);
        }
        break;
    }
    if (!skipClearSelect)
        // Matikan selection text
        clearSelected();
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

void insertRow(int at,const char *s, size_t len)
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
    addModified();
}

void deleteRow(int at)
{
    // if (at < 0 || at >= teks_editor.numrows)
    // return;
    // editorFreeRow(&E.row[at]);
    memmove(&teks_editor.row[at], &teks_editor.row[at + 1], sizeof(erow) * (teks_editor.numrows - at - 1));
    teks_editor.numrows--;
    addModified();
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
    addModified();
}

void rowAppendString(erow *row, char *s, size_t len)
{
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    updateRow(&(*row));
    addModified();
}

void rowDelChar(erow *row, int at)
{
    // if (at < 0 || at >= row->size)
    //     return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    updateRow(row);

    addModified();
}

/*** editor operations ***/
void insertChar(int c)
{
    cursorHandler cursor = getCursor();
    
    if (cursor.y == teks_editor.numrows)
    {
        insertRow(teks_editor.numrows, "", 0);
    }

    if (teks_editor.row[cursor.y].size < MAX_COLUMN)
    {
        rowInsertChar(&teks_editor.row[cursor.y], cursor.x, c);
        addCursorX();

    }
    else
    {
        setMessage("PERINGATAN ! MENCAPAI BATAS COLUMN");
    }
}

void deleteChar()
{
    cursorHandler cursor = getCursor();
    if (cursor.x == 0 && cursor.y == 0)
        return;
    if (cursor.x > 0)
    {
        rowDelChar(&teks_editor.row[cursor.y], cursor.x - 1);
        setCursorX(cursor.x - 1);
    }
    else
    {
        setCursorX(teks_editor.row[cursor.y - 1].size);
        if (teks_editor.row[cursor.y - 1].size + teks_editor.row[cursor.y].size <= MAX_COLUMN)
        {
            rowAppendString(&teks_editor.row[cursor.y - 1], teks_editor.row[cursor.y].chars, teks_editor.row[cursor.y].size);
            deleteRow(cursor.y);
            setCursorY(cursor.y - 1);
        }
        else
        {
            setMessage("PERINGATAN ! TIDAK BISA MENGHAPUS, KOLOM TIDAK MEMADAI");
            setCursorX(teks_editor.row[cursor.y].size);
        }
    }
}

void insertNewline()
{
    cursorHandler cursor = getCursor();
    if (teks_editor.numrows < MAX_ROW)
    {
        if (cursor.x == 0)
        {
            insertRow(cursor.y, "", 0);
        }
        else
        {
            erow *row = &teks_editor.row[cursor.y];
            insertRow(cursor.y + 1, &row->chars[cursor.x], row->size - cursor.x);
            row = &teks_editor.row[cursor.y];
            row->size = cursor.x;
            row->chars[row->size] = '\0';
            updateRow(&(*row));
        }
        setCursorY(cursor.y + 1);
        setCursorX(0);
    }
    else
    {
        setMessage("PERINGATAN ! MENCAPAI BATAS BARIS");
    }
}
void inputInit()
{
    teks_editor.numrows = 0;
}

teksEditor getTeksEditor()
{
    return teks_editor;
}
