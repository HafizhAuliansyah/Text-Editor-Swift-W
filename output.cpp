#include "output.h"


outputHandler outputConfig;

//variabel menu
std::string menuList[2] = {"File", "Help"};
//variabel submenu
std::string subMenuList[2][5] = {{"New","Open", "Save", "Exit"}, {"Help"}};
int lenSubMenuList[2] = {4,1};

void drawRows(outputBuffer *ob)
{
    int y;
    // Array 2 dimensi penampung help
    char **help = NULL;
    // Panjang baris help
    int help_len;
    // Ketika isInHelp, ambil data dari help.txt
    if (outputConfig.isInHelp)
    {
        bufferAppend(ob, "\x1b[96m", 5);
        help = openHelp(&help_len);
    }
    selectionText scanSelected = getSelection();
    //Membuat Jumlah Baris Sesuai Dengan ScreenRows
    for (y = 0; y < getScreenRows(); y++)
    {
        int filerow = y + getCursor().start_row;
        // Ketika isInHelp, tampilkan baris perbaris isi data help
        if (outputConfig.isInHelp)
        {
            if (filerow < help_len)
            {
                // Mengisi buffer dengan baris isi help
                bufferAppend(ob, help[filerow], strlen(help[filerow]));
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

void drawMenuBar(outputBuffer *ob, int selectedMenu, bool isDropDown, int selectedDrop){

    //maksimal panjang submenu
    int maxLengthDrop = 8;
    //panjang menu list yang select
    int lenMenu = sizeof(menuList)/sizeof(menuList[0]);
    //panjang submenu list yang select
    int lenSubMenu = sizeof(subMenuList[selectedMenu - 1])/sizeof(subMenuList[selectedMenu - 1][0]);
    //mengitung kolom yang sudah terisi
    int lenFilled = 0, filledBeforeDrop = 0;
    int currentSelected = selectedMenu;

    //membersihkan baris
    bufferAppend(ob, "\x1b[K", 3);
    // Looping Menu Utama
    for(int i = 0; i < lenMenu; i++){
        if(i == currentSelected - 1){
            //mengubah background menu yang terpilih menjadi biru
            bufferAppend(ob, "\x1b[44m", 5);
        }
        else{
            //mengubah background menu yang tidak terpilih menjadi negative
            bufferAppend(ob, "\x1b[7m", 4);
        }
        //menambah spasi pada untuk jarak permenu nya
        bufferAppend(ob, " ", 1);
        //menampilkan list menu FILE
        bufferAppend(ob, menuList[i].c_str(), menuList[i].length());
        //menambah spasi pada untuk jarak permenu nya
        bufferAppend(ob, " ", 1);
        //mengembalikan background ke defult
        bufferAppend(ob, "\x1b[m", 3);

        lenFilled += (menuList[i].length() + 2 );
        if(i < currentSelected - 1)
            filledBeforeDrop += (menuList[i].length() + 2 );
    }
    //mengubah background menjadi negative
    bufferAppend(ob, "\x1b[7m", 4);
    int x;
    //menambah spasi sampai ke akhir dari kolom layar
    for(x = lenFilled; x < getScrenCols(); x++){
        bufferAppend(ob, " ", 1);
    }
    //mengembalikan background ke defult
    bufferAppend(ob, "\x1b[m", 3);
    //enter
    bufferAppend(ob, "\r\n", 2);

    // Looping Dropdown
    if(isDropDown){

        for(int i = 0; i < lenSubMenuList[selectedMenu - 1]; i++){

            int filledDrop = 0, filledBefore = 0;
            //mengubah posisi x menjadi
            int x = filledBeforeDrop + 1;
            int y = i + 2;
            char buf[32];
            //mengubah nilai buf menjadi posisi kursor
            snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);
            //mengubah kursor ke posisi yang sudah di atur
            bufferAppend(ob, buf, strlen(buf));


            if(i == selectedDrop - 1){
                //mengubah background menu yang terpilih menjadi biru
                bufferAppend(ob, "\x1b[44m", 5);
            }
            else{
                 //mengubah background menu yang terpilih menjadi negative
                bufferAppend(ob, "\x1b[7m", 4);
            }
            bufferAppend(ob, " ", 1);
            //menampilkan list menu HELP
            bufferAppend(ob, subMenuList[selectedMenu - 1][i].c_str(), subMenuList[selectedMenu - 1][i].length());

            //menghitung ukuran kolom yang terisi
            filledDrop = 1 + subMenuList[selectedMenu - 1][i].length();\
           //menambah spasi bila si submenu kurang dari maxLengthDrop
            while(filledDrop < maxLengthDrop){
                bufferAppend(ob, " ", 1);
                filledDrop++;
            }
            //mereset warna background
            bufferAppend(ob, "\x1b[m", 3);
            //enter
            bufferAppend(ob, "\r\n", 2);
        }
    }
}
void MenuMode(){
    //variabel aktif tidak nya menu
    bool dropOn = false;
    //varabel menu yg terpilih
    int selectedMenu = 1;
    //variabel submenu yg terpilih
    int selectedDrop = -1;
    //panjang menu list yang terpilih
    int lenMenu = sizeof(menuList)/sizeof(menuList[0]);
    //panjang subemenu yg terpilih
    int lenSubMenu;
    teksEditor tEditor=getTeksEditor();
    static int quit_times = SWIFT_QUIT_TIMES;
    while (1)
    {   

        lenSubMenu = lenSubMenuList[selectedMenu - 1];
        outputBuffer ob = OUTPUT_INIT;
        setMessage("\x1b[46m MENU MODE \x1b[m");

        //Memposisikan Cursor Di Sudut Kiri Atas
        bufferAppend(&ob, "\x1b[?25l", 6);
        bufferAppend(&ob, "\x1b[H", 3);
        drawMenuBar(&ob, selectedMenu, dropOn, selectedDrop);
        if(!dropOn){
            //Menampilkan Setiap Baris Buffer Teks Yang Sedang Di Edit
            drawRows(&ob);

            drawShortcutBarMenu(&ob);
            //Menampilkan Status Bar
            addStatusBar(&ob);
            //Menampilkan Status Messege
            addMessageBar(&ob);
        }
        WriteFile(getConsoleOut(), ob.buffer, ob.len, NULL, NULL);
        // Membaca input untuk mengisi file name
        int c = readKey();
        // Escape, untuk keluar dari menu mode

        switch(c){
             
            case '\x1b':
                if(!dropOn)
                    outputConfig.isInMenu = false;
                dropOn = false;
                break;
            case '\r':{
                if(!dropOn){
                    dropOn = true;
                    selectedDrop = 1;
                }else{
                     outputConfig.isInMenu = false;
                    // TODO case prosses
                    if (selectedMenu == 1){
                        switch (selectedDrop){
                        //saat submenu pertama terpilih, jalankan fungsi newFile
                        case 1 :{
                          newFile(&tEditor);
                        }
                        break;
                        //saat submenu kedua terpilih, jalankan fungsi openNewFile
                        case 2 :{
                          openNewFile(&tEditor);
                        }
                        break;
                        //saat submenu ketiga terpilih, jalankan fungsi saveFile
                        case 3 :{
                         saveFile();

                        }
                          break;
                        //saat submenu pertama terpilih, keluar
                        case 4 :{
                            HANDLE console_out = getConsoleOut();
                            if (getFileHandler().modified && quit_times > 0)
                            {
                                setMessage("\x1b[91mPERINGATAN !! FILE BELUM DISIMPAN TEKAN Ctrl + s UNTUK SIMPAN, Ctrl + q UNTUK KELUAR\x1b[m", quit_times);
                                quit_times--;
                                return;
                            }
                            WriteFile(console_out, "\x1b[2J", 4, NULL, NULL);
                            WriteFile(console_out, "\x1b[H", 3, NULL, NULL);
                            exit(0);
                            break;
                        }
                        }
                    }
                    else{
                        if (selectedDrop == 1){
                            setInHelp(true);
                        }
                    }
                }
            }
                break;
            //saat menekan arah kiri menu yang terpilih dikurang 1
            case ARROW_LEFT:
                if(selectedMenu > 1 && !dropOn)
                    selectedMenu -= 1;
                break;
            //saat menekan arah kanan menu yang terpilih ditambah 1
            case ARROW_RIGHT:
                if(selectedMenu < lenMenu && !dropOn)
                    selectedMenu += 1;
                break;
            //saat menekan arah atas submenu yang terpilih dikurang 1    
            case ARROW_UP:
                if(dropOn && selectedDrop > 1)
                    selectedDrop -= 1;
                break;
            //saat menekan arah atas submenu yang terpilih ditambah 1    
            case ARROW_DOWN:
                if(dropOn && selectedDrop < lenSubMenu)
                    selectedDrop += 1;
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

void drawShortcutBar(outputBuffer *ob){
    const char *short1="-CTRL+C = Copy    -CTRL+X = Cut   -CTRL+V = Paste     -CTRL+H = Help      -CTRL+F = Find";
    const char *short2="-CTRL+S = Save    -CTRL+N = New   -CTRL+O = Open      -CTRL+Q = Keluar    -ESC    = Menu";
    bufferAppend(ob, "\x1b[92m", 5);
    bufferAppend(ob, "\x1b[K", 3);
    bufferAppend(ob, short1,strlen(short1));
    bufferAppend(ob, "\r\n", 2);
    bufferAppend(ob, "\x1b[K", 3);
    bufferAppend(ob, short2,strlen(short2));
    bufferAppend(ob, "\r\n", 2);
    bufferAppend(ob, "\x1b[m", 3);
}

void drawShortcutBarMenu(outputBuffer *ob){
    char *short1="-CTRL+C = Copy    -CTRL+X = Cut   -CTRL+V = Paste     -CTRL+H = Help      -CTRL+F = Find";
    char *short2="-CTRL+S = Save    -CTRL+N = New   -CTRL+O = Open      -CTRL+Q = Keluar    -ESC    = Kembali ";
    bufferAppend(ob, "\x1b[92m", 5);
    bufferAppend(ob, "\x1b[K", 3);
    bufferAppend(ob, short1,strlen(short1));
    bufferAppend(ob, "\r\n", 2);
    bufferAppend(ob, "\x1b[K", 3);
    bufferAppend(ob, short2,strlen(short2));
    bufferAppend(ob, "\r\n", 2);
    bufferAppend(ob, "\x1b[m", 3);
}

void addStatusBar(outputBuffer *ob)
{
    cursorHandler C = getCursor();
    teksEditor tEditor = getTeksEditor();
    //Mengubah Warna BG nya Menjadi Negative
    bufferAppend(ob, "\x1b[44m", 5);
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

    drawMenuBar(&ob, -1, false, -1);
    //Menampilkan Setiap Baris Buffer Teks Yang Sedang Di Edit
    drawRows(&ob);
    if(outputConfig.isInHelp){
    drawShortcutBarMenu(&ob);
    }
    else{
        drawShortcutBar(&ob);
    }


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
    stat_cursor.y = getScreenRows() + 4;
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
