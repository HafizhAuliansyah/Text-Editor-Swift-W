#ifndef fileIO_h
#define fileIO_h

#include "terminal.h"
#include "input.h"
#include <string.h>
#include <sys/types.h>
#include "atlstr.h"
#include <stdio.h>
#include <stdlib.h>

struct fileHandler
{
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

/* Selector */
fileHandler getFileHandler();

/* Mutator */
void setFilename();
void addModified();

#endif