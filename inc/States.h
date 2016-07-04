#ifndef STATES_H_
#define STATES_H_

//States
namespace State {
	state currentState = PRE_INIT;
	typedef enum State{
		PRE_INIT,
		INIT,
		INIT_ERROR,
		READY,
		KP_INPUT,
		PROCESSING,
		PROCESSING_ERROR,
		STOPPING,
		STOPPED
	};
}

#endif /* STATES_H_ */
