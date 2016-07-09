#pragma once

namespace LCD {

	void init();
	void destroy();

	void writeMessage(char*);
	void writeChar(char);

	void write(char);
	int writeRaw(char);

	void clear();
	void home();
	void goTo(int);

	char encodeChar(char);
}
