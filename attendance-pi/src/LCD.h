#pragma once

namespace LCD {

	void init();
	void destroy();

	void writeMessage(char*, int);

	int writeRaw(char);
	int writeRaw2(bool,bool,bool,bool,bool,bool,bool,bool);

	void clear();
	void goTo(int);

	char encodeChar(char);
}
