#include "Keypad.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>

namespace Keypad {

	//Key mapping
	int keymap[] {
			RPI_GPIO_P1_15,	//1
			RPI_GPIO_P1_15,	//2
			RPI_GPIO_P1_15,	//3
			RPI_GPIO_P1_15,	//4
			RPI_GPIO_P1_15,	//5
			RPI_GPIO_P1_15,	//6
			RPI_GPIO_P1_15,	//7
			RPI_GPIO_P1_15,	//8
			RPI_GPIO_P1_15,	//9
			RPI_GPIO_P1_15,	//0
			RPI_GPIO_P1_15,	//Enter
			RPI_GPIO_P1_15	//Clear
	};

	void init() {
		//Initialize the keypad
		printf("[" WHITE "----" RESET "] Initializing Keypad...");

		//Configure each key
		for(unsigned int i = 0; i < sizeof(keymap) / sizeof(int); i++) {
			//Set to input
			bcm2835_gpio_fsel(keymap[i], BCM2835_GPIO_FSEL_INPT);
			//Set to pull-down
			bcm2835_gpio_set_pud(keymap[i], BCM2835_GPIO_PUD_DOWN);
		}

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
