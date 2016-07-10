#pragma once

namespace Keypad {

	void init();
	void destroy();

	void keyThread();
	void handle(char);

	char codeToChar(int);

}
