#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"
#include "cursor.h"

int main () {
	rawModeOn();

	while(1){
		keyProcess();
	}

	return 0;
}
