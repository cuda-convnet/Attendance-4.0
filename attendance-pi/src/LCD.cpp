#include "LCD.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>

#define I2C_SLAVE_ADDRESS	0x00
#define I2C_BAUD_RATE		9600

namespace LCD {

	void init() {
		//Initialize the LCD
		printf("[" WHITE "----" RESET "] Initializing LCD...");

		//Initialize the I2C connection
		if(!bcm2835_i2c_begin()) {
			//Failed to initialize I2C
			printf("\r[" RED "FAIL\n" RESET);
			throw std::runtime_error("Failed to initialize I2C");
		}

		//Set the display slave address
		bcm2835_i2c_setSlaveAddress(I2C_SLAVE_ADDRESS);

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void destroy() {
		//Destroy the LCD
		printf("[" WHITE "----" RESET "] Destroying LCD...");

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void writeMessage(char* message, int offset) {
		//I do not remember how to write messages to the lcd
	}

}
