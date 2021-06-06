#include "ST7735.h"
#include "Digits.h"

#include <pigpio.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

#define PIN_BTNA 24
#define PIN_BTNB -1 // Not used
#define PIN_BTNC -1 // Not used
#define PIN_BTND -1 // Not used

// Somme globals states
bool Backlight = false;
bool mode24h = true;
Theme currentTheme = Theme::SEG14;

// call aFunction whenever GPIO changes state
void gpio_callback(int gpio, int level, uint32_t tick)
{
	if (gpio == PIN_BTNA && level == 0) // Button A cycling all themes
	{
		currentTheme = (Theme)((int)currentTheme + 1);
		if (currentTheme == Theme::Theme_Number)
			currentTheme = (Theme) 0;
	}
}

// ============ Main ======================
int main()
{
	// Init pigpio lib
	int res = gpioInitialise();
	if (res < 0)
	{
		std::cerr << "Error in gpioInitialise(): " << res << std::endl;
		exit(1);
	}

	// Init SPI port 0
	int spi = spiOpen(0, 10000000, 0);

	// Configure GPIO directions
	gpioSetMode(PIN_CS1, PI_OUTPUT);
	gpioSetMode(PIN_CS2, PI_OUTPUT);
	gpioSetMode(PIN_CS3, PI_OUTPUT);
	gpioSetMode(PIN_CS4, PI_OUTPUT);
	gpioSetMode(PIN_CS5, PI_OUTPUT);
	gpioSetMode(PIN_CS6, PI_OUTPUT);
	gpioSetMode(PIN_DC,  PI_OUTPUT);
	gpioSetMode(PIN_RST, PI_OUTPUT);
	gpioSetMode(PIN_BLK, PI_OUTPUT);

	// Buttons
	gpioSetMode(PIN_BTNA, PI_INPUT);
	gpioSetAlertFunc(PIN_BTNA, gpio_callback);

	// Select all screens (to configure all at same time)
	selectDisplay(0);
	gpioWrite(PIN_RST, 1);

	// Init screens
	lcdInit(spi, st7735_initSeq);

	// Switch on backlight
	gpioWrite(PIN_BLK, 1);
	Backlight = true;

	lcdStartPx(spi);

	// Current digits (pointers to images)
	Digits digits;

	while (1)
	{
		// Update digit style according to current theme (changed by Button A)
		changeDigits(digits, currentTheme);

		// Get System time
		time_t tim = time(nullptr);
		struct tm t = *localtime(&tim);

		// Display digits on LCD
		// Hours + AM/PM for 12h mode
		if (!mode24h)
		{
			// 12h mode

			int hour = t.tm_hour;

			if (hour > 12)
				hour-=12;
			else if (hour == 0)
				hour = 12;

			lcdDrawNumber(spi, display1, hour/10, digits);
			lcdDrawNumber(spi, display2, hour%10, digits);
			lcdDrawNumber(spi, display6, (t.tm_hour < 12)?AM:PM, digits);
		}
		else
		{
			// 24h mode
			lcdDrawNumber(spi, display1, t.tm_hour/10, digits);
			lcdDrawNumber(spi, display2, t.tm_hour%10, digits);
			lcdDrawNumber(spi, display6, SPACE, digits);
		}

		// Minutes
		lcdDrawNumber(spi, display4, t.tm_min/10, digits);
		lcdDrawNumber(spi, display5, t.tm_min%10, digits);

		// Blinking colon on each second
		lcdDrawNumber(spi, display3, ((t.tm_sec % 2) == 0)? COLON:SPACE, digits);

		// Wait a little bit to reduce cpu load
		usleep(50000);
	}

	gpioTerminate();

	return 0;
}
