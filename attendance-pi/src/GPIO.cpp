#include "GPIO.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>

namespace GPIO {

	void init() {
		//Initialize the GPIO
		printf("[" WHITE "----" RESET "] Initializing GPIO...");

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void destroy() {
		//Destroy the GPIO
		printf("[" WHITE "----" RESET "] Destroying GPIO...");

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}
}
