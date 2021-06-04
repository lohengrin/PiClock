#include "ST7735.h"
#include "Digits.h"

#include <pigpio.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

#define HRS 0
#define MINS 1

#define PIN_BTNA 18
#define PIN_BTNB 7
#define PIN_BTNC 8
#define PIN_BTND 9

bool Backlight = false;
bool mode24h = true;
Theme currentTheme = Theme::VFD;

// call aFunction whenever GPIO changes state
void gpio_callback(int gpio, int level, uint32_t tick)
{
	std::cout << "Bouton press: GPIO: " << gpio << "  Level: " << level << "  Tick: " << tick << std::endl;
	if (gpio == PIN_BTNA && level == 0)
	{
		currentTheme = (Theme)((int)currentTheme + 1);
		if (currentTheme == Theme::Theme_Number)
			currentTheme = Theme::Digital;
	}
}

// Main
int main()
{
	int res = gpioInitialise();
	if (res < 0)
	{
		std::cerr << "Error in gpioInitialise(): " << res << std::endl;
		exit(1);
	}

	// Init SPI port
	int spi = spiOpen(0, 10000000, 0);

	// Configure GPIO directions
	gpioSetMode(PIN_CS1, PI_OUTPUT);
	gpioSetMode(PIN_CS2, PI_OUTPUT);
	gpioSetMode(PIN_CS3, PI_OUTPUT);
	gpioSetMode(PIN_CS4, PI_OUTPUT);
	gpioSetMode(PIN_CS5, PI_OUTPUT);
	gpioSetMode(PIN_CS6, PI_OUTPUT);
	gpioSetMode(PIN_DC, PI_OUTPUT);
	gpioSetMode(PIN_RST, PI_OUTPUT);
	gpioSetMode(PIN_BLK, PI_OUTPUT);

	// Buttons
	gpioSetMode(PIN_BTNA, PI_INPUT);
	gpioSetPullUpDown(PIN_BTNA, PI_PUD_UP);
	//gpioNoiseFilter(PIN_BTNA, 100, 1);
	//gpioSetAlertFunc(PIN_BTNA, gpio_callback);

	// Select all screens (to configure all at same time)
	gpioWrite(PIN_CS1, 0);
	gpioWrite(PIN_CS2, 0);
	gpioWrite(PIN_CS3, 0);
	gpioWrite(PIN_CS4, 0);
	gpioWrite(PIN_CS5, 0);
	gpioWrite(PIN_CS6, 0);
	gpioWrite(PIN_RST, 1);

	lcdInit(spi, st7735_initSeq);

	gpioWrite(PIN_BLK, 1);
	Backlight = true;

	lcdStartPx(spi);

	Digits digits;

	while (1)
	{
		// Update digit style according to current theme (changed by Button A)
		changeDigits(digits, currentTheme);

		// Get System time
		time_t tim = time(nullptr);
		struct tm t = *localtime(&tim);

		// Display digits on LCD
		if (!mode24h)
		{
			// 12h mode
			if (t.tm_hour >= 1 && t.tm_hour <= 12)
			{
				lcdDrawNumber(spi, display1, t.tm_hour/10, digits);
				lcdDrawNumber(spi, display2, t.tm_hour%10, digits);
			}
			else if (t.tm_hour >= 13 & t.tm_hour < 24)
			{
				lcdDrawNumber(spi, display1, (t.tm_hour-12)/10, digits);
				lcdDrawNumber(spi, display2, (t.tm_hour-12)%10, digits);
			}
			else if (t.tm_hour == 0)
			{
				lcdDrawNumber(spi, display1, 1, digits);
				lcdDrawNumber(spi, display2, 2, digits);
			}

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
		//std::cout << gpioRead(PIN_BTNA) << std::endl;
	}

	gpioTerminate();

	return 0;
}
