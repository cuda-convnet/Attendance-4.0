#ifndef RFID_H_
#define RFID_H_

#include "../Errors.h"

namespace RFID {
	int error = NO_ERROR;
	bool init();
	void destroy();
}

#endif /* RFID_H_ */
