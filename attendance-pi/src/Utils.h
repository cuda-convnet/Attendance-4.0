#pragma once

#include "json.hpp"
#include <vector>

namespace Utils {
	typedef enum {
		NO_CONNECTION,
		ETHERNET,
		WIFI
	} ConnectionType;

	typedef struct {
		ConnectionType connectionType;
		unsigned char dBm;
	} ConnectionState;

	nlohmann::json jsonGetRequest(const char*);
	bool jsonGetRequestSuccess();

	bool hasInternetConnectivity();
	std::vector<ConnectionState> getConnectionState();

	void writeError(std::string);

	void shutdownPi();
	void restartProgram();
}
