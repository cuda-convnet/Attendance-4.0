#include "RFID.h"
#include "mfrc522/MFRC522.h"
#include "LCD.h"
#include "ANSI.h"
#include "Buzzer.h"
#include "UserHandler.h"
#include "State.h"

#include <bcm2835.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <thread>
#include <time.h>

using std::string;
using std::stringstream;

/*!	@section mod_init	Module Initialization
 *
 * 	The initialization process for the RFID interface first initializes the
 * 	MFRC522 library, and then proceeds to spawn a polling thread. The MFRC522
 * 	library does not throw any exceptions or return any indication of weather
 * 	or not the device is working, so we just kind of assume it is and move on
 * 	with our lives.
 *
 * 	@image html rfid_flow.png
 *
 * 	@section poll_thread	Polling Thread
 *
 * 	The polling thread continuously calls @p RFID::pollForUID() in an attempt
 * 	to retrieve the UID of any card that is brought up to the reader, either as
 * 	a ritual sacrifice or otherwise.  Should there be no card present, the
 * 	thread simply tries again, after waiting an excruciating 2 milliseconds.
 *
 *	@image html rfid_threadflow.png
 */
namespace RFID {

	///RFID reader reference object
	MFRC522* mfrc;

	///RFID polling thread
	std::thread rfidThread;
	///Thread termination condition
	bool run = true;

	///Polling thread
	void thread();

	///UID of last card read - used to prevent reading same card multiple times
	std::string lastUID = "--------";
	///Time the last card was read - used to override @p lastUID condition
	std::clock_t lctime;

	/*!	RFID Initialization Method.
	 *
	 * 	This method initializes the RFID interface by first initializing the
	 * 	MFRC522 RFID module, and then spawning the RFID polling thread.
	 */
	void init() {
		printf(LOADING "Initializing RFID...");
		fflush(stdout);

		// initialize MFRC522 library
		mfrc = new MFRC522();
		mfrc->PCD_Init();

		//Start the thread
		rfidThread = std::thread(thread);

		printf(OKAY "\n");
	}

	/*!	RFID Destruction Method.
	 *
	 * 	This method destroys the RFID interface by first destroying the
	 * 	MFRC522 object, ending the bcm2835's SPI interface, instructing the
	 * 	polling thread to terminate, joining the polling thread, and finally
	 * 	returning control to the application.
	 */
	void destroy() {
		printf(LOADING "Destroy RFID...");
		fflush(stdout);

		// clean up MFRC522 library and SPI
		delete mfrc;
		bcm2835_spi_end();

		//Instruct the thread to terminate
		run = false;
		//Join the thread
		rfidThread.join();

		printf(OKAY "\n");
	}

	/*!	RFID polling method.
	 *
	 * 	This method polls the MFRC522 RFID module for any present cards, and
	 * 	if found, returns an @p RFIDPollResult object containing the UID of the
	 * 	detected card and the @p success value set to @p true.
	 *
	 * 	If the reader fails to detect a card, or detects a card but is unable to
	 * 	read the UID, it returns an @p RFIDPollResult object in which the
	 * 	@p success value is set to @p false, and the @p uid value is an empty
	 * 	string.
	 *
	 * 	@returns	An @p RFIDPollResult object containing a boolean value
	 * 		representing the outcome of the request, and the UID of the detected
	 * 		card if the request was successful.
	 */
	RFIDPollResult pollForUID() {
		if (!mfrc->PICC_IsNewCardPresent()) {
			return{ false, "" };
		}

		// read the UID of the card, which will be stored in mfrc->uid
		if (!mfrc->PICC_ReadCardSerial()) {
			printf(WARN "RFID failed to read card\n");
			return{ false, "" };
		}

		//Convert the UID to a hex string
		char buf[8];
		//Get the bytes
		byte b0 = mfrc->uid.uidByte[0];
		byte b1 = mfrc->uid.uidByte[1];
		byte b2 = mfrc->uid.uidByte[2];
		byte b3 = mfrc->uid.uidByte[3];
		//Get UID
		sprintf(buf,"%02X%02X%02X%02X", b0, b1, b2, b3);

		//Convert UID to string
		string uid(buf);

		//Return RFIDPollResult struct containing data
		return { true, uid };
	}

	/*!	RFID polling thread.
	 *
	 * 	This method is spawned as a new thread by the RFID initialization
	 * 	process, and handles the dirty work of repeatedly polling the RFID
	 * 	module for updates, and processing them if and when they occur.
	 */
	void thread() {
		//Check termination condition
		while(run) {
			//Get the RFID poll result
			RFIDPollResult result = pollForUID();
			//Check if the polling was successful
			if(result.success) {
				//Check UID
				if(result.uid.compare(lastUID) != 0 ||
						std::difftime(std::time(0), lctime) > 1) {
					//Set the last UID variable
					lastUID = result.uid;
					//Set the last time variable
					lctime = std::time(0);
					//Trigger the event
					UserHandler::triggerRfid(result.uid.c_str());
					//Beep
					Buzzer::buzz(50000);
					//Wait 1 second
					usleep(1000000);
					//Change the state back to ready
					State::changeState(State::READY);
				}
			}
			//Delay before checking again
			usleep(2000);
		}
	}
}
