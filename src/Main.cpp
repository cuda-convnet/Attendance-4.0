/*
 * Main.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: Nicholas
 */

#include "../inc/LCD.h"

#define LCD_I2C_ADDR		0x10

//Main method
int main() {
	//Initialize the LCD
	LCD lcd = LCD(LCD_I2C_ADDR);
}

/*!
 *	\mainpage Attendance System Documentation
 *	@todo Consider relocating documentation source to a different or dedicated
 *	file
 *
 *	Testing Testing 123...
 */
