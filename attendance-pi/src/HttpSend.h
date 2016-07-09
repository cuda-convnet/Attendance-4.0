#pragma once

#include <string>

using std::string;

namespace HttpSend {
	/**
	 * Sends an HTTP request to authenticate the user with the specified user ID
	 * Returns the content of the response.
	 * 
	 * The request is a standard POST request with a text/plain body containing
	 * the user ID. The URL to request to is defined by HTTP_DOMAIN and HTTP_PATH
	 */
	string authenticateUser(string userid);
}