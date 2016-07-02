/*!	I2C Header
 * 	Provides the class definition for the I2C class
 */

//Duplicate inclusion protection
#ifndef I2C_H_
#define I2C_H_

/*!	I2C Interface Class
 * 	Provides means for communication with I2C devices
 */
class I2C {
	private:
		int i2cFileHandle;
	public:
		I2C(char*, int);
		~I2C();
		bool write(char*);
};

#endif /* I2C_H_ */
