#include "file_io.h"
#include <stdio.h>
#include <iostream>

fileHandler fileStatus;

void fileInit(){
    fileStatus.filename = NULL;
    fileStatus.modified = 0;
}
void openFile(char *filename)
{
    free(fileStatus.filename);
    fileStatus.filename = strdup(filename);

    FILE *fp = fopen(filename, "r");
    if (!fp)
        die("fopen");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) != -1)
    {
        while (linelen > 0 && (line[linelen - 1] == '\n') || line[linelen - 1] == '\r')
            linelen--;
        insertRow(getTeksEditor().numrows, line, linelen);
    }
    free(line);
    fclose(fp);
    fileStatus.modified = 0;
}
void editorSave()
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
    TCHAR *fName;
    _tcscpy(fName, A2T(fileStatus.filename));
    handleFile = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD errorID = GetLastError();
    // Jika file yang dicari tersedia
    if (errorID == ERROR_ALREADY_EXISTS || errorID == 0)
    {   
        WriteFile(handleFile, buffer, len, NULL, NULL);
        errorID = GetLastError();
        if (errorID != 0)
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
    FILE *fp = fopen("help.txt", "r");
    if (!fp)
        die("fopen");

    char **isi_file = NULL;
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    int x = 0;
    while ((linelen = getline(&line, &linecap, fp)) != -1)
    {
        while (linelen > 0 && (line[linelen - 1] == '\n') || line[linelen - 1] == '\r')
            linelen--;

        isi_file = (char**) realloc(isi_file, sizeof(char **) * (x + 1));
        isi_file[x] = (char*) malloc(linelen + 1);
        memcpy(isi_file[x], line, linelen);
        isi_file[x][linelen] = '\0';
        x++;
    }
    free(line);
    fclose(fp);
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