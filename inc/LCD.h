//Duplicate inclusion protection
#ifndef LCD_H_
#define LCD_H_

//Dependencies
#include "I2C.h"
#include <string>

/*!	LCD Control Class
 * 	Provides means to interface and control an I2C LCD display
 * 	@todo Write code and documentation
 */
class LCD {
	private:
	public:
		static bool init();
		static void destroy();
};


#endif /* LCD_H_ */
