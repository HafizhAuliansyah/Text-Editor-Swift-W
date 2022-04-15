#ifndef terminal_H
#define terminal_H
#include <windows.h>
#include <stdio.h>
#include <ctype.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING //define secara manual karena versi SDK yang lama
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004 //agar lebih banyak escape sequence lebih banyak
#endif
#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif
#ifndef ENABLE_VIRTUAL_TERMINAL_INPUT
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0200
#endif


void rawModeOn();


// Accessor Configuration Console
HANDLE getConsoleOut();
HANDLE getConsoleIn();

#endif
