#include "LCD.h"
#include "GPIO.h"
#include "Keypad.h"
#include <stdio.h>

int main() {
	//Initialize things
	LCD::init();
	GPIO::init();
	Keypad::init();
	printf("HELLO WORLD\n");
}
