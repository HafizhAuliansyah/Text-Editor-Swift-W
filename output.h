#ifndef output_h
#define input_h
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cursor.h"
#include "file_io.h"

struct outputHandler
{
    char statusmsg[100];
    time_t statusmsg_time;
    bool isInstatus;
    bool isInHelp; 
};



void editorDrawRows(struct abuf *ab);

void editorDrawStatusBar(struct abuf *ab);

void editorDrawMessageBar(struct abuf *ab);

void editorRefreshScreen();

void editorSetStatusMessage(const char *fmt, ...);

char *editorRowsToString(int *buflen);

char *editorPrompt(char *prompt, int start_cx);



#endif