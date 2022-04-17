#include "output.h"

void editorScroll()
{
    // Tab Detector and Handler
    C.rx = 0;
    if (C.y < E.numrows)
    {
        C.rx = editorRowCxToRx(&E.row[C.y], C.x);
    }
    // Pengaturan row offset ketika scroll keatas
    if (C.y < E.rowoff)
    {
        E.rowoff = C.y;
    }
    // Pengaturan row offset ketika scroll kebawah
    if (C.y >= E.rowoff + E.screenrows)
    {
        E.rowoff = C.y - E.screenrows + 1;
    }
    // Pengaturan coll offset ketika scroll ke kiri
    if (C.rx < E.coloff)
    {
        E.coloff = C.rx;
    }
    // Pengaturan coll offset ketika scroll ke kanan
    if (C.rx >= E.coloff + E.screencols)
    {
        E.coloff = C.rx - E.screencols + 1;
    }
}

void editorDrawRows(struct abuf *ab)
{
    int y;
    char **help = NULL;
    int help_len;
    if (isInHelp)
    {
        help = openHelp(&help_len);
    }

    for (y = 0; y < E.screenrows; y++)
    {
        int filerow = y + E.rowoff;
        if (isInHelp)
        {
            if (filerow < help_len)
            {
                int len = strlen(help[filerow]) - E.coloff;
                abAppend(ab, help[filerow], len);
            }
        }
        else
        {
            if (filerow >= E.numrows)
            {
                if (E.numrows == 0 && y == MAX_ROW / 2)
                {
                    char welcome[80];
                    int welcomelen = snprintf(welcome, sizeof(welcome), "Swift editor -- version %s", SWIFT_VERSION);
                    if (welcomelen > E.screencols)
                        welcomelen = E.screencols;
                    int padding = (E.screencols - welcomelen) / 2;
                    if (padding)
                    {
                        if (y < MAX_ROW)
                        {
                            abAppend(ab, "~", 1);
                        }
                        padding--;
                    }
                    while (padding--)
                        abAppend(ab, " ", 1);
                    abAppend(ab, welcome, welcomelen);
                }
                else
                {
                    if (y < MAX_ROW)
                    {
                        abAppend(ab, "~", 1);
                    }
                }
            }
            else
            {
                int len = E.row[filerow].rsize - E.coloff;
                if (len < 0)
                    len = 0;
                if (len > E.screencols)
                    len = E.screencols;

                // Konversi char ke char*
                char *c = &E.row[filerow].render[E.coloff];

                // Select Text
                if (filerow == selection.y && E.coloff <= selection.x && selection.isOn)
                {
                    addSelectionText(ab, c, len);
                }
                else
                {
                    abAppend(ab, c, len);
                }
            }
        }
        abAppend(ab, "\x1b[K", 3);
        abAppend(ab, "\r\n", 2);
    }
    free(help);
}

void editorDrawStatusBar(struct abuf *ab)
{
    abAppend(ab, "\x1b[7m", 4);
    char status[80], rstatus[80];
    int len = snprintf(status, sizeof(status), "%.20s - %d baris %s",
                       E.filename ? E.filename : "[File Kosong]", E.numrows,
                       E.dirty ? "(modified)" : "");
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", C.y + 1, E.numrows);
    if (len > E.screencols)
        len = E.screencols;
    abAppend(ab, status, len);
    while (len < E.screencols)
    {
        if (E.screencols - len == rlen)
        {
            abAppend(ab, rstatus, rlen);
            break;
        }
        else
        {
            abAppend(ab, " ", 1);
            len++;
        }
    }
    abAppend(ab, "\x1b[m", 3);
    abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct abuf *ab)
{
    abAppend(ab, "\x1b[K", 3);
    int msglen = strlen(E.statusmsg);
    if (msglen > E.screencols)
        msglen = E.screencols;
    if (msglen && time(NULL) - E.statusmsg_time < 5)
        abAppend(ab, E.statusmsg, msglen);
}

void editorRefreshScreen()
{
    editorScroll();

    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);

    editorDrawRows(&ab);
    editorDrawStatusBar(&ab);
    editorDrawMessageBar(&ab);

    char buf[32];
    int y = isInStatus ? stat_cursor.y + 1 : (C.y - E.rowoff) + 1;
    int x = isInStatus ? stat_cursor.x + 1 : (C.rx - E.coloff) + 1;
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);

    abAppend(&ab, buf, strlen(buf));
    abAppend(&ab, "\x1b[?25h", 6);
    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
    va_end(ap);
    E.statusmsg_time = time(NULL);
}

char *editorRowsToString(int *buflen)
{
    int totlen = 0;
    int j;
    for (j = 0; j < E.numrows; j++)
        totlen += E.row[j].size + 1;
    *buflen = totlen;

    char *buf = malloc(totlen);
    char *p = buf;
    for (j = 0; j < E.numrows; j++)
    {
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

char *editorPrompt(char *prompt, int start_cx)
{
    // Deklarasi variabel penampung nama file, dengan size 128 bytes
    size_t name_size = 128;
    char *filename = malloc(name_size);

    // Inisialisasi awal size isi, dan zero character
    size_t name_len = 0;
    filename[0] = '\0';

    // Pindah cursor kebawah
    isInStatus = true;
    stat_cursor.y = E.screenrows + 2;
    stat_cursor.x = start_cx;

    while (1)
    {
        // Membuka status bar, untuk menerima input ke filename
        editorSetStatusMessage(prompt, filename);
        editorRefreshScreen();

        // Membaca input untuk mengisi file name
        int c = editorReadKey();

        // Mini delete character handler
        if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE)
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
            editorSetStatusMessage("");
            free(filename);
            isInStatus = false;
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
                    editorSetStatusMessage("");
                    isInStatus = false;
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
                    filename = realloc(filename, name_size);
                }
                // Mengisi filename
                filename[name_len++] = c;
                filename[name_len] = '\0';
                stat_cursor.x++;
            }
        }
    }
}