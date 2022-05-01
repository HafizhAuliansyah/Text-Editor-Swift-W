#include "input.h"
teksEditor teks_editor;
int readKey()
{
    DWORD read;
    char key[3];
    HANDLE console_in = getConsoleIn();
    // READ KEYBOARD
    ReadFile(console_in, &key, 3, &read, NULL);

    if (key[0] == '\x1b' && read > 1)
    {
        char seq[3];
        seq[0] = key[1];
        seq[1] = key[2];

        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                if (ReadFile(console_in, &seq[2], 1, &read, NULL) == 0)
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
        return key[0];
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
            setCursorX(searchByIndex(teks_editor.first_row, cursor.y)->info.size);
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
        copyGlobal(teks_editor);
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

void updateRow(infotype *row)
{
    address_column P = row->chars; // untuk pindah pindah
    int idx = 0;
    while (P != Nil)
    {
        if (Info(P) == '\t')
        {
            InsVLastChar(&row->render, ' ');
            idx++;
            while (idx % SWIFT_TAB_STOP != 0)
            {
                InsVLastChar(&row->render, ' ');
                idx++;
            }
        }
        else
        {
            InsVLastChar(&row->render, Info(P));
            idx++;
        }
        P = NextColumn(P);
    }
    row->rsize = row->size;
}

void insertRow(int at, address_column s, int len)
{
    infotype temp;
    address_row row_temp;
    address_row prec_row = searchByIndex(teks_editor.first_row, at - 1);
    // setting row baru
    columnInit(&temp);
    while (s != Nil)
    {
        InsVLastChar(&temp.chars, Info(s));
        s = NextColumn(s);
    }
    temp.size = len;
    // alokasi row baru
    row_temp = Alokasi(temp);
    // insert after prec
    if (teks_editor.first_row == Nil)
    {
        InsertFirst(&teks_editor.first_row, row_temp);
    }
    else
    {
        if (prec_row == Nil)
        {
            InsertFirst(&teks_editor.first_row, row_temp);
        }
        else
        {
            InsertAfter(&teks_editor.first_row, row_temp, prec_row);
        }
    }
    updateRow(&row_temp->info); // sepertinya harus diganti
    teks_editor.numrows++;
    addModified();
}

void deleteRow(int at)
{
    infotype temp;
    if (at - 1 < 0)
    {
        DelVFirst(&teks_editor.first_row, &temp);
    }
    else
    {
        address_row row_prec = searchByIndex(teks_editor.first_row, at - 1);
        address_row row = searchByIndex(teks_editor.first_row, at);
        DelAfter(&teks_editor.first_row, &row, row_prec);
    }
    teks_editor.numrows--;
    addModified();
}

void rowInsertChar(infotype *row, int at, int c)
{
    address_column column_prec = SearchCharByIndex(row->chars, at - 1);
    address_column column_temp;
    if (column_prec == Nil)
    {
        InsVFirstChar(&row->chars, c);
    }
    else
    {
        column_temp = AlokasiChar(c);
        InsertAfterChar(&row->chars, column_temp, column_prec);
    }

    row->size++;
    updateRow(&*row);
    addModified();
}

void rowAppendString(infotype *row, address_column s, int len)
{
    InsertLastChar(&row->chars, s);
    row->size += len;
    updateRow(&(*row));
    addModified();
}

void rowDelChar(infotype *row, int at)
{
    columnInfotype temp;
    if (at - 1 < 0)
    {
        DelVFirstChar(&row->chars, &temp);
    }
    else
    {
        address_column column_prec = SearchCharByIndex(row->chars, at - 1);
        address_column column = SearchCharByIndex(row->chars, at);
        DelAfterChar(&row->chars, &column, column_prec);
    }
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
        address_column s = Nil;
        insertRow(teks_editor.numrows, s, 0);
    }
    address_row cur = searchByIndex(teks_editor.first_row, cursor.y);
    rowInsertChar(&cur->info, cursor.x, c);
    addCursorX();
}

void deleteChar()
{
    cursorHandler cursor = getCursor();
    if (cursor.x == 0 && cursor.y == 0)
        return;
    if (cursor.x > 0)
    {
        rowDelChar(&searchByIndex(teks_editor.first_row, cursor.y)->info, cursor.x - 1);
        setCursorX(cursor.x - 1);
    }
    else
    {
        setCursorX(searchByIndex(teks_editor.first_row, cursor.y - 1)->info.size);
        if (searchByIndex(teks_editor.first_row, cursor.y)->info.chars != Nil)
            rowAppendString(&searchByIndex(teks_editor.first_row, cursor.y - 1)->info, searchByIndex(teks_editor.first_row, cursor.y)->info.chars, searchByIndex(teks_editor.first_row, cursor.y)->info.size);
        deleteRow(cursor.y);
        setCursorY(cursor.y - 1);
    }
}

void insertNewline()
{
    cursorHandler cursor = getCursor();
    if (cursor.x == 0)
    {
        address_column s = Nil;
        insertRow(cursor.y, s, 0);
    }
    else
    {
        address_row row = searchByIndex(teks_editor.first_row, cursor.y);
        insertRow(cursor.y + 1, SearchCharByIndex(teks_editor.first_row->info.chars, cursor.x), row->info.size - cursor.x);
        // replace row sebelumnya
        row->info.size = cursor.x;
        updateRow(&row->info);
    }
    setCursorY(cursor.y + 1);
    setCursorX(0);
}
void inputInit()
{
    teks_editor.numrows = 0;
    CreateRow(&teks_editor.first_row);
}

void columnInit(infotype *row)
{
    row->size = 0;
    row->rsize = 0;
    CreateColumn(&row->chars);
    CreateColumn(&row->render);
}

teksEditor getTeksEditor()
{
    return teks_editor;
}
