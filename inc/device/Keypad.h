#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../Errors.h"

namespace Keypad {
	int error = NO_ERROR;
	bool init();
	void destroy();
}

#endif /* KEYPAD_H_ */
