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

    setMessage("-ctrl+c = Copy  -ctrl+x = Cut  -ctrl+v = Paste -ctrl+h = Help  -ctrl+f = Find -ctrl+s = Save ");

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
