#include "vs-intellisense-fix.hpp"

#include "ANSI.h"
#include "json.hpp"
#include "UserHandler.h"
#include "Utils.h"
#include "LCD.h"
#include "Buzzer.h"
#include "State.h"

#include <stdio.h>
#include <curl/curl.h>
#include <unordered_map>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <ctime>

/*!	@section mod_init	Module Initialization
 *
 * 	The initialization process for the User Handler attempts to update the user
 * 	information table, and that's just about it
 *
 * 	@section update_method	Update Method
 *
 * 	The update method is called periodically, and instructs the device to
 * 	request an updated user state table from the server which replaces the
 * 	one stored locally in memory.
 *
 * 	@section trigger_method	Trigger Method
 *
 * 	The trigger method is called whenever a user signs in by RFID or by
 * 	entering their PIN into the keypad.  This method displays a message
 * 	confirming the user has signed in or out, then sends a request to the
 * 	server informing it that the user has signed in.
 *
 */
namespace UserHandler {

	//CURL object
	CURL* curl;

	//User object
	class User {
		public:
			//Variables
			std::string fname;
			std::string pin;
			std::string rfid;
			bool signedin;
			//Constructor
			User(std::string, std::string, std::string, bool);
	};

	//Periodic updating
	std::thread updaterThread;
	std::mutex m;
	bool run = true;
	std::condition_variable cv;

	//Periodic update function
	//this should run every day after the backend resets signed in users
	void periodicUpdateThread() {
		std::unique_lock<std::mutex> lk(m);
		while (run) {
			// get current time
			time_t currentTs;
			time(&currentTs);
			tm* currentTime = localtime(&currentTs);
			// add one day to current time, then set it to 3am
			currentTime->tm_mday++;
			currentTime->tm_hour = 3;
			currentTime->tm_min = 0;
			currentTime->tm_sec = 0;
			time_t next = mktime(currentTime);
			// wait until it's 3am
			auto tp = std::chrono::system_clock::from_time_t(next);
			cv.wait_until(lk, tp);
			if (run) {
				printf(INFO "Updating local database...\n");
				State::changeState(State::BUSY);
				update();
				State::changeState(State::READY);
			} // else we were rudely awakened by shutdown
		}
	}

	/*! User object constructor
	 *
	 * 	This constructs a new user object with the provided user information.
	 */
	User::User(std::string fnamex, std::string pinx, std::string rfidx,
			bool signedinx) {
		//Save the values
		this->fname = std::string(fnamex);
		this->pin = std::string(pinx);
		this->rfid = std::string(rfidx);
		this->signedin = signedinx;
		//printf("%s\t%s\t%s\n",
		//		fnamex.c_str(), pinx.c_str(), rfidx.c_str());
		//printf("%s\t%s\t%s\n",
		//		this->fname.c_str(), this->pin.c_str(), this->rfid.c_str());
	}

	//Users
	std::vector<User> users;

	// I would remove this, but I know that as soon as I do the issue will
	// come back to seek revenge.
	//
	//TEST FUNCTION
	//void test() {
	//	printf("THESE ARE THE USERS:\n");
	//	for(int i = 0; i < users.size(); i++) {
	//		printf("%s\t%s\t%s\n",
	//				users[i].fname.c_str(),
	//				users[i].pin.c_str(),
	//				users[i].rfid.c_str());
	//	}
	//	printf("========================================\n");
	//}

	/*! User Handler initialization method
	 *
	 * 	This method initializes the user handler by first updating the user
	 * 	table and then scheduling subsequent updates to happen at a regular
	 * 	interval.
	 *
	 */
	void init() {
		//Initialize the user handler
		printf(LOADING "Initializing User Handler...");
		fflush(stdout);

		//Perform global CURL initialization
		curl_global_init(CURL_GLOBAL_SSL);

		updaterThread = std::thread(periodicUpdateThread);

		//Success
		printf(OKAY "\n");
	}

