#include "file_io.h"
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
using namespace std;

fileHandler fileStatus;

void fileInit(){
    fileStatus.filename = NULL;
    fileStatus.modified = 0;
}
void openFile(char *filename)
{
    // HAFIZH : Mengisi variabel file name
    free(fileStatus.filename);
    fileStatus.filename = strdup(filename);

    // HAFIZH : Pembuka File
    ifstream file_teks;
    file_teks.open(filename, ios::in);

    // HAFIZH : Error handling file tidak ada
    if(file_teks.fail()){
        die("ERROR Open File : ");
        return;
    }

    // HAFIZH : Proses pembacaan isi file
    string line;
    size_t linecap = 0;
    int linelen;
    while (getline(file_teks, line))
    {
        int linelen = line.length();
        while (linelen > 0 && (line[linelen - 1] == '\n') || line[linelen - 1] == '\r')
            linelen--;
        insertRow(getTeksEditor().numrows, line.c_str(), linelen);
    }
    file_teks.close();
    fileStatus.modified = 0;
}
void openNewFile(teksEditor *tEditor){
    // HAFIZH : Konfirmasi save file sebelum buka file lain
    if(fileStatus.modified != 0){
        char simpan;
        while(simpan != 'Y' && simpan != 'N' && simpan != '\x1b'){
            char *s = setInputMassage("Simpan Perubahan File ? [Y/N] : %s (ESC untuk keluar)", 32);
            if(s != NULL && strlen(s) == 1){
                simpan = toupper(s[0]);
            }
        }
        if(simpan == 'Y'){
            saveFile();
        }else if(simpan == '\x1b'){
            return;
        }
    }

    char *filename;
    filename = setInputMassage("Open File : %s (ESC untuk keluar)", 12);
    if(filename == NULL){
        setMessage("NAMA FILE KOSOSNG!");
        return;
    }

    // HAFIZH : Pembuka File
    ifstream file_teks;
    file_teks.open(filename, ios::in);

    // HAFIZH : Error handling file tidak ada
    if(file_teks.fail()){
        char buat;
        while(buat != 'Y' && buat != 'N' && buat != '\x1b'){
            char *s = setInputMassage("File Tidak Ditemukan, Buat File ? [Y/N] : %s (ESC untuk keluar)", 42);
            if(s != NULL && strlen(s) == 1){
                buat = toupper(s[0]);
            }
        }
        if(buat == 'N'){
            setMessage("File Gagal Dibuka");
            return;
        }else if(buat == 'Y'){
            ofstream file_new{filename};
            if(!file_new.fail())
                setMessage("File Berhasil Dibuat");
            else
                setMessage("File Gagal Dibuat");
            file_new.close();
        }else{
            return;
        }
        
    }
    
    // HAFIZH : Clear isi teks_editor
    erow df_row;
    fill_n(tEditor->row, MAX_COLUMN + 1, df_row);
    tEditor->numrows = 0;
    // HAFIZH : Proses pembacaan isi file
    string line;
    size_t linecap = 0;
    int linelen;
    while (getline(file_teks, line))
    {
        int linelen = line.length();
        while (linelen > 0 && (line[linelen - 1] == '\n') || line[linelen - 1] == '\r')
            linelen--;
        insertRow(getTeksEditor().numrows, line.c_str(), linelen);
    }

    file_teks.close();
    fileStatus.modified = 0;
    free(fileStatus.filename);
    fileStatus.filename = filename;
    cursorInit();

}
void saveFile()
{
    // Jika argumen filename kosong
    if (fileStatus.filename == NULL)
    {
        // Memasukkan nama file penyimpanan
        fileStatus.filename = setInputMassage("Nama File Penyimpanan : %s  (ESC untuk keluar)", 24);
        if (fileStatus.filename == NULL)
        {
            setMessage("GAGAL MENYIMPAN");
            return;
        }
    }
    int len;
    char *buffer = rowsToString(&len);
    HANDLE handleFile;
    handleFile = CreateFile(fileStatus.filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD errorID = GetLastError();
    // Jika file yang dicari tersedia
    if (errorID == ERROR_ALREADY_EXISTS || errorID == 0)
    {   
        
        if (WriteFile(handleFile, buffer, len, NULL, NULL))
        {
            CloseHandle(handleFile);
            free(buffer);
            fileStatus.modified = 0;
            setMessage("%d bytes disimpan", len);
            return;
        }
        CloseHandle(handleFile);
    }
    free(buffer);
    setMessage("Error menyimpan : %s", strerror(errno));
}
char **openHelp(int *help_len)
{
    ifstream help_file;
    help_file.open("help.txt");

    char **isi_file = NULL;
    size_t linecap = 0;
    int linelen;
    int x = 0;
    string line;
    while (getline(help_file, line))
    {
        linelen = line.length();
        while (linelen > 0 && (line[linelen - 1] == '\n') || line[linelen - 1] == '\r')
            linelen--;

        isi_file = (char**) realloc(isi_file, sizeof(char **) * (x + 1));
        isi_file[x] = (char*) malloc(linelen + 1);
        memcpy(isi_file[x], line.c_str(), linelen);
        isi_file[x][linelen] = '\0';
        x++;
    }
    help_file.close();
    *help_len = x;
    return isi_file;
}
fileHandler getFileHandler(){
    return fileStatus;
}
void setFilename(char *name){
    fileStatus.filename = name;
}
void addModified(){
    fileStatus.modified++;
}