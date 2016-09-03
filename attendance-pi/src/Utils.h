#pragma once

#include "json.hpp"

namespace Utils {

	nlohmann::json jsonGetRequest(const char*);

	void writeError(std::string);
}
