#pragma once

#include <string>

#define CHAR_NO_NET 0x03
#define CHAR_ETH 0x01
#define CHAR_WIFI 0x02

namespace LCD {

	void init();
	void destroy();

	void writeMessage(std::string,int,int);

	void clear();
	void home();
}
