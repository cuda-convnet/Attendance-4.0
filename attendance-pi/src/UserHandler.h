#pragma once

namespace UserHandler {

	void init();
	void destroy();

	void triggerPin(char*);
	void triggerRfid(const char*);
	bool update();
	void assignRfidToPin(char* pin, const char* rfid);
}
