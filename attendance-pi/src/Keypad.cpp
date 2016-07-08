#include "Keypad.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>

#define KP_0	RPI_GPIO_P1_15
#define KP_1	RPI_GPIO_PIN_1
#define KP_2	RPI_GPIO_PIN_2
#define KP_3	RPI_GPIO_PIN_3
#define KP_4	RPI_GPIO_PIN_4
#define KP_5	RPI_GPIO_PIN_5
#define KP_6	RPI_GPIO_PIN_6
#define KP_7	RPI_GPIO_PIN_7
#define KP_8	RPI_GPIO_PIN_8
#define KP_9	RPI_GPIO_PIN_9
#define KP_E	RPI_GPIO_PIN_10
#define KP_C	RPI_GPIO_PIN_11

namespace Keypad {

	//Key mapping
	int map[] {
			RPI_GPIO_P1_15
	};

	void init() {
		//Initialize the keypad
		printf("[" WHITE "----" RESET "] Initializing Keypad...");

		//Set mode for each key
		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void destroy() {
		//Destroy the keypad
		printf("[" WHITE "----" RESET "] Destroy Keypad...");

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

}
