#include "Keypad.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>

/*!	@section mod_init	Module Initialization
 *
 * 	The initialization process for the keypad attempts to iterate through the
 * 	keymap specified by the @p keymap variable, and for each of the given pins,
 * 	change it to input mode, and then activate its internal pulldown resistor.
 * 	As the bcm2835 library provides no error handling for this operation, there
 * 	is no way to tell if the operation was successful.
 *
 * 	@image html keypad_flow.png
 *
 * 	@section poll_thread	Polling Thread
 *
 * 	The polling thread continuously monitors the 12 GPIO pins assigned to keys
 * 	on the keypad for a positive change, and if detected, trigger the
 * 	appropriate event handler method.
 *
 * 	@image html keypad_threadflow.png
 *
 */
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

	/*!	Keypad Initialization Method.
	 *
	 * 	This method initializes the keypad by first configuring the GPIO pin
	 * 	associated with each individual key, and then spawning the pin polling
	 * 	thread.
	 *
	 * 	@todo Write the GPIO monitor thread
	 *
	 */
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

		//Start the GPIO monitor thread


		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	/*!	Keypad Destruction Method.
	 *
	 * 	This method doesn't actually do anything at the moment, but is included
	 * 	for consistency and in case of a future update.
	 *
	 * 	@todo Destroy the GPIO monitor thread
	 *
	 */
	void destroy() {
		//Destroy the keypad
		printf("[" WHITE "----" RESET "] Destroy Keypad...");

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

}
