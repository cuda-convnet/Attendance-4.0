#ifndef I2C_H_
#define I2C_H_

#include "../Errors.h"

namespace I2C {
	int error = NO_ERROR;
	bool init();
	void destroy();
}

#endif /* I2C_H_ */
