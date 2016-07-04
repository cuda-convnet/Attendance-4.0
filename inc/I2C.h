//Duplicate inclusion protection
#ifndef I2C_H_
#define I2C_H_

/*!	I2C Interface Class
 * 	Provides means for communication with I2C devices
 */
class I2C {
	private:
	public:
		static bool init();
		static void destroy();
};

#endif /* I2C_H_ */
