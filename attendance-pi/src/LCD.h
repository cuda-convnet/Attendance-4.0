#pragma once

#include <string>

namespace LCD {

	void init();
	void destroy();

	void writeMessage(std::string,int,int);

	void clear();
	void home();
}
