#include "output.h"

outputHandler outputConfig;

void drawRows(outputBuffer *ob)
{
    teksEditor tEditor = getTeksEditor();
    selectionText selection = getSelection();
    int y;
    char **help = NULL;
    int help_len;
    if (outputConfig.isInHelp)
    {
        help = openHelp(&help_len);
    }

    for (y = 0; y < getScrennRows(); y++)
    {
        int filerow = y + getCursor().start_row;
        if (outputConfig.isInHelp)
        {
            if (filerow < help_len)
            {
                int len = strlen(help[filerow]) - getCursor().start_col;
                bufferAppend(ob, help[filerow], len);
            }
        }
        else
        {
            if (filerow >= tEditor.numrows)
            {
                if (tEditor.numrows == 0 && y == MAX_ROW / 2)
                {
                    char welcome[80];
                    int welcomelen = snprintf(welcome, sizeof(welcome), "Swift Text Editor");
                    if (welcomelen > getScrenCols())
                        welcomelen = getScrenCols();
                    int padding = (getScrenCols() - welcomelen) / 2;
                    if (padding)
                    {
                        if (y < MAX_ROW)
                        {
                            bufferAppend(ob, "~", 1);
                        }
                        padding--;
                    }
                    while (padding--)
                        bufferAppend(ob, " ", 1);
                    bufferAppend(ob, welcome, welcomelen);
                }
                else
                {
                    if (y < MAX_ROW)
                    {
                        bufferAppend(ob, "~", 1);
                    }
                }
            }
            else
            {
                int len = tEditor.row[filerow].rsize - getStartCol();
                if (len < 0)
                    len = 0;
                if (len > getScrenCols())
                    len = getScrenCols();

                // Konversi char ke char*
                char *c = &tEditor.row[filerow].render[getStartCol()];

                // Select Text
                if (filerow == selection.y && getStartCol() <= selection.x && selection.isOn)
                {
                    addSelectionText(ob, c, len);
                }
                else
                {
                    bufferAppend(ob, c, len);
                }
            }
        }
        bufferAppend(ob, "\x1b[K", 3);
        bufferAppend(ob, "\r\n", 2);
    }
    free(help);
}

void addStatusBar(outputBuffer *ob)
{
    cursorHandler C = getCursor();
    teksEditor tEditor = getTeksEditor();
    bufferAppend(ob, "\x1b[7m", 4);
    char status[80], rstatus[80];
    int len = snprintf(status, sizeof(status), "%.20s - %d baris %s",
                       getFileHandler().filename ? getFileHandler().filename : "[File Kosong]", tEditor.numrows,
                       getFileHandler().modified ? "(modified)" : "");
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", C.y + 1, tEditor.numrows);
    if (len > getScrenCols())
        len = getScrenCols();
    bufferAppend(ob, status, len);
    while (len < getScrenCols())
    {
        if (getScrenCols() - len == rlen)
        {
            bufferAppend(ob, rstatus, rlen);
            break;
        }
        else
        {
            bufferAppend(ob, " ", 1);
            len++;
        }
    }
    bufferAppend(ob, "\x1b[m", 3);
    bufferAppend(ob, "\r\n", 2);
}

void addMessageBar(outputBuffer *ob)
{
    bufferAppend(ob, "\x1b[K", 3);
    int msglen = strlen(outputConfig.statusmsg);
    if (msglen > getScrenCols())
        msglen = getScrenCols();
    if (msglen && time(NULL) - outputConfig.statusmsg_time < 5)
        bufferAppend(ob, outputConfig.statusmsg, msglen);
}

void refreshScreen()
{
    cursorHandler C = getCursor();
    cursorHandler stat_cursor = getMessageCursor();
    cursorScroll(getTeksEditor());

    outputBuffer ob = OUTPUT_INIT;

    bufferAppend(&ob, "\x1b[?25l", 6);
    bufferAppend(&ob, "\x1b[H", 3);

    drawRows(&ob);
    addStatusBar(&ob);
    addMessageBar(&ob);

    char buf[32];
    int y = outputConfig.isInStatus ? stat_cursor.y + 1 : (C.y - getStartRow()) + 1;
    int x = outputConfig.isInStatus ? stat_cursor.x + 1 : (C.rx - getStartCol()) + 1;
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);

    bufferAppend(&ob, buf, strlen(buf));
    bufferAppend(&ob, "\x1b[?25h", 6);
    WriteFile(getConsoleOut, ob.buffer, ob.len, NULL, NULL);
    bufferFree(&ob);
}

void setMessage(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(outputConfig.statusmsg, sizeof(outputConfig.statusmsg), fmt, ap);
    va_end(ap);
    outputConfig.statusmsg_time = time(NULL);
}

char *rowsToString(int *buflen)
{
    teksEditor tEditor = getTeksEditor();
    int totlen = 0;
    int j;
    for (j = 0; j < tEditor.numrows; j++)
        totlen += tEditor.row[j].size + 1;
    *buflen = totlen;

    char *buf = (char*) malloc(totlen);
    char *p = buf;
    for (j = 0; j < tEditor.numrows; j++)
    {
        memcpy(p, tEditor.row[j].chars, tEditor.row[j].size);
        p += tEditor.row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

char *setInputMassage(char *prompt, int start_cx)
{
    // Deklarasi variabel penampung nama file, dengan size 128 bytes
    size_t name_size = 128;
    char *filename = (char*) malloc(name_size);

    // Inisialisasi awal size isi, dan zero character
    size_t name_len = 0;
    filename[0] = '\0';

    // Pindah cursor kebawah
    outputConfig.isInStatus = true;
    cursorHandler stat_cursor = getMessageCursor();
    stat_cursor.y = getScrennRows() + 2;
    stat_cursor.x = start_cx;

    while (1)
    {
        // Membuka status bar, untuk menerima input ke filename
        setMessage(prompt, filename);
        refreshScreen();

        // Membaca input untuk mengisi file name
        int c = readKey();

        // Mini delete character handler
        if (c == DEL_KEY || c == CTRL('h') || c == BACKSPACE)
        {
            if (name_len != 0)
            {
                filename[--name_len] = '\0';
                stat_cursor.x--;
            }
        }
        // Escape, untuk keluar dari editor Prompt
        else if (c == '\x1b')
        {
            setMessage("");
            free(filename);
            outputConfig.isInStatus = false;
            return NULL;
        }
        // Memasukkan input ke filename
        else
        {
            // Enter, untuk menghentikan input
            if (c == '\r')
            {
                if (name_len != 0)
                {
                    setMessage("");
                    outputConfig.isInStatus = false;
                    return filename;
                }
            }
            // Jika input bukan control character, masukkan input ke filename
            else if (!iscntrl(c) && c < 128)
            {
                // Menambah size filename, ketika hampir penuh
                if (name_len == name_size - 1)
                {
                    name_size *= 2;
                    filename = (char*) realloc(filename, name_size);
                }
                // Mengisi filename
                filename[name_len++] = c;
                filename[name_len] = '\0';
                stat_cursor.x++;
            }
        }
        setMessageCursor(stat_cursor);
    }
}
outputHandler getOutputHandler(){
    return outputConfig;
}
void setInStatus(bool new_status){
    outputConfig.isInStatus = new_status;
}
void setInHelp(bool new_help){
    outputConfig.isInHelp = new_help;
}