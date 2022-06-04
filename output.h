#ifndef output_h
#define output_h
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sstream>

#include "cursor.h"
#include "file_io.h"
#include "input.h"
#include "buffer.h"

struct outputHandler
{
    char statusmsg[100];
    time_t statusmsg_time;
    bool isInStatus;
    bool isInHelp;
    bool isInMenu;
};



void drawRows(outputBuffer *ob);

void addStatusBar(outputBuffer *ob);

void addMessageBar(outputBuffer *ob);

void refreshScreen();

void setMessage(const char *fmt, ...);

char *rowsToString(int *buflen);

char *setInputMassage(const char *prompt, int start_cx);

/* SELECTOR */
outputHandler getOutputHandler();
/* MUTATOR */
void setInStatus(bool new_status);
void setInHelp(bool new_help);

/* Initialize Value */
void outputInit();

void drawMenuBar(outputBuffer *ob, int selected, bool isDropDown);
void MenuMode();
void setInMenu(bool new_menu);


#endif
