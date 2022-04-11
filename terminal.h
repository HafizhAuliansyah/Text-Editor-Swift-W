#ifndef prototype_h
#define prototype_h
#include <windows.h>


void rawModeOn();
DWORD readKey();
void keyProcess();

// Accessor Configuration Console
HANDLE getConsoleOut();
HANDLE getConsoleIn();

#endif
