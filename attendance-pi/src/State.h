#pragma once

namespace State {

	enum State {
		PRE_INIT,
		INIT,
		NO_INTERNET,
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
	extern bool haveEthernet;
	extern bool haveWifi;

	void changeState(State);
	char* getName(State);

	extern char assignRfidPin[];
}
