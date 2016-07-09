#include "Main.h"
#include "LCD.h"
#include "GPIO.h"
#include "Keypad.h"
#include "RFID.h"
#include "HttpSend.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>

int main() {
	try {
		//Initialize things
		Main::init();
		LCD::init();
		GPIO::init();
		Keypad::init();
		RFID::init();
	} catch(const std::runtime_error& e) {
		//Catch the error
		printf("[" RED "ERR " RESET "] %s\n" RESET, e.what());
	}
}

namespace Main {

	void init() {
		//Initialize the main things
		printf("[" WHITE "----" RESET "] Initializing Main...");

		//Initialize the bcm2835 library
		if(!bcm2835_init()) {
			//Failed to initialize library
			printf("\r[" RED "FAIL\n" RESET);
			throw std::runtime_error("Failed to initialize bcm2835 library");
		}

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void destroy() {
		//Destroy the main things
		printf("[" WHITE "----" RESET "] Destroying Main...");

		//Un-initialize the bcm2835 library
		if(!bcm2835_close()) {
			//Failed to de-initialize library
			printf("\r[" RED "FAIL\n" RESET);
			throw std::runtime_error("Failed to destroy bcm2835 library");
		}

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}
}
