#include "LCD.h"
#include "ANSI.h"

#include <stdio.h>
#include <stdexcept>
#include <bcm2835.h>
#include <unistd.h>
#include <string>
#include <mutex>

#define I2C_SLAVE_ADDRESS	0x27

#define BACKLIGHT_BIT	0b00001000
#define ENABLE_BIT		0b00000100
#define RDWR_BIT		0b00000010
#define REGSELECT_BIT	0b00000001

#define MODE_COMMAND	false
#define MODE_CHARACTER	true

/*!	@section mod_init Module Initialization
 *
 * 	The initialization process for the LCD module first attempts to initialize
 * 	the bcm2835's I2C interface, and if successful, proceeds to the
 * 	configuration of the LCD module itself.  If the I2C initialization fails,
 * 	the method throws an exception of type std::runtime_error().  The process
 * 	for initializing the LCD module involves shifting the device into 4-bit
 * 	mode, specifying the character mode, and specifying other miscellaneous
 * 	operating parameters.
 *
 * 	@image html lcd_flow.png
 *
 *	@section lcd_init LCD Initialization
 *
 *	The LCD must be properly initialized before it can be used. Initialization
 *	configures the device to use a 4-bit communication model instead of the
 *	standard 8-bit one. The initialization process also configures settings on
 *	the device such as disabling the blinking cursor, hiding the cursor
 *	altogether, and disabling scrolling.
 *
 *	@section lcd_hwdoc LCD Communication Protocol
 *
 *	Following initialization, the LCD is set to operate in 4-bit mode.  The
 *	4 bits used for communication correspond to the higher 4 bits of the raw
 *	i2c message, as shown in the below chart:
 *
 *	@image html lcd_cmdbyte.png
 *
 *	The <b>BL</b> bit is used to determine weather or not the display's LED
 *	backlight should be turned on or off.<br>
 *	The <b>EN</b> bit is used to "push" the state of D1-D4 into the memory of
 *	the LCD controller.  After data has been written to pins D1-D4, the EN pin
 *	is pulled high momentarily to instruct the controller to read the 4 bits
 *	available.<br>
 *	The <b>R/W</b> bit is used to change the device between read and write
 *	mode.<br>
 *	The <b>RS</b> bit selects the target register to write data to.  When high,
 *	data is interpreted as a command, such as clear or go home.  When low, data
 *	is interpreted as a character.
 *
 *	Because the LCD requires 8 bits of data to perform an action, the command
 *	is split in half, with the higher 8 bits being sent first, followed by the
 *	second immediately after.
 *
 *	More complete documentation of the commands and character codes used by the
 *	LCD module can be found on its datasheet, which can be found
 *	<a href="https://www.sparkfun.com/datasheets/LCD/GDM1602K-Extended.pdf">
 *	here</a>
 *
 *	@todo Implement thread safe checking
 */
namespace LCD {

	//Private declaractions
	void writeDisplay(char);		//Writes a byte to the DISPLAY
	void writeExpander(char);		//Writes a byte to the EXPANDER
	char encodeChar(char);			//Encodes character to display format
	int writeRaw(char);
	void setMode(bool mode);		//Sets the target register

	///Weather or not the LCD backlight is on. true = on, false = off.
	bool backlight = true;
	///Register selector. true = character register, false = command register.
	bool regSelect = false;
	///Read/write register.  Currently unused.
	bool readWrite = false;

	///Locking mutex
	std::mutex lmux;

	/*!	LCD Initialization Method.
	 *
	 * 	This method initializes the bcm2835's I2C interface, then sends the
	 * 	proper messages to configure the LCD device.
	 */
	void init() {
		//Initialize the LCD
		printf(LOADING "Initializing LCD...");
		fflush(stdout);

		//Initialize the I2C connection
		if(!bcm2835_i2c_begin()) {
			//Failed to initialize I2C
			printf("\r[" RED "FAIL\n" RESET);
			throw std::runtime_error("Failed to initialize I2C");
		}

		//Set the display slave address
		bcm2835_i2c_setSlaveAddress(I2C_SLAVE_ADDRESS);

		/*	Bug - The initialization data must be sent twice, and at some point
		 * 	during the process a message must be written to the display. I have
		 * 	absolutely no idea why this is, but I've spent about 5 hours trying
		 * 	to figure it out to no avail. This works for whatever reason, so it
		 * 	should probably work well enough to sort of leave it the way it is.
		 */
		for(int i = 0; i < 2; i++) {
			//Set display mode
			setMode(MODE_COMMAND);
			writeDisplay(0b00101000);	//Set mode to 4-bit 2 line 5x8 character
			usleep(10000);
			writeDisplay(0b00101000);	//Set mode to 4-bit 2 line 5x8 character
			usleep(10000);
			writeDisplay(0b00101000);	//Set mode to 4-bit 2 line 5x8 character
			usleep(10000);

			writeDisplay(0b00001100);	//Set display on, cursor off, blink off
			usleep(10000);

			writeDisplay(0b00000110);	//Set cursor to increment right, don't shift
			usleep(10000);

			writeDisplay(0b00000001);	//Clear the display
			usleep(10000);

			//Write an empty message
			writeMessage("!",0,0);
		}

		//Something must be wrong here but I have no idea
		writeMessage("   Loading...",0,0);

		//Success
		printf(OKAY "\n");
	}

	/*!	LCD Destruction Method.
	 *
	 * 	This method terminates the bcm2835's I2C interface, contributing to a
	 * 	more graceful shutdown sequence.
	 */
	void destroy() {
		//Destroy the LCD
		printf(LOADING "Destroying LCD...");

		clear();

		bcm2835_i2c_end();

		//Success
		printf(OKAY "\n");
	}

	/*!	LCD write method.
	 *
	 * 	@warning This method should never be called directly, as doing so may
	 * 	result in i2c message synchronization issues.
	 *
	 * 	This method converts a 4-bit nybble destined for the LCD into a properly
	 * 	formatted 8-bit message which includes necessary bits for identifying
	 * 	the target register and weather or not the backlight should be enabled.
	 *
	 * 	@param byte	Byte to write to the LCD.  Note that the 4 higher order bits
	 * 		will be ignored.
	 */
	void writeExpander(char byte) {
		//Shift the byte
		byte = byte << 4;

		//Set the backlight bit
		if(backlight) { byte += BACKLIGHT_BIT; }
		//Set the register select bit
		if(regSelect) { byte += REGSELECT_BIT; }

		//Write low
		int r1 = writeRaw(byte);
		usleep(100);

		//Write high
		int r2 = writeRaw(byte | ENABLE_BIT);
		usleep(100);

		//Write low
		int r3 = writeRaw(byte);
		usleep(100);

		//Check for error
		if(r1 + r2 + r3 > 0) {
			printf(WARN "Write error: %i %i %i\n", r1, r2, r3);
		}
	}

	/*!	Character write method.
	 *
	 * 	This method takes the given character, encodes it so that it can be
	 * 	understood by the LCD, specifies the proper register, and then sends it
	 * 	to the LCD.
	 *
	 *	@param c	Character to display.
	 */
	void writeDisplay(char byte) {
		//Split the character into two messages
		char m1 = (byte & 0b11110000) >> 4;
		char m2 = (byte & 0b00001111);
		//Write the two messages
		writeExpander(m1);
		writeExpander(m2);

		/*	This is old debugging code for communications to the LCD.  I was
		 *	having troubles caused by an improperly set bit, and I ended up
		 *	writing this thing in the process, but once the issue was
		 *	resolved I didn't have the heart to delete it.

		std::string ba;
		if(regSelect) {
			ba += "Character command to draw '";
			ba += byte;
			ba += "'";
		} else {
			ba += "Display command to ";
			if(byte == 0b00000001) {
				ba += "clear display";
			} else if(byte == 0b00000010) {
				ba += "return home";
			} else if(byte == 0b00000011) {
				ba += "return home (extra bit)";
			} else if(byte & 0b10000000) {
				ba += "set DDRAM address to ";
				ba += (int) (byte & 0b01111111);
			} else if(byte & 0b01000000) {
				ba += "set CGRAM address to ";
				ba += (int) (byte & 0b00111111);
			} else if(byte & 0b00100000) {
				ba += "set display function: ";
				if(byte & 0b00010000) {
					ba += "Data: 8-bit  ";
				} else {
					ba += "Data: 4-bit  ";
				}
				if(byte & 0b00001000) {
					ba += "Lines: 2-line  ";
				} else {
					ba += "Lines: 1-line  ";
				}
				if(byte & 0b00000100) {
					ba += "Format: 5x11 dots";
				} else {
					ba += "Format: 5x8 dots";
				}
			} else if(byte & 0b00010000) {
				ba += "shift cursor/display: ";
				if(byte & 0b00001100) {
					ba += "Shift entire display to right, cursor moves along";
				} else if(byte & 0b00001000) {
					ba += "Shift entire display to left, cursor moves along";
				} else if(byte & 0b00000100) {
					ba += "Shift cursor to the right, AC increased by 1";
				} else {
					ba += "Shift cursor to the left, AC decreased by 1";
				}
			} else if(byte & 0b00001000) {
				ba += "set display on/off control: ";
				if(byte & 0b00000100) {
					ba += "Display: ON  ";
				} else {
					ba += "Display: OFF  ";
				}
				if(byte & 0b00000010) {
					ba += "Cursor: ON  ";
				} else {
					ba += "Cursor: OFF  ";
				}
				if(byte & 0b00000001) {
					ba += "Blink: ON";
				} else {
					ba += "Blink: OFF";
				}
			} else if(byte & 0b00000100) {
				ba += "set entry mode: ";
				if(byte & 0b00000010) {
					ba += "DDRAM: Increment  ";
				} else {
					ba += "DDRAM: Decrement  ";
				}
				if(byte & 0b00000001) {
					ba += "Shift: ON";
				} else {
					ba += "Shift: OFF";
				}
			} else {
				ba += "-UNKNOWN COMMAND-";
			}
		}

		//DEBUG
		printf("%s\n", ba.c_str());
	*/
	}

