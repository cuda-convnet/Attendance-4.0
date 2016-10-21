#include "vs-intellisense-fix.hpp"

#include "Clock.h"
#include "LCD.h"
#include "State.h"
#include "ANSI.h"
#include "Utils.h"

#include <stdio.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <unistd.h>
#include <string>
#include <time.h>

namespace Clock {

	//Private declarations
	void clockThread();
	std::string getDate();

	///Clock thead
	std::thread cThread;
	std::mutex m;
	///Clock thread termination condition
	bool run = true;
	std::condition_variable cv;

	void init() {
		//Initialize the clock
		printf(LOADING "Initializing Clock...");
		fflush(stdout);

		//Start clock thread
		cThread = std::thread(clockThread);

		//Success
		printf(OKAY "\n");
	}

	void destroy() {
		//Destroy the clock
		printf(LOADING "Destroy Clock...");
		fflush(stdout);

		//Instruct the thread to terminate
		run = false;
		cv.notify_one();
		//Join the thread
		cThread.join();

		//Success
		printf(OKAY "\n");
	}

	void clockThread() {
		//Current time variable
		std::string date;

		std::unique_lock<std::mutex> lk(m);

		//Loop
		while(run) {
			//Check if allowed state
			if(State::state != State::READY && State::state != State::NO_INTERNET) {
				cv.wait_for(lk, std::chrono::seconds(1));
				continue;
			}
			//Get formatted date
			date = getDate();

			//Write the message
			std::string str;
			str += date;
			str += "  ";
			if (State::haveEthernet && State::haveWifi) {
				str += CHAR_ETH;
				str += CHAR_WIFI;
			} else if (State::haveEthernet) {
				str += ' ';
				str += CHAR_ETH;
			} else if (State::haveWifi) {
				str += ' ';
				str += CHAR_WIFI;
			} else {
				str += ' ';
				str += CHAR_NO_NET;
			}
			
			LCD::writeMessage(str, 1, 0);
			//Delay
			cv.wait_for(lk, std::chrono::seconds(10));
		}
		lk.release();
	}

	std::string getDate() {
		//Time object
		std::time_t now = std::time(0);
		struct std::tm tstruct;
		char buf[17];
		//Calculate time
		tstruct = *std::localtime(&now);
		//Convert to string
		std::strftime(buf, sizeof(buf), "%m/%d  %I:%M", &tstruct);
		return buf;
	}

	void wakeup() {
		cv.notify_one();
	}
}
