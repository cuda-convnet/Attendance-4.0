#include "State.h"
#include "LCD.h"
#include "ANSI.h"

namespace State {

	char assignRfidPin[] = "----";

	//Application state
	State state = State::PRE_INIT;

	//Network
	bool haveEthernet = false;
	bool haveWifi = false;

	void changeState(State s) {
		//Print the state change
		printf(INFO "State changed to %s\n", getName(s));
		//Change the state
		state = s;

		//Check for state specific handlers
		if(s == READY) {
			//Display the ready text
			LCD::writeMessage("     Ready!     ", 0, 0);
		}
		if (s == NO_INTERNET) {
			LCD::writeMessage("No connection   ", 0, 0);
		}
	}

	char* getName(State state) {
		//Return the name
		switch (state) {
		case PRE_INIT: return "PRE_INIT";
		case INIT: return "INIT";
		case NO_INTERNET: return "NO_INTERNET";
		case READY: return "READY";
		case INPUT: return "INPUT";
		case INPUT_ASSIGN_RFID: return "INPUT_ASSIGN_RFID";
		case READ_ASSIGN_RFID: return "READ_ASSIGN_RFID";
		case BUSY: return "BUSY";
		case ERROR: return "ERROR";
		case STOPPING: return "STOPPING";
		case STOPPED: return "STOPPED";
		default: return "UNKNOWN STATE";
		}
	}
}
