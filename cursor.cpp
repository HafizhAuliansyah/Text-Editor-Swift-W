#include "cursor.h"

CONSOLE_SCREEN_BUFFER_INFO infoScreenBuffer;
cursorHandler cursor, ms_cursor;

void getCursorPosition(int *row, int *column)
{
    // Mengambil konfigurasi console out
    HANDLE console_out = getConsoleOut();
    // Mengambil konfigurasi screen buffer info
    if (!GetConsoleScreenBufferInfo(console_out, &infoScreenBuffer))
    {
        MessageBox(NULL, TEXT("Error Pengambilan Info Screen Buffer"), TEXT("ERROR"), MB_OK);
    }
    // Memasukkan koordinat cursor ke struct cursor handler
    cursor.x = infoScreenBuffer.dwCursorPosition.X - 1;
    cursor.y = infoScreenBuffer.dwCursorPosition.Y - 1;
    // Mengambalikan nilai
    *row = cursor.y;
    *column = cursor.x;
}

void cursorInit()
{
    cursor.x = 0;
    cursor.y = 0;
    cursor.rx = 0;
    cursor.start_col = 0;
    cursor.start_row = 0;
}

void moveCursor(int button, teksEditor tEditor)
{
    // Check jika baris dimana cursor berada tidak kosong
    erow *row = (cursor.y >= tEditor.numrows) ? NULL : &searchByIndex(tEditor.first_row, cursor.y)->info;
    // Arrow Handler
    switch (button)
    {
    case ARROW_LEFT:
        if (cursor.x != 0)
        {
            // Memindahkan Cursor ke kiri 1
            cursor.x--;
        }
        else if (cursor.y > 0 && cursor.x == 0)
        {
            // Cursor ke baris atas
            cursor.y--;
            cursor.x = searchByIndex(tEditor.first_row, cursor.y)->info.size;
        }
        break;
    case ARROW_RIGHT:
        if (row && cursor.x < row->size)
        {
            cursor.x++;
        }
        else if (row && cursor.x == row->size && cursor.y != tEditor.numrows - 1)
        {
            // Cursor ke baris bawah
            cursor.y++;
            cursor.x = 0;
        }
        break;
    case ARROW_UP:
        if (cursor.y != 0)
        {
            // Cursor pindah ke baris atas
            cursor.y--;
        }
        break;
    case ARROW_DOWN:
        if (cursor.y < tEditor.numrows - 1)
        {
            // Cursor pindah ke baris bawah
            cursor.y++;
        }
        break;
    }

    // Error Handling, cursor melebihi jumlah kolom
    row = (cursor.y >= tEditor.numrows) ? NULL : &searchByIndex(tEditor.first_row, cursor.y)->info;
    int len = row ? row->size : 0;
    if (cursor.x > len)
    {
        cursor.x = len;
    }
}
int cursorXToRenderX(erow *row, int x)
{
    int rx = 0;
    int j;
    for (j = 0; j < x; j++)
    {
        if (SearchCharByIndex(row->chars, j)->info == '\t')
            rx += (SWIFT_TAB_STOP - 1) - (rx % SWIFT_TAB_STOP);
        rx++;
    }
    return rx;
}
int renderXToCursorX(erow *row, int rx)
{
    int cur_rx = 0;
    int x;
    for (x = 0; x < row->size; x++)
    {
        if (Info(SearchCharByIndex(row->chars, x)) == '\t')
            cur_rx += (SWIFT_TAB_STOP) - (cur_rx % SWIFT_TAB_STOP);
        cur_rx++;

        if (cur_rx > rx)
            return x;
    }
    return x;
}
void cursorScroll(teksEditor tEditor)
{
    int screenrows = getScreenRows(); // get max screen row
    int screencols = getScrenCols();  // get max screen column
    // Tab Detector and Handler
    cursor.rx = 0;
    if (cursor.y < tEditor.numrows)
    {
        cursor.rx = cursorXToRenderX(&searchByIndex(tEditor.first_row, cursor.y)->info, cursor.x);
    }
    // Pengaturan row offset ketika scroll keatas
    if (cursor.y < cursor.start_row)
    {
        cursor.start_row = cursor.y;
    }
    // Pengaturan row offset ketika scroll kebawah
    if (cursor.y >= cursor.start_row + screenrows)
    {
        cursor.start_col = cursor.y - screenrows + 1;
    }
    // Pengaturan coll offset ketika scroll ke kiri
    if (cursor.rx < cursor.start_col)
    {
        cursor.start_col = cursor.rx;
    }
    // Pengaturan coll offset ketika scroll ke kanan
    if (cursor.rx >= cursor.start_col + screencols)
    {
        cursor.start_col = cursor.rx - screencols + 1;
    }
}
cursorHandler getCursor()
{
    return cursor;
}
void setCursor(cursorHandler c)
{
    cursor = c;
}
void setCursorX(int x)
{
    cursor.x = x;
}
void addCursorX()
{
    cursor.x++;
}
void addCursorY()
{
    cursor.y++;
}
void setCursorY(int y)
{
    cursor.y = y;
}
int getStartRow()
{
    return cursor.start_row;
}
int getStartCol()
{
    return cursor.start_col;
}
void setStartRow(int sr)
{
    cursor.start_row = sr;
}
void setStartCol(int sc)
{
    cursor.start_col = sc;
}
void addStartRow()
{
    cursor.start_row++;
}
void addStartCol()
{
    cursor.start_col++;
}
cursorHandler getMessageCursor()
{
    return ms_cursor;
}
void setMessageCursor(cursorHandler new_ms)
{
    ms_cursor = new_ms;
}
