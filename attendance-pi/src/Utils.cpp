#include "Utils.h"
#include "json.hpp"
#include "ANSI.h"
#include "Buzzer.h"

#include <curl/curl.h>
#include <fstream>
#include <ctime>

using namespace std;

namespace Utils {

	//Get time prototype
	string getTime();

	/*! Request callback method
	 *
	 * 	This method passes data read by curl and passes it into the json
	 * 	object for parsing.
	 */
	size_t curlParseCallback(char *ptr, size_t size, size_t nmemb,
			string *string) {
		//Get the real size
		size_t realsize = size * nmemb;
		//Convert into a string
		std::string tempstring(ptr, size);
		//Write the temp string to the json
		string->append(ptr, realsize);
		//Return the number of bytes taken care of
		return realsize;
	}

	/*! JSON request object
	 *
	 * 	This method sends a get request to the URL passed in @p url, and parses
	 * 	the returned information as a json object, which it then returns.
	 *
	 * 	@parameter url The URL to send the get request to, including parameters
	 *
	 * 	@returns The decoded json object
	 *
	 * 	@throws Probably throws something if such and such does a thing, but I
	 * 			quite honestly have absolutely no idea.  Maybe someone else can
	 * 			figure this out down the line and edit this comment so that I
	 * 			don't look like a sleep deprived programmer, despite the fact
	 * 			that that is the most accurate way to describe me currently.
	 */
	nlohmann::json jsonGetRequest(const char* url) {
		//Server response
		string response;
		//Be prepared to catch errors here
		try {
			//DEBUG
			//printf("Sending request to %s...\n", url);
			//Create the curl handle
			CURL* chandle = curl_easy_init();
			//Set the url
			curl_easy_setopt(chandle, CURLOPT_URL, url);
			//Set the callback
			curl_easy_setopt(chandle, CURLOPT_WRITEFUNCTION,
					Utils::curlParseCallback);
			//Enable authentication
			curl_easy_setopt(chandle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
			//Set the username
			curl_easy_setopt(chandle, CURLOPT_USERNAME, getenv("HTTP_USERNAME"));
			//Set the password
			curl_easy_setopt(chandle, CURLOPT_PASSWORD, getenv("HTTP_PASSWORD"));
			//Tell curl to write the response into the string
			curl_easy_setopt(chandle, CURLOPT_WRITEDATA, &response);
			curl_easy_setopt(chandle, CURLOPT_USE_SSL, CURLUSESSL_TRY);
			//Perform the ritual sacrifice / get request
			CURLcode result = curl_easy_perform(chandle);
			if (result != CURLE_OK) {
				std::ostringstream err;
				err << "CURL error: ";
				err << result;
				throw std::runtime_error(err.str());
			}
			//Create a json object
			nlohmann::json json = nlohmann::json::parse(response);
			//We're done here
			return json;
		} catch(exception e) {
			//Make an error sound
			Buzzer::buzz(1000000);
			//Something went wrong!
			printf(WARN "Utils::jsonGetRequest() encountered an exception!\n");
			//Create the error message
			string message;
			message += "Encountered an error while performing a json ";
			message += "get request.  Details of the error are below:\n\n";
			message += "URL: ";
			message += url;
			message += "\n";
			message += "Username: ";
			message += getenv("HTTP_USERNAME");
			message += "\n";
			message += "Password: [not shown]\n";
			message += "Raw message:\n";
			message += response;
			//Save the error
			Utils::writeError(message);
			//TODO: Proper error handling here
			return  nlohmann::json::parse("{}");
		}
	}

	/*! Write an error detail to the disk
	 *
	 * 	This method takes the given error string and writes it to the disk. It
	 * 	will also print a message to the console informing that the error has
	 * 	been written.
	 */
	void writeError(string message) {
		//Get the file name
		string fname("error_");
		fname += Utils::getTime();
		fname += ".txt";
		//Create the output stream
		std::ofstream file;
		//Open the file
		file.open(fname);
		//Write the message to the file
		file << message;
		//Close the file
		file.close();
		//Print
		printf(INFO "Error details have been saved to %s\n", fname.c_str());
	}

	/*! Get the current time as a string
	 *
	 * 	This method gets and returns the time as a string formatted such that
	 * 	it is a valid filename.  This method is used for generating error
	 * 	reports.
	 */
	string getTime() {
		//Time string
		std::ostringstream ss;
		//Get the time object
		time_t t = time(0);
		//Get the local time
		struct tm * now = localtime(&t);
		//Convert to a string
		ss << (now->tm_year + 1900) << "-";
		ss << (now->tm_mon + 1) << "-";
		ss << (now->tm_mday) << "_";
		ss << (now->tm_hour) << "-";
		ss << (now->tm_min) << "-";
		ss << (now->tm_sec);
		//Return the time
		return ss.str();
	}
}
