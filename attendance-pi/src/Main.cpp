#include "LCD.h"
#include "GPIO.h"
#include "Keypad.h"

int main() {
	//Initialize things
	LCD::init();
	GPIO::init();
	Keypad::init();
}
