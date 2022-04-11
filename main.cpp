#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"

int main () {
	rawModeOn();
	while(1){
		keyProcess();
	}

	return 0;
}
