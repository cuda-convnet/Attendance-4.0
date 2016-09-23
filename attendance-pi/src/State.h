#pragma once

namespace State {

	enum State {
		PRE_INIT,
		INIT,
		READY,
		INPUT,
		INPUT_ASSIGN_RFID,
		READ_ASSIGN_RFID,
		BUSY,
		ERROR,
		STOPPING,
		STOPPED
	};

	extern State state;

	void changeState(State);
	char* getName(State);

	extern char assignRfidPin[];
}
