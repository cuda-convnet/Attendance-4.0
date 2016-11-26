#include "vs-intellisense-fix.hpp"

#include "Main.h"
#include "LCD.h"
#include "Buzzer.h"
#include "Keypad.h"
#include "RFID.h"
#include "Clock.h"
#include "UserHandler.h"
#include "Utils.h"

#include "State.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <signal.h>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
bool running = true;

void catch_signal(int signum) {
	// notify main thread to die
	running = false;
	cv.notify_one();
}

int main() {
	// add signal handlers
	struct sigaction action1 = {};
	action1.sa_handler = catch_signal;
	sigaction(SIGTERM, &action1, NULL);
	sigaction(SIGINT, &action1, NULL);
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
		printf("\n\n");
		printf(FAIL "%s\n" RESET, e.what());
		State::changeState(State::ERROR);
		exit(0);
	}

	fflush(stdout);
	fflush(stderr);

	State::changeState(State::NO_INTERNET);
	std::unique_lock<std::mutex> lk(m);

	while (true) {
		std::vector<Utils::ConnectionState> cs = Utils::getConnectionState();
		if (cs.size() == 0) {
			State::haveWifi = false;
			State::haveEthernet = false;
			State::changeState(State::NO_INTERNET);
			Clock::wakeup();
		} else {
			bool wifi = false, ethernet = false;
			for (int i = 0; i < cs.size(); i++) {
				if (cs[i].connectionType == Utils::WIFI) {
					wifi = true;
				} else if (cs[i].connectionType == Utils::ETHERNET) {
					ethernet = true;
				}
			}
			State::haveWifi = wifi;
			State::haveEthernet = ethernet;

			if (State::state == State::NO_INTERNET) {
				if (Utils::hasInternetConnectivity()) {
					LCD::writeMessage("Loading users...", 0, 0);
					if (UserHandler::update()) {
						State::changeState(State::READY);
						Clock::wakeup();
					} else {
						LCD::writeMessage("Connecting...   ", 0, 0);
					}
				} else {
					LCD::writeMessage("Connecting...   ", 0, 0);
				}
			}
		}

		// wait 1 sec until we poll again, or until interrupted by a signal
		cv.wait_for(lk, std::chrono::seconds(1));
		if (!running) {
			// interrupted, time to clean up and exit
			break;
		}
	}

	lk.release();

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
		if (getenv("API_ASSIGN") == nullptr) {
			throw std::runtime_error(
				"Environment variable \"API_ASSIGN\" is not set");
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
