#include <stdlib.h>
#include "input.h"
#include "output.h"

int main(int argc, char *argv[])
{
    rawModeOn();
	terminalInit();
    inputInit();
	outputInit();
	cursorInit();
	fileInit();
	
    if (argc >= 2)
    {
        openFile(argv[1]);
    }

    setMessage("SHORTCUT: Ctrl - Q = quit; Ctrl - S  = save, Search : CTRL - q");

    while (1)
    {
        refreshScreen();
        keyProcess();
    }
    return 0;
}