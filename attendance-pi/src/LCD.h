#pragma once

#include <string>

namespace LCD {

	void init();
	void destroy();

	void writeMessage(std::string message);
	void writeChar(char c);

	void write(char byte);
	int writeRaw(char byte);

	void clear();
	void home();
	void goTo(int pos);

	char encodeChar(char c);
}
