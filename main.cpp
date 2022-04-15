#include <stdlib.h>
#include "input.h"

int main () {
	rawModeOn();
	while(1){
		keyProcess();
	}
	return 0;
}
