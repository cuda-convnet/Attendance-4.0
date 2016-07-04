#ifndef BUZZER_H_
#define BUZZER_H_

#include "../Errors.h"

namespace Buzzer {
	int error = NO_ERROR;
	bool init();
	void destroy();
}

#endif /* BUZZER_H_ */
