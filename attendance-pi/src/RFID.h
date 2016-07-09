#pragma once

#include <string>

using std::string;

namespace RFID {
	/**
	 * The result of polling for a nearby RFID tag
	 * success: Whether there's a tag nearby that was successfully read
	 * uid: The uid of the tag, if success is true
	 */
	struct RFIDPollResult {
		bool success;
		std::string uid;
	};

	/*
	 * Sets up the MFRC522 library and SPI
	 */
	void init();

	/**
	 * Cleans up SPI
	 */
	void destroy();

	/*
	 * Polls for a card nearby. If there is a card, it attempts to read the UID of the card.
	 * If reading the UID was successful, it returns an RFIDPollResult with success = true
	 * and uid = the uid of the card.
	 * If reading failed or there was no card nearby, it returns an RFIDPollResult with success = false
	 * 
	 * In theory, this method shouldn't block
	 */
	RFIDPollResult pollForUID();
}
