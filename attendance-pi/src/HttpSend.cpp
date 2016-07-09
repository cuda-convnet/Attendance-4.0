#include "vs-intellisense-fix.hpp"


#include "HttpSend.h"
#include "ANSI.h"
#include <stdio.h>
#include <mutex>
#include <thread>
#include <string>
#include <sstream>

#include "rpi-http-request/HttpException.h"
#include "rpi-http-request/HttpRequest.h"


using std::string;
using std::mutex;
using std::lock_guard;
using std::stringstream;

namespace HttpSend {

#define HTTP_DOMAIN "sampletext.com"
#define HTTP_PATH "/authenticate.php"

	mutex requestMutex;
	stringstream responseData;

	void headersReady(const HttpResponse *response, void *additionalParams) {
		printf("\r[" WHITE "----" RESET "] HTTP Status: %d - %s                ",
			response->getStatus(), response->getReason());
		responseData.str(string());
	}

	void receiveData(const HttpResponse *response, void *additionalParams, const unsigned char *data, int sizeOfData) {
		responseData.write((const char*) data, sizeOfData);
	}

	void responseComplete(const HttpResponse *response, void *additionalParams) {
		printf("\r[" WHITE "----" RESET "] Http complete                        ");
	}

	string authenticateUser(string userid) {
		// make sure we don't do 2 of these at the same time
		lock_guard<mutex> authLock(requestMutex);

		printf("[" WHITE "----" RESET "] HttpSend authenticating user");

		const char *headers[] = {
			"Connection", "close",
			"Content-type", "text/plain",
			"Accept", "text/plain",
			0  //null terminator
		};

		HttpRequest request(HTTP_DOMAIN, 80);
		request.initCallbacks(headersReady, receiveData, responseComplete, 0);
		request.sendRequest("POST", HTTP_PATH, headers, (const unsigned char*) userid.c_str(), userid.size());

		while (request.responsesPending()) {
			request.processRequest();
		}

		printf("\r[" GREEN "OKAY" RESET "] HttpSend user authenticated            \n");

		return responseData.str();
	}
}