	/*!	User Handler Destruction Method.
	 *
	 * 	This method destroys the user handler by doing little more than telling
	 * 	the scheduled update to cease.
	 *
	 */
	void destroy() {
		//Destroy the user handler
		printf(LOADING "Destroy User Handler...");
		fflush(stdout);

		run = false;
		cv.notify_one();
		updaterThread.join();
		curl_global_cleanup();

		//Success
		printf(OKAY "\n");
	}

	/*!	User Handler update table method
	 *
	 * 	This method sends a request to the server to retrieve an updated user
	 * 	information table, which it then decodes and stores in memory.
	 */
	bool update() {
		//Create the request
		std::string url = getenv("API_BASEURL");
		url += getenv("API_LISTUSERS");
		//Send a get request
		nlohmann::json json = Utils::jsonGetRequest(url.c_str());
		if (!Utils::jsonGetRequestSuccess()) {
			return false;
		}
		//Remove existing user records
		users.clear();
		//Iterate the elements in the response
		for(int i = 0; i < json.size(); i++) {
			//Get the object
			nlohmann::json element = json[i];
			//Get the user properties
			std::string fname = element["fname"].get<std::string>();
			std::string pin = element["pin"].get<std::string>();
			std::string rfid = element["rfid"].get<std::string>();
			bool signedin = element["signedin"].get<bool>();
			//Create the user object
			User* user = new User(fname, pin, rfid, signedin);
			//Push the user into the vector
			users.push_back(*user);
		}
		return true;
	}

	/*!	Trigger by Pin method
	 *
	 * 	This method sends a request to sign the user specified by the given pin
	 * 	in or out. If the user does not exist, such a situation is handled
	 * 	accordingly.
	 */
	void triggerPin(char* pin) {
		//UserHandler::test(); <-- I'm afraid to remove this
		//Try and find the user
		for(int i = 0; i < users.size(); i++) {
			//Check the pin
			//The next 3 print statements might get angry if you delete them
			//printf("Checking if this is %s whose ID is %s...\n", users[i].fname.c_str(), users[i].pin.c_str());
			//printf("Comparing ID [%s] and [%s]\n", users[i].pin.c_str(), pin);
			//printf("Result: %i\n", users[i].pin.compare(pin));
			if(users[i].pin.compare(pin) == 0) {
				//Message to show to the user
				std::string message = "";
				//Found the user, check their status
				if(users[i].signedin) {
					//This is goodbye :'(
					message += "Goodbye ";
					//Change the local state
					users[i].signedin = false;
				} else {
					//This is hello :D
					message += "Hello ";
					//Change the local state
					users[i].signedin = true;
				}
				//Add the users name
				message.append(users[i].fname);
				//Show that message to their face
				LCD::writeMessage(message, 0, 0);
				//Create the trigger url
				std::ostringstream url;
				url << getenv("API_BASEURL");
				url << getenv("API_TRIGGER");
				url << "?pin=";
				url << pin;
				//Send the request
				nlohmann::json resp = Utils::jsonGetRequest(url.str().c_str());
				bool signedin = resp["signed_in"].get<bool>();
				std::string actualResponse = resp["message"].get<std::string>();
				if (signedin != users[i].signedin) {
					// uh oh, problem
					users[i].signedin = signedin;
					LCD::writeMessage("                ", 0, 0);
					LCD::writeMessage(actualResponse, 0, 0);
					Buzzer::buzz(100000);
					usleep(100000);
					Buzzer::buzz(100000);
				}
				//Print to console
				printf(OKAY "%s has been %s\n", users[i].fname.c_str(),
					users[i].signedin ? "signed out" : "signed in");
				//Finished
				return;
			}
		}
		//If the program reaches this point, there is no user with this pin
		printf(FAIL "Pin %s does not belong to anyone!\n", pin);
		LCD::writeMessage("Invalid PIN", 0, 0);
		Buzzer::buzz(1000000);
		//Change the state back to ready
		State::changeState(State::READY);
	}

