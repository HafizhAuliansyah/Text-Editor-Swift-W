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
    selectInit();

    if (argc >= 2)
    {
        openFile(argv[1]);
    }

    while (1)
    {
        if(getOutputHandler().isInMenu)
            MenuMode();
        else
            refreshScreen();
        keyProcess();
    }
    return 0;
}
