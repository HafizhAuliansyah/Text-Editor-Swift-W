#include "output.h"

outputHandler outputConfig;
std::string menuList[2] = {"File", "Help"};
std::string subMenuList[2][5] = {{"Open, Save"}, {"Help"}};
void drawRows(outputBuffer *ob)
{
    int y;
    char **help = NULL;
    int help_len;
    if (outputConfig.isInHelp)
    {
        bufferAppend(ob, "\x1b[lkkji", 5);
        help = openHelp(&help_len);
    }
    selectionText scanSelected = getSelection();
    //Membuat Jumlah Baris Sesuai Dengan ScreenRows
    for (y = 0; y < getScreenRows(); y++)
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
            teksEditor tEditor = getTeksEditor();
            if (filerow >= tEditor.numrows)
            {
                if (tEditor.numrows == 0 && y == getScreenRows() / 2)
                {
                    //Menampilkan Welcome Messege Saat Pertama Kali Memubuka Aplikasi
                    char welcome[80];
                    int welcomelen = snprintf(welcome, sizeof(welcome), "Swift Text Editor");
                    if (welcomelen > getScrenCols())
                        welcomelen = getScrenCols();
                    int padding = (getScrenCols() - welcomelen) / 2;
                    //Menambah tanda "~" untuk tanda baris
                    if (padding)
                    {
                        if (y < getScreenRows())
                        {
                            bufferAppend(ob, "~", 1);
                        }
                        padding--;
                    }
                    //Bila Menampilkan Welcome Messege Maka Tanda "~" tidak di print
                    while (padding--)
                        bufferAppend(ob, " ", 1);
                    bufferAppend(ob, welcome, welcomelen);
                }
                else
                {
                    //Menambah tanda "~" untuk tanda baris
                    if (y < getScreenRows())
                    {
                        bufferAppend(ob, "~", 1);
                    }
                }
            }
            else
            {
                // LINE NUMBERING
                // std::stringstream line_number;
                // line_number << "[";
                // line_number << filerow;
                // line_number << "]";
                // const char* line_num_c = line_number.str().c_str();
                // bufferAppend(ob, line_num_c, strlen(line_num_c));
                int len = searchByIndex(tEditor.first_row, filerow)->info.rsize - getStartCol();
                if (len < 0)
                    len = 0;
                if (len > getScrenCols())
                    len = getScrenCols();

                // Konversi char ke char*
                infotype row = searchByIndex(tEditor.first_row, filerow)->info;
                // char *c = &searchByIndex(tEditor.first_row, filerow)->info.render[getStartCol()];
                address_column column = row.render;
                char *temp_char = (char *)malloc((row.rsize + 1) * sizeof(char));
                temp_char[row.rsize] = '\0';
                int i = 0;
                while (i < row.rsize)
                {
                    temp_char[i] = Info(column);
                    // setMessage("%c, rsize : %d, x: %d,y: %d", Info(column), row.rsize, getCursor().x, getCursor().y);
                    column = NextColumn(column);
                    i++;
                }
                char *c = &temp_char[getStartCol()];
                // Select Text
                if (filerow == scanSelected.y && getStartCol() <= getSelection().x && getSelection().isOn)
                {
                    addSelectionText(ob, c, len, &scanSelected);
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
    if (outputConfig.isInHelp)
        bufferAppend(ob, "\x1b[m", 3);
    free(help);
}
void drawMenuBar(outputBuffer *ob, int selected, bool isDropDown){

    bufferAppend(ob, "\x1b[K", 3);
    int lenMenu = sizeof(menuList)/sizeof(menuList[0]);
    int lenFilled = 0;
    int currentSelected = selected;
    for(int i = 0; i < lenMenu; i++){
        if(i == currentSelected - 1){
            bufferAppend(ob, "\x1b[44m", 5);

        }
        else{
            bufferAppend(ob, "\x1b[7m", 4);
        }

        bufferAppend(ob, " ", 1);
        bufferAppend(ob, menuList[i].c_str(), menuList[i].length());
        bufferAppend(ob, " ", 1);
        bufferAppend(ob, "\x1b[m", 3);
        lenFilled += (menuList[i].length() + 2 );

    }
    bufferAppend(ob, "\x1b[7m", 4);
    int x;
    for(x = lenFilled; x < getScrenCols(); x++){
        bufferAppend(ob, " ", 1);
    }
    bufferAppend(ob, "\x1b[m", 3);
    bufferAppend(ob, "\r\n", 2);
}
void MenuMode(){
    bool dropOn = false;
    int selectedMenu = 1;
    int lenMenu = sizeof(menuList)/sizeof(menuList[0]);

    while (1)
    {
        outputBuffer ob = OUTPUT_INIT;
        setMessage("\x1b[46m MENU MODE \x1b[m");
        //Memposisikan Cursor Di Sudut Kiri Atas
        bufferAppend(&ob, "\x1b[?25l", 6);
        bufferAppend(&ob, "\x1b[H", 3);
        drawMenuBar(&ob, selectedMenu, dropOn);
        //Menampilkan Setiap Baris Buffer Teks Yang Sedang Di Edit
        drawRows(&ob);
        //Menampilkan Status Bar
        addStatusBar(&ob);
        //Menampilkan Status Messege
        addMessageBar(&ob);
        WriteFile(getConsoleOut(), ob.buffer, ob.len, NULL, NULL);
        // Membaca input untuk mengisi file name
        int c = readKey();
        // Escape, untuk keluar dari menu mode

        switch(c){
            case '\x1b':
                outputConfig.isInMenu = false;
                break;
            case '\r':
                dropOn = true;
                return;
                break;
            case ARROW_LEFT:
                if(selectedMenu > 1)
                    selectedMenu -= 1;
                break;
            case ARROW_RIGHT:
                if(selectedMenu < lenMenu)
                    selectedMenu += 1;
                break;
        }

        bufferFree(&ob);
        if(outputConfig.isInMenu == false){
            setMessage("\x1b[101m EXIT MENU MODE \x1b[m");
            refreshScreen();
            return;
        }
    }
}
void addStatusBar(outputBuffer *ob)
{
    cursorHandler C = getCursor();
    teksEditor tEditor = getTeksEditor();
    //Mengubah Warna BG nya Menjadi Negative
    bufferAppend(ob, "\x1b[7m", 4);
    char status[80], rstatus[80];
    //Untuk Menampilkan Jumlah Baris
    int len = snprintf(status, sizeof(status), "%.20s - %d baris %s",
    //Untuk Menampilkan Nama File dan bila belum ada file nya akan muncul [File Kosong]
    getFileHandler().filename ? getFileHandler().filename : "[File Kosong]", tEditor.numrows,
    //Bila Si File Di Ubah Akan Menampilkan (Modified)
    getFileHandler().modified ? "(modified)" : "");
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", C.y + 1, tEditor.numrows);
    if (len > getScrenCols())
        len = getScrenCols();
    bufferAppend(ob, status, len);
    //Meminta Posisi Paling Bawah Pada Layar
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

    cursorScroll(getTeksEditor());

    outputBuffer ob = OUTPUT_INIT;
    //Memposisikan Cursor Di Sudut Kiri Atas
    bufferAppend(&ob, "\x1b[?25l", 6);
    bufferAppend(&ob, "\x1b[H", 3);

    drawMenuBar(&ob, -1, false);
    //Menampilkan Setiap Baris Buffer Teks Yang Sedang Di Edit
    drawRows(&ob);
    //Menampilkan Status Bar
    addStatusBar(&ob);
    //Menampilkan Status Messege
    addMessageBar(&ob);
    char buf[32];
    int y = outputConfig.isInStatus ? getMessageCursor().y + 1 : (getCursor().y - getStartRow()) + 2;
    int x = outputConfig.isInStatus ? getMessageCursor().x + 1 : (getCursor().rx - getStartCol()) + 1;
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);

    bufferAppend(&ob, buf, strlen(buf));
    if (!outputConfig.isInHelp)
        bufferAppend(&ob, "\x1b[?25h", 6);
    WriteFile(getConsoleOut(), ob.buffer, ob.len, NULL, NULL);
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
        totlen += searchByIndex(tEditor.first_row, j)->info.size + 1;
    *buflen = totlen;

    char *buf = (char *)malloc(totlen);
    char *p = buf;
    for (j = 0; j < tEditor.numrows; j++)
    {
        address_row row = searchByIndex(tEditor.first_row, j);
        for(int k=0; k < row->info.size; k++){
            address_column col = SearchCharByIndex(row->info.chars, k);
            char c = col->info;
            p[k] = c;
        }
        p += searchByIndex(tEditor.first_row, j)->info.size;
        *p = '\n';
        p++;
    }

    return buf;
}


char *setInputMassage(const char *prompt, int start_cx)
{
    // Deklarasi variabel penampung hasil inputan, dengan size 50 bytes
    size_t name_size = 50;
    char *inputMessage = (char*) malloc(name_size);

    // Inisialisasi awal size isi, dan zero character
    size_t name_len = 0;
    inputMessage[0] = '\0';

    // HAFIZH : Pindah cursor kebawah
    outputConfig.isInStatus = true;
    cursorHandler stat_cursor = getMessageCursor();
    stat_cursor.y = getScreenRows() + 2;
    stat_cursor.x = start_cx;
    setMessageCursor(stat_cursor);

    while (1)
    {
        // Menampilkan message ke message bar beserta hasil input message
        setMessage(prompt, inputMessage);
        refreshScreen();

        // Membaca input untuk mengisi file name
        int c = readKey();
        // Mini delete character handler
        if (c == DEL_KEY || c == CTRL('h') || c == BACKSPACE)
        {
            if (name_len != 0)
            {
                inputMessage[--name_len] = '\0';
                // HAFIZH : Memindahkan kursor ke kiri
                stat_cursor.x--;
            }
        }
        // Escape, untuk keluar dari status input message, kembali ke text editor
        else if (c == '\x1b')
        {
            setMessage("");
            free(inputMessage);
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
                    return inputMessage;
                }
            }
            // Jika input bukan control character, masukkan input ke filename
            else if (!iscntrl(c) && c < 128)
            {
                // Menambah size filename, ketika hampir penuh
                if (name_len == name_size - 1)
                {
                    name_size *= 2;
                    inputMessage = (char*) realloc(inputMessage, name_size);
                }
                // Mengisi inputMessage dengan karakter inputan
                inputMessage[name_len++] = c;
                inputMessage[name_len] = '\0';
                // HAFIZH : Memindahkan kursor ke kanan
                stat_cursor.x++;
            }
        }
        // HAFIZH : Update cursor position yang ada di message bar
        setMessageCursor(stat_cursor);
    }
}
outputHandler getOutputHandler()
{
    return outputConfig;
}
void setInStatus(bool new_status)
{
    outputConfig.isInStatus = new_status;
}
void setInHelp(bool new_help)
{
    outputConfig.isInHelp = new_help;
}
void setInMenu(bool new_menu){
    outputConfig.isInMenu = new_menu;
}
void outputInit()
{
    outputConfig.isInHelp = false;
    outputConfig.isInStatus = false;
    outputConfig.statusmsg[0] = '\0';
    outputConfig.statusmsg_time = 0;
    outputConfig.isInMenu = false;
}
