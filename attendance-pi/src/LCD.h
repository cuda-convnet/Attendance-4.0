#pragma once

#include <string>

namespace LCD {

	void init();
	void destroy();

	void writeMessage(std::string message);

	void writeByte(char);
	void write(char);
	int writeRaw(char);

	void clear();
	void home();
	void goTo(int, int);

	void wait();

	char encodeChar(char);
}
