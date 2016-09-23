#pragma once

#include "json.hpp"

namespace Utils {

	nlohmann::json jsonGetRequest(const char*);

	bool hasInternetConnectivity();

	void writeError(std::string);
}
