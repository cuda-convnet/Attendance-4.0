#include "Utils.h"
#include "json.hpp"
#include "ANSI.h"
#include "Buzzer.h"
#include "LCD.h"
#include "State.h"

#include <curl/curl.h>
#include <fstream>
#include <ctime>
#include <vector>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

using namespace std;

namespace Utils {

	bool _jsonGetRequestSuccess = false;

	/*! Checks an individual interface's status
	 * 
	 * @return A ConnectionState struct describing the status of the interface
	 */
	ConnectionState checkInterface(const char* ifname) {
		ConnectionState connectionState;
		connectionState.dBm = 0;
		connectionState.connectionType = NO_CONNECTION;

		int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (sock < 0) {
			printf(WARN "socket() error: %d\n", errno);
			return connectionState;
		}

		// first check if the interface is up
		struct ifreq if_req = {};
		strncpy(if_req.ifr_name, ifname, IFNAMSIZ);
		int rv = ioctl(sock, SIOCGIFFLAGS, &if_req);

		if (rv == -1) {
			printf(WARN "ioctl() error: %d\n", errno);
			close(sock);
			return connectionState;
		}

		if (!((if_req.ifr_flags & IFF_UP) && (if_req.ifr_flags & IFF_RUNNING))) {
			// it's not up
			close(sock);
			return connectionState;
		}

		// then check what type of interface it is
		struct iwreq iw_req = {};
		strncpy(iw_req.ifr_name, ifname, IFNAMSIZ);
		if (ioctl(sock, SIOCGIWNAME, &iw_req) != -1) {
			connectionState.connectionType = WIFI;

			// check the signal strength
			iw_req.u.data.pointer = (iw_statistics*) malloc(sizeof(iw_statistics));
			iw_req.u.data.length = sizeof(iw_statistics);
			if (ioctl(sock, SIOCGIWSTATS, &iw_req) == -1) {
				printf(WARN "ioctl() error: %d\n", errno);
			} else if (((iw_statistics*) iw_req.u.data.pointer)->qual.updated & IW_QUAL_DBM) {
				// signal is measured in dBm and is valid for us to use
				connectionState.dBm = ((iw_statistics*) iw_req.u.data.pointer)->qual.level /*- 256*/;
			} else {
				connectionState.dBm = -1; // unknown
			}
		} else {
			connectionState.connectionType = ETHERNET;
		}

		close(sock);

		return connectionState;
	}

	/*! Checks the status for all network interfaces that are up and running
	*
	* @return A vector of ConnectionState structs for each interface that is up and running
	*/
	std::vector<ConnectionState> getConnectionState() {
		struct ifaddrs* ifaddr;
		struct ifaddrs* ifa;
		std::vector<ConnectionState> retval;

		// find all interfaces
		if (getifaddrs(&ifaddr) == -1) {
			printf(WARN "getifaddrs() error\n");

			return retval;
		}

		// check status of each interface
		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL ||
				ifa->ifa_addr->sa_family != AF_PACKET)
				continue;

			if (ifa->ifa_flags & IFF_LOOPBACK)
				continue;

			ConnectionState cs = checkInterface(ifa->ifa_name);
			if (cs.connectionType != NO_CONNECTION) {
				// this is a network that is up
				struct ifreq ifr = {};
				int fd = socket(AF_INET, SOCK_DGRAM, 0);
				if (fd >= 0) {
					ifr.ifr_addr.sa_family = AF_INET;
					strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);
					int res = ioctl(fd, SIOCGIFADDR, &ifr);
					if (res >= 0) {
						cs.ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
					}
					close(fd);
				}
				retval.push_back(cs);
			}
		}

		freeifaddrs(ifaddr);
		
		return retval;
	}

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
			curl_easy_cleanup(chandle);
			if (result != CURLE_OK) {
				std::ostringstream err;
				err << "CURL error: ";
				err << result;
				throw std::runtime_error(err.str());
			}
			//Create a json object
			nlohmann::json json = nlohmann::json::parse(response);
			//We're done here
			_jsonGetRequestSuccess = true;
			return json;
		} catch(exception e) {
			LCD::writeMessage("Request error   ", 0, 0);
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
			_jsonGetRequestSuccess = false;
			return  nlohmann::json::parse("{}");
		}
	}

	bool jsonGetRequestSuccess() {
		return _jsonGetRequestSuccess;
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

	/*! Check for internet connectivity
	*
	* 	This method returns true if the pi can connect
	*   to the attendance server, false otherwise
	*/
	bool hasInternetConnectivity() {
		CURL *curl = curl_easy_init();;
		CURLcode res;

		bool hasInternet = true;

		curl_easy_setopt(curl, CURLOPT_URL, getenv("API_BASEURL"));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
			Utils::curlParseCallback);
		std::string response;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		if ((res = curl_easy_perform(curl)) != CURLE_OK) {
			switch (res) {
			case CURLE_COULDNT_CONNECT:
			case CURLE_COULDNT_RESOLVE_HOST:
			case CURLE_COULDNT_RESOLVE_PROXY:
				hasInternet = false;
				break;
			default:
				break; // site error, guess we just continue
			}
		}

		curl_easy_cleanup(curl);
		return hasInternet;
	}

	void shutdownPi() {
		State::didTriggerShutdown = true;
		system("shutdown -h now");
	}

	void restartProgram() {
		LCD::writeMessage("Force restart...", 0, 0);
		system("service attendancev4 restart &");
		std::exit(1);
	}
}
