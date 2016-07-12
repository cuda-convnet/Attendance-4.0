#include "State.h"
#include "LCD.h"
#include "ANSI.h"

namespace State {

	//Application state
	State state = State::PRE_INIT;

	void changeState(State s) {
		//Print the state change
		printf(INFO "State changed to %s\n", getName(s));
		//Change the state
		state = s;

		//Check for state specific handlers
		if(s == READY) {
			//Display the ready text
			LCD::writeMessage("     Ready!     ",0,0);
		}
	}

	char* getName(State state) {
		//Return the name
		switch(state) {
			case PRE_INIT:	return "PRE_INIT";
			case INIT:		return "INIT";
			case READY:		return "READY";
			case INPUT:		return "INPUT";
			case BUSY:		return "BUSY";
			case ERROR:		return "ERROR";
			case STOPPING:	return "STOPPING";
			case STOPPED:	return "STOPPED";
			default:	return "UNKNOWN STATE";
		}
	}
}
