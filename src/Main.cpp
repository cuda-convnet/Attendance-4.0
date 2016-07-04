/*
 * Main.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: Nicholas
 */

#include "../inc/device/LCD.h"
#include "../inc/device/Buzzer.h"
#include "../inc/device/Keypad.h"
#include "../inc/device/RFID.h"


//Main method
int main() {
	//Initialize hardware components
	if(!LCD::init()) {
		printf("Failed to initialize LCD (%i)", LCD::error);
		return;
	}

	if(!Buzzer::init()) {
		printf("Failed to initialize Buzzer (%i)", Buzzer::error);
		return;
	}

	if(!Keypad::init()) {
		printf("Failed to initialize Keypad (%i)", Keypad::error);
		return;
	}

	if(!RFID::init()) {
		printf("Failed to initialize RFID (%i)", RFID::error);
		return;
	}
}

/*!
 *	\mainpage Attendance System Documentation
 *	@todo Consider relocating documentation source to a different or dedicated
 *	file
 *
 *	Testing Testing 123...
 */
