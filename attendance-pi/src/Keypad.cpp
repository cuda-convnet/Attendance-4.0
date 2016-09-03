#include "Keypad.h"
#include "ANSI.h"
#include "Main.h"
#include "State.h"
#include "LCD.h"
#include "Buzzer.h"
#include "UserHandler.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>
#include <thread>
#include <unistd.h>

/*!	@section mod_init	Module Initialization
 *
 * 	The initialization process for the keypad attempts to iterate through the
 * 	keymap specified by the @p keymap variable, and for each of the given pins,
 * 	change it to input mode, and then activate its internal pulldown resistor.
 * 	As the bcm2835 library provides no error handling for this operation, there
 * 	is no way to tell if the operation was successful.
 *
 * 	@image html keypad_flow.png
 *
 * 	@section poll_thread	Polling Thread
 *
 * 	The polling thread continuously monitors the 12 GPIO pins assigned to keys
 * 	on the keypad for a positive change, and if detected, trigger the
 * 	appropriate event handler method.
 *
 * 	@image html keypad_threadflow.png
 *
 */
namespace Keypad {

	//Private declarations
	void handle(char);
	void thread();
	char codeToChar(int);

	///Thread termination condition
	bool run = true;

	/*!	Key release state.
	 *
	 * 	The key release state is set to HIGH when a keypress is detected, and
	 * 	is not returned to LOW until the key is registered as being up.  This
	 * 	prevents the keypress from being registered repeatedly for a single
	 * 	actual press.
	 */
	int keystate[] { LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW };

	///Polling thread
	std::thread pollThread;

	///Key map.
	int keymap[] {
			RPI_V2_GPIO_P1_13,	//1
			RPI_V2_GPIO_P1_40,	//2
			RPI_V2_GPIO_P1_33,	//3
			RPI_V2_GPIO_P1_12,	//4
			RPI_V2_GPIO_P1_18,	//5
			RPI_V2_GPIO_P1_32,	//6
			RPI_V2_GPIO_P1_11,	//7
			RPI_V2_GPIO_P1_16,	//8
			RPI_V2_GPIO_P1_31,	//9
			RPI_V2_GPIO_P1_15,	//0
			RPI_V2_GPIO_P1_29,	//Enter #
			RPI_V2_GPIO_P1_07	//Clear *
	};

	///Input buffer
	char input[] = "----";
	///Position to write the next digit to in the input buffer
	int ipos = 0;

	/*!	Keypad Initialization Method.
	 *
	 * 	This method initializes the keypad by first configuring the GPIO pin
	 * 	associated with each individual key, and then spawning the pin polling
	 * 	thread.
	 *
	 */
	void init() {
		//Initialize the keypad
		printf(LOADING "Initializing Keypad...");
		fflush(stdout);

		//Configure each key
		for(int i = 0; i < 12; i++) {
			//Set to input
			bcm2835_gpio_fsel(keymap[i], BCM2835_GPIO_FSEL_INPT);
			//Set to pull-down
			bcm2835_gpio_set_pud(keymap[i], BCM2835_GPIO_PUD_DOWN);
		}

		//Start the GPIO monitor thread
		pollThread = std::thread(thread);

		//Success
		printf(OKAY "\n");
	}

	/*!	Keypad Destruction Method.
	 *
	 * 	This method instructs the GPIO polling thread to terminate and then
	 * 	rejoins it with the main thread to facilitate a clean shutdown.
	 *
	 */
	void destroy() {
		//Destroy the keypad
		printf(LOADING "Destroying Keypad...");
		fflush(stdout);

		//Instruct the thread to terminate
		run = false;
		//Join the thread
		pollThread.join();

		//Success
		printf(OKAY "\n");
	}

	/*!	Keypad polling thread.
	 *
	 * 	This method is spawned as a new thread by the keypad initialization
	 * 	process, and handles the dirty work of listening for when a user has
	 * 	pressed one of the keys by continuously polling the GPIO pins.
	 *
	 */
	void thread() {
		//Loop
		while(run) {
			//Iterate over each input
			for(int i = 0; i < 12; i++) {
				//Get the pin state
				if(bcm2835_gpio_lev(keymap[i]) == HIGH) {
					//Check the key state
					if(keystate[i] == HIGH) { continue; } //Already registered
					//Invoke the handler
					handle(codeToChar(i));
					//Set key state
					keystate[i] = HIGH;
				} else {
					//Set state to low
					keystate[i] = LOW;
				}
			}
			//Give the processor a bit of time
			usleep(2000);
		}
	}

	/*!	Keypad event handle method.
	 *
	 * 	This method is called by the keypad polling thread whenever it detects
	 * 	that a key has been pressed, and passes the the respective key as an
	 * 	argument.
	 *
	 * 	@todo Create a nifty diagram
	 *
	 * 	@todo Create a better display clear system
	 *
	 * 	@todo Implement user input handling
	 *
	 * 	@param key	The key that was pressed
	 */
	void handle(char key) {
		//Check the state
		if(State::state == State::READY) {
			//Change the state
			State::changeState(State::INPUT);
			//Clear the first line of the display
			LCD::writeMessage("                ",0,0);
		} else if(State::state != State::INPUT) {
			//Not allowed
			printf(WARN "Input received during illegal state\n");
			return;
		}

		if(key == '*') {	//Check if this is the reset command
			//Clear the display
			reset();
			//Beep
			Buzzer::buzz(25000);
			printf(INFO "Cleared the display\n");
		} else if(key == '#') {	//Check if this is the submit command
			//Check he number of digits
			if(ipos < 4) {
				//Not enough digits
				printf(WARN "Not enough digits\n");
				//Beep
				Buzzer::buzz(25000);
			} else {
				//Trigger the event
				UserHandler::triggerPin(input);
				//Beep
				Buzzer::buzz(25000);
				//Sleep for 1 second
				usleep(1000000);
				//Clear the display
				reset();
				//Change the state back to ready
				State::changeState(State::READY);
				//Return
				return;
			}
		} else if(ipos > 3) {	//Check the number of digits
			//Not allowed - too many digits
			printf(WARN "Too many digits\n");
			//Beep
			Buzzer::buzz(25000);
			usleep(25000);
			Buzzer::buzz(25000);
			usleep(25000);
		} else {
			//Beep
			Buzzer::buzz(25000);
			//Set the character
			input[ipos] = key;
			//Increment the input position counter
			ipos++;
		}

		//Update the display
		LCD::writeMessage(input,0,6);

	}

	/*!	Input buffer reset method.
	 *
	 * 	This method clears the input buffer and digit position counter. This
	 * 	method does not update the display, however, which must still be done
	 * 	manually if that is the desired behavior.
	 */
	void reset() {
		//Clear the buffer
		input[0] = '-';
		input[1] = '-';
		input[2] = '-';
		input[3] = '-';
		//Reset the position counter
		ipos = 0;
	}

	/*!	Key index to character converter.
	 *
	 * 	This method takes the index of a key as specified in @p keymap[] and
	 * 	returns the respective character for it, or @p \\0 if somebody screwed
	 * 	up the code.
	 *
	 * 	@param key	The index of the key
	 * 	@returns	The character representation of that key.
	 */
	char codeToChar(int key) {
		//Find it
		switch(key) {
			case 0: return '1';
			case 1: return '2';
			case 2: return '3';
			case 3: return '4';
			case 4: return '5';
			case 5: return '6';
			case 6: return '7';
			case 7: return '8';
			case 8: return '9';
			case 9: return '0';
			case 10: return '#';
			case 11: return '*';
			default: return '\0';
		}
	}

}