	/*!	Trigger by RFID method
	 *
	 * 	This method sends a request to sign the user specified by the given
	 * 	rfid uid in or out. If the user does not exist, such a situation is
	 * 	handled accordingly.
	 *
	 * 	I wish there was a way to do this without the const, but it's not wroth
	 * 	the effort :(
	 */
	void triggerRfid(const char* rfid) {
		//Try and find the user
		for(int i = 0; i < users.size(); i++) {
			//Check the pin
			//The next 3 print statements might get angry if you delete them
			//printf("Checking if this is %s whose ID is %s...\n", users[i].fname.c_str(), users[i].pin.c_str());
			//printf("Comparing ID [%s] and [%s]\n", users[i].pin.c_str(), pin);
			//printf("Result: %i\n", users[i].pin.compare(pin));
			if(users[i].rfid.compare(rfid) == 0) {
				//Message to show to the user
				std::string message = "";
				//Found the user, check their status
				if(users[i].signedin) {
					//This is goodbye :'(
					message += "Goodbye ";
					//Change the local state
					users[i].signedin = false;
				} else {
					//This is hello :D
					message += "Hello ";
					//Change the local state
					users[i].signedin = true;
				}
				//Add the users name
				message.append(users[i].fname);
				//Show that message to their face
				LCD::writeMessage(message, 0, 0);
				//Create the trigger url
				std::ostringstream url;
				url << getenv("API_BASEURL");
				url << getenv("API_TRIGGER");
				url << "?rfid=";
				url << rfid;
				//Send the request
				nlohmann::json resp = Utils::jsonGetRequest(url.str().c_str());
				bool signedin = resp["signed_in"].get<bool>();
				std::string actualResponse = resp["message"].get<std::string>();
				if (signedin != users[i].signedin) {
					// uh oh, problem
					users[i].signedin = signedin;
					LCD::writeMessage("                ", 0, 0);
					LCD::writeMessage(actualResponse, 0, 0);
					Buzzer::buzz(100000);
					usleep(100000);
					Buzzer::buzz(100000);
				}
				//Print to console
				printf(OKAY "%s has been %s\n", users[i].fname.c_str(),
					users[i].signedin ? "signed in" : "signed out");
				//Finished
				return;
			}
		}
		//If the program reaches this point, there is no user with this pin
		printf(FAIL "RFID %s does not belong to anyone!\n", rfid);
		LCD::writeMessage("Invalid RFID", 0, 0);
		Buzzer::buzz(1000000);
		//Change the state back to ready
		State::changeState(State::READY);

	}

	/*!	Assign RFID to PIN method
	*
	* 	This method sends a request to assign an RFID tag
	*   to the user with the specified pin
	*/
	void assignRfidToPin(char* pin, const char* rfid) {
		//Try and find the user
		for (int i = 0; i < users.size(); i++) {
			//Check the pin
			//The next 3 print statements might get angry if you delete them
			//printf("Checking if this is %s whose ID is %s...\n", users[i].fname.c_str(), users[i].pin.c_str());
			//printf("Comparing ID [%s] and [%s]\n", users[i].pin.c_str(), pin);
			//printf("Result: %i\n", users[i].pin.compare(pin));
			if (users[i].pin.compare(pin) == 0) {
				//Message to show to the user
				std::string message = "Assigning tag...";
				//Show that message to their face
				LCD::writeMessage(message, 0, 0);
				//Create the trigger url
				std::ostringstream url;
				url << getenv("API_BASEURL");
				url << getenv("API_ASSIGN");
				url << "?pin=";
				url << pin;
				url << "&rfid=";
				url << rfid;
				//Send the request TODO: Error checking
				Utils::jsonGetRequest(url.str().c_str());
				// update local db
				users[i].rfid = std::string(rfid);
				//Print to console
				printf(OKAY "%s has been given rfid %s\n", users[i].fname.c_str(),
					rfid);
				//Finished
				return;
			}
		}
		//If the program reaches this point, there is no user with this pin
		printf(FAIL "Pin %s does not belong to anyone!\n", pin);
		LCD::writeMessage("     Invalid PIN", 0, 0);
		Buzzer::buzz(1000000);
		//Change the state back to ready
		State::changeState(State::READY);
	}

}
