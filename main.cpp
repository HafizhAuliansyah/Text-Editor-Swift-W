#include <stdio.h>
#include <stdlib.h>
#include "prototype.h"

int main () {
	rawModeOn();
	while(1){
		keyProcess();
	}

	return 0;
}
