#include "helpers/helpers.h"
#include <stdio.h>
int main() {
	printf("Current time: %d hours %d minutes %d seconds", currentHour(), currentMinute(), currentSecond());
	return 0;
}
