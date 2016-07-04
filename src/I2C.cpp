#include "../inc/I2C.h"
#include <ioctl.h>

/*!	I2C Class Constructor.
 * 	Opens and prepares the i2c device for use by the application
 * 	@param	file	Path to the i2c device file
 * 	@param	addr	I2C address of the target device
 * 	@todo Implement proper error checking and handling
 */
I2C::I2C(char* file, int addr) {
	//Open the handle
	if((i2cFileHandle = open(file, O_RDWR)) < 0) {
		//Fatal error
		return;
	}
	//Set the target address
	if(ioctl(i2cFileHandle, I2C_SLAVE, addr) < 0) {
		//Fatal error
		return;
	}

}

/*!	I2C Class Destructor */
I2C::~I2C() {

}

/*! Write Method.
 * 	Writes the given information to the i2c channel
 * 	@param	message	The data to be written to the I2C device
 * 	@return	True on success, False on failure
 * 	@todo There's a better way to do this
 */
bool I2C::write(char* message) {
	//Try to write to the device
	if(write(i2cFileHandle, message, sizeof(message)) {
		return false;
	}
	return true;
}
