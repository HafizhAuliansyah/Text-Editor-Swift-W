#ifndef fileIO_h
#define fileIO_h

struct fileHandler{
    char *filename;
    int modified;
};

/* Constructor for fileHandler */
void fileInit();

/* Open File */
void openFile(char *filename);

/* Save file */
void saveFile();

/* Opean Help File */
char **openHelp(int *help_len);

#endif