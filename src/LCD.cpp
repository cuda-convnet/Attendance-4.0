/*!	LCD Class
 * 	Method definitions for the LCD class
 */

#include "../inc/LCD.h"

#define I2C_FILE_LOCATION	"/dev/i2c"

/*!	LCD Class Constructor
 * 	Opens an I2C connection, and prepares for sending standard I2C display
 * 	commands to the address specified by \c address
 * 	@param	address	The I2C address where the display is located
 */
LCD::LCD(int address) {
}

/*! LCD Class Destructor
 * 	Closes the I2C connection, and frees any resources
 */
LCD::~LCD() {

}

///
void LCD::writeString(std::string message, int offset) {

}
