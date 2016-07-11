#include "Main.h"
#include "LCD.h"
#include "Keypad.h"
#include "RFID.h"
#include "Clock.h"

#include "HttpSend.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>
#include <unistd.h>

int main() {
	//Initialize components
	printf(INFO "Initializing components...\n");
	try {
		//Initialize things
		Main::init();
		LCD::init();
		Keypad::init();
		RFID::init();
		Clock::init();
	} catch(const std::runtime_error& e) {
		//Catch the error
		printf("[" RED "ERR " RESET "] %s\n" RESET, e.what());
	}

	usleep(600000000);


	//Clean up time
	printf(INFO "Destroying components...\n");
	Clock::destroy();
	RFID::destroy();
	Keypad::destroy();
	LCD::destroy();
	Main::destroy();
}

namespace Main {

	void init() {
		//Initialize the main things
		printf(LOADING "Initializing Main...");
		fflush(stdout);

		//Initialize the bcm2835 library
		if(!bcm2835_init()) {
			//Failed to initialize library
			printf("\r[" RED "FAIL\n" RESET);
			throw std::runtime_error("Failed to initialize bcm2835 library");
		}

		//Success
		printf(OKAY "\n");
	}

	void destroy() {
		//Destroy the main things
		printf(LOADING "Destroying Main...");
		fflush(stdout);

		//Un-initialize the bcm2835 library
		if(!bcm2835_close()) {
			//Failed to de-initialize library
			printf("\r[" YELLOW "WARN\n" RESET);
			printf("  Failed to destroy bcm2835 library");
			return;
		}

		//Success
		printf(OKAY "\n");
	}
}

/*!	@mainpage Ligerbots Attendance System Documentation
 *
 * 	Ooh, look at this nifty chart!
 *
 * 	@image html main_flow.png
 *
 * 	Additional documentation for each of the primary modules is available under
 * 	their dedicated page. To see this documentation, click the "Namespaces"
 * 	tab at the top of the screen.
 *
 */
