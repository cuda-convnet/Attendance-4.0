#include "vs-intellisense-fix.hpp"

#include "Clock.h"
#include "LCD.h"
#include "State.h"
#include "ANSI.h"

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
			if(State::state != State::READY) {
				cv.wait_for(lk, std::chrono::seconds(1));
				continue;
			}
			//Get formatted date
			date = getDate();
			//Write the message
			LCD::writeMessage(date,1,0);
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
		std::strftime(buf, sizeof(buf), "%m/%d      %I:%M", &tstruct);
		return buf;
	}
}
