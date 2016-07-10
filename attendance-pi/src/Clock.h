#pragma once

#include <string>

namespace Clock {

	void init();
	void destroy();

	void clockThread();

	std::string getDate();

}
