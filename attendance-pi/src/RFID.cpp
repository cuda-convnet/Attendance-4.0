#include "RFID.h"
#include "mfrc522/MFRC522.h"
#include <bcm2835.h>
#include "ANSI.h"
#include <unistd.h>
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

namespace RFID {

	MFRC522* mfrc;

	void init() {
		printf("[" WHITE "----" RESET "] Initializing RFID...");

		// initialize MFRC522 library
		mfrc = new MFRC522();
		mfrc->PCD_Init();

		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void destroy() {
		printf("[" WHITE "----" RESET "] Destroy RFID...");

		// clean up MFRC522 library and SPI
		delete mfrc;
		bcm2835_spi_end();

		printf("\r[" GREEN "OKAY\n" RESET);
	}

	RFIDPollResult pollForUID() {
		if (!mfrc->PICC_IsNewCardPresent()) {
			return{ false, "" };
		}

		// read the UID of the card, which will be stored in mfrc->uid
		if (!mfrc->PICC_ReadCardSerial()) {
			printf("[" YELLOW "WARN" RESET "] RFID failed to read card");
			return{ false, "" };
		}

		// convert the UID to space-separated hex bytes
		stringstream ss;
		// Get UID
		for (int i = 0; i < mfrc->uid.size; ++i) {
			char uidByte = mfrc->uid.uidByte[i];
			if (uidByte < 0x10) {
				ss << "0";
				ss << std::hex << uidByte;
			} else {
				ss << std::hex << uidByte;
			}

		}

		string uid(ss.str());

		printf("[" GREEN "INFO" RESET "] Card UID: %s\n", uid.c_str());

		return { true, uid };
	}
}