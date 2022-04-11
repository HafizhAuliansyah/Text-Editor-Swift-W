#ifndef terminal_H
#define terminal_H
#include <windows.h>
#include <stdio.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING //define secara manual karena versi SDK yang lama
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004 //agar lebih banyak escape sequence lebih banyak
#endif
#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif

void rawModeOn();
DWORD readKey();
void keyProcess();

#endif
