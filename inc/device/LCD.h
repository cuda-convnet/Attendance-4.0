#ifndef LCD_H_
#define LCD_H_

#include "../Errors.h"

namespace LCD {
	int error = NO_ERROR;
	bool init();
	void destroy();
}

#endif /* LCD_H_ */
