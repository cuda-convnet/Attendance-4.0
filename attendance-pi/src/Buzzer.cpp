#include "Buzzer.h"
#include "ANSI.h"

#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>

#define BUZZER_PIN	RPI_V2_GPIO_P1_37

namespace Buzzer {

	void init() {
		//Initialize the Buzzer
		printf(LOADING "Initializing Buzzer...");
		fflush(stdout);

		//Allocate the GPIO pin
		bcm2835_gpio_fsel(BUZZER_PIN, BCM2835_GPIO_FSEL_OUTP);

		//Success
		printf(OKAY "\n");
	}

	void destroy() {
		//Destroy the Buzzer
		printf(LOADING "Destroying Buzzer...");
		fflush(stdout);

		//Nothing to do here

		//Success
		printf(OKAY "\n");
	}

	void buzz(int duration) {
		//Turn on
		bcm2835_gpio_write(BUZZER_PIN, HIGH);
		//Wait
		usleep(duration);
		//Turn off
		bcm2835_gpio_write(BUZZER_PIN, LOW);
	}
}
