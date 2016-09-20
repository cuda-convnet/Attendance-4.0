#include "Main.h"
#include "LCD.h"
#include "Buzzer.h"
#include "Keypad.h"
#include "RFID.h"
#include "Clock.h"
#include "UserHandler.h"

#include "State.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <streambuf>

int main() {
	//Change state
	State::changeState(State::INIT);
	//Initialize components
	printf(INFO "Initializing components...\n");
	try {
		//Initialize things
		Main::init();
		LCD::init();
		Buzzer::init();
		Keypad::init();
		RFID::init();
		Clock::init();
		UserHandler::init();
	} catch(const std::exception& e) {
		//Catch the error
		printf(FAIL "%s\n" RESET, e.what());
		State::changeState(State::ERROR);
		exit(0);
	}

	//Read
	State::changeState(State::READY);
	printf(INFO "Ready!\n");

	usleep(600000000);


	//Clean up time
	State::changeState(State::STOPPING);
	printf(INFO "Destroying components...\n");
	UserHandler::destroy();
	Clock::destroy();
	RFID::destroy();
	Keypad::destroy();
	Buzzer::destroy();
	LCD::destroy();
	Main::destroy();

	//Change state for the last time
	State::changeState(State::STOPPED);
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

		//Create the configuration file input stream
		//std::ifstream configFile("config.json");
		//Parse the stream
		//config << configFile;

		//Check for expected configuration values
		if(getenv("HTTP_USERNAME") == nullptr) {
			throw std::runtime_error(
					"Environment variable \"HTTP_USERNAME\" is not set");
		}
		if(getenv("HTTP_PASSWORD") == nullptr) {
			throw std::runtime_error(
					"Environment variable \"HTTP_PASSWORD\" is not set");
		}
		if(getenv("API_BASEURL") == nullptr) {
			throw std::runtime_error(
					"Environment variable \"API_BASEURL\" is not set");
		}
		if(getenv("API_LISTUSERS") == nullptr) {
			throw std::runtime_error(
					"Environment variable \"API_LISTUSERS\" is not set");
		}
		if(getenv("API_TRIGGER") == nullptr) {
			throw std::runtime_error(
					"Environment variable \"API_TRIGGER\" is not set");
		}

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