	/*!	Write a raw byte to the IO expander.
	 *
	 * 	This method writes an 8-bit message directly to the IO expander on the
	 * 	back of the LCD module.
	 *
	 * 	@param byte	The byte to write to the IO expander.
	 */
	int writeRaw(char byte) {
		//Convert to character array
		char message[] { byte };
		//Send message
		return bcm2835_i2c_write(message, 1);
	}

	/*! Sets cursor position and writes message to display.
	 *
	 * 	This message writes @message to the display starting at the coordinates
	 * 	specified by the variables @p row and @p col.
	 *
	 * 	@param message	Null-terminated string to display
	 * 	@param row	The row to write the message to
	 * 	@param col	The column to write the message to
	 */
	void writeMessage(std::string message, int row, int col) {
		//Lock thread to prevent simultaneous LCD operations
		std::lock_guard<std::mutex> lock (lmux);

		//Set the mode to comand
		setMode(MODE_COMMAND);
		//Calculate raw offset
		int offset = (row * 64) + col;
		//Create the command byte
		char cmd = (offset & 0b01111111) | 0b10000000;
		//Send the command
		writeDisplay(cmd);

		//Set mode to char
		setMode(MODE_CHARACTER);
		//printf(INFO "Printing message \"%s\" of length %i\n", message.c_str(), message.size());
		//Encode the message
		for (unsigned int i = 0; i < message.size(); i++) {
			//Write encoded character
			writeDisplay(encodeChar(message.c_str()[i]));
		}
		//printf(INFO "Finished printing message\n");
	}

	/*!	Clear the display.
	 *
	 *	This method does not clear the display. It doesn't. Nope. Not at all.
	 *	I don't know where you got that idea.
	 */
	void clear() {
		//Lock thread to prevent simultaneous LCD operations
		std::lock_guard<std::mutex> lock (lmux);

		//Set mode to command
		setMode(MODE_COMMAND);
		//Send the command
		writeDisplay(0b00000001);
	}

	/*!	Return cursor to home.
	 *
	 * 	This method moves the cursor position back to zero.
	 */
	void home() {
		//Lock thread to prevent simultaneous LCD operations
		std::lock_guard<std::mutex> lock (lmux);

		//Set mode to command
		setMode(MODE_COMMAND);
		//Send the command
		writeDisplay(0b00000010);
	}

	/*!	Character encoder.
	 *
	 * 	This method takes the input character c, and returns the corresponding
	 * 	LCD character code, if found.
	 *
	 * 	@param c	Input character
	 * 	@return		LCD character code, or "?" if the character was not
	 * 		recognized.
	 */
	char encodeChar(char c) {
		// the range of characters that match ascii
		// 33 is ' ' and 125 is '}'
		// (except the yen symbol which seems to be randomly in there
		// in the place of '\'.
		// not that we use the yen symbol or \ anyway)
		if (c < 32 || c > 125) {
			return '?';
		} else {
			return c;
		}
	}

	/*! Mode selector
	 *
	 * 	User-friendly approach to setting the appropriate register. For command
	 * 	operations, the register selector should be set to LOW, whereas for
	 * 	character write operations, it should be set to HIGH.
	 */
	void setMode(bool state) {
		//Select the appropriate register
		regSelect = state;
	}

}
