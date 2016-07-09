#include "LCD.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>
#include <unistd.h>

#define I2C_SLAVE_ADDRESS	0x27

#define BACKLIGHT_BIT	0b00001000
#define ENABLE_BIT		0b00000100
#define REGSELECT_BIT	0b00000001

/*	I2C MESSAGE STRUCTURE
 *
 */

namespace LCD {

	bool backlight = false;
	bool regSelect = false;
	bool readWrite = false;

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

		//Set display to command mode
		regSelect = false;
		//Send the initialization data
		write(0b0010);	usleep(5000);	//Set mode to 4-bit
		write(0b1000);	usleep(5000);	//Set to 2 line, 5x8 character mode

		write(0b0000);
		write(0b1000);	usleep(5000);	//Set display off, cursor off, and blink off

		write(0b0000);
		write(0b0001);	usleep(5000);	//Clear display and move cursor home

		write(0b0000);
		write(0b0110);	usleep(5000);	//Set cursor to increment right, don't shift screen

		write(0b0000);
		write(0b1100);	usleep(5000);	//Turn the display back on

		//Turn the backlight on
		backlight = true;

		//Test message
		writeMessage("TESTING TESTING",0);

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	void destroy() {
		//Destroy the LCD
		printf("[" WHITE "----" RESET "] Destroying LCD...");

		bcm2835_i2c_end();

		//Success
		printf("\r[" GREEN "OKAY\n" RESET);
	}

	//Writes a 4 bit message to the LCD
	void write(char byte) {
		//Shift the byte
		byte = byte << 4;

		//Set the backlight bit
		if(backlight) { byte += BACKLIGHT_BIT; }
		//Set the register select bit
		if(regSelect) { byte += REGSELECT_BIT; }

		//Write low
		writeRaw(byte);
		usleep(1500);

		//Write high
		writeRaw(byte | ENABLE_BIT);
		usleep(1500);

		//Write low
		writeRaw(byte);
		usleep(1500);
	}

	//Writes a character to the LCD
	void writeChar(char c) {
		//Get the character code
		char ccode = encodeChar(c);
		//Split the character into two messages
		char m1 = (ccode & 0b11110000) >> 4;
		char m2 = (ccode & 0b00001111);
		//Set mode to character
		regSelect = true;
		//Write the two messages
		write(m1);
		write(m2);
	}

	//Writes a raw 8 bit message to the expander
	int writeRaw(char byte) {
		//Convert to character array
		char message[] { byte };
		//Send message
		return bcm2835_i2c_write(message, 1);
	}

	void writeMessage(char* message, int offset) {
		//Encode the message
		int i = 0;
		while(true) {
			//Check if end of message
			if(message[i] == '\0') { return; }
			//Write encoded character
			writeChar(message[i]);
			//Increment counter
			i++;
		}
	}

	void clear() {
		//Set mode to command
		regSelect = false;
		//Send the command
		write(0b0000);
		write(0b0001);
	}

	void home() {
		//Set mode to command
		regSelect = false;
		//Send the command
		write(0b0000);
		write(0b0010);
	}

	char encodeChar(char c) {
		switch(c) {
			case ' ': return 0b00100000;
			case '!': return 0b00100001;
			case '"': return 0b00100010;
			case '#': return 0b00100011;
			case '$': return 0b00100100;
			case '%': return 0b00100101;
			case '&': return 0b00100110;
			case '\'':return 0b00100111;
			case '(': return 0b00101000;
			case ')': return 0b00101001;
			case '*': return 0b00101010;
			case '+': return 0b00101011;
			case ',': return 0b00101100;
			case '-': return 0b00101101;
			case '.': return 0b00101110;
			case '/': return 0b00101111;

			case '0': return 0b00110000;
			case '1': return 0b00110001;
			case '2': return 0b00110010;
			case '3': return 0b00110011;
			case '4': return 0b00110100;
			case '5': return 0b00110101;
			case '6': return 0b00110110;
			case '7': return 0b00110111;
			case '8': return 0b00111000;
			case '9': return 0b00111001;
			case ':': return 0b00111010;
			case ';': return 0b00111011;
			case '<': return 0b00111100;
			case '=': return 0b00111101;
			case '>': return 0b00111110;
			case '?': return 0b00111111;

			case '@': return 0b01000000;
			case 'A': return 0b01000001;
			case 'B': return 0b01000010;
			case 'C': return 0b01000011;
			case 'D': return 0b01000100;
			case 'E': return 0b01000101;
			case 'F': return 0b01000110;
			case 'G': return 0b01000111;
			case 'H': return 0b01001000;
			case 'I': return 0b01001001;
			case 'J': return 0b01001010;
			case 'K': return 0b01001011;
			case 'L': return 0b01001100;
			case 'M': return 0b01001101;
			case 'N': return 0b01001110;
			case 'O': return 0b01001111;

			case 'P': return 0b01010000;
			case 'Q': return 0b01010001;
			case 'R': return 0b01010010;
			case 'S': return 0b01010011;
			case 'T': return 0b01010100;
			case 'U': return 0b01010101;
			case 'V': return 0b01010110;
			case 'W': return 0b01010111;
			case 'X': return 0b01011000;
			case 'Y': return 0b01011001;
			case 'Z': return 0b01011010;
			case '[': return 0b01011011;
			case '¥': return 0b01011100;
			case ']': return 0b01011101;
			case '^': return 0b01011110;
			case '_': return 0b01011111;

			case '`': return 0b01100000;
			case 'a': return 0b01100001;
			case 'b': return 0b01100010;
			case 'c': return 0b01100011;
			case 'd': return 0b01100100;
			case 'e': return 0b01100101;
			case 'f': return 0b01100110;
			case 'g': return 0b01100111;
			case 'h': return 0b01101000;
			case 'i': return 0b01101001;
			case 'j': return 0b01101010;
			case 'k': return 0b01101011;
			case 'l': return 0b01101100;
			case 'm': return 0b01101101;
			case 'n': return 0b01101110;
			case 'o': return 0b01101111;

			case 'p': return 0b01110000;
			case 'q': return 0b01110001;
			case 'r': return 0b01110010;
			case 's': return 0b01110011;
			case 't': return 0b01110100;
			case 'u': return 0b01110101;
			case 'v': return 0b01110110;
			case 'w': return 0b01110111;
			case 'x': return 0b01111000;
			case 'y': return 0b01111001;
			case 'z': return 0b01111010;
			case '{': return 0b01111011;
			case '|': return 0b01111100;
			case '}': return 0b01111101;
			//Further characters omitted

			default: return  0b00111111;	//Question mark
		}
	}

}
