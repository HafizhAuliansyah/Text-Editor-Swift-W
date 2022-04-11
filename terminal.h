#ifndef terminal_H
#define terminal_H
#include <windows.h>
#include <stdio.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004 //define secara manual karena versi SDK yang lama
#endif
void rawModeOn();
DWORD readKey();
void keyProcess();

#endif
