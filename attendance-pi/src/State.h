#pragma once

namespace State {

	enum State {
		PRE_INIT,
		INIT,
		READY,
		INPUT,
		BUSY,
		ERROR,
		STOPPING,
		STOPPED
	};

	extern State state;

	void changeState(State);
	char* getName(State);
}
