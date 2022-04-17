#ifndef output_h
#define output_h
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "cursor.h"
#include "file_io.h"
#include "input.h"

struct outputHandler
{
    char statusmsg[100];
    time_t statusmsg_time;
    bool isInstatus;
    bool isInHelp; 
};



void drawRows(struct abuf *ab);

void addStatusBar(struct abuf *ab);

void addMessageBar(struct abuf *ab);

void refreshScreen();

void setMessage(const char *fmt, ...);

char *rowsToString(int *buflen);

char *setInputMassage(char *prompt, int start_cx);



#endif