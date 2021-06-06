#include "ST7735.h"
#include "Digits.h"

#include <pigpio.h>

#include <iostream>
#include <chrono>
#include <unistd.h>

// Optionnal Weather
#ifdef WITH_QWEATHER
	#include "QWeather/DisplayImageGenerator.h"
	#include "QWeather/OpenWeatherGrabber.h"

	#include <QtGui>
	#include <QtNetwork>
	time_t lastUpdatedWeather = 0;
#endif

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

// Optionnal weather display on display 6
#ifdef WITH_QWEATHER
void displayWeather(int spi)
{
    static OpenWeatherGrabber grabber;
    static DisplayImageGenerator generator(80,160);

	QPixmap wicon;
    float mintemp = 0, maxtemp = 0;
    grabber.grab(OWLOCATION, wicon, mintemp, maxtemp);
    std::cout << "Min/Max: " << mintemp << "/" << maxtemp << std::endl;

	QImage res = generator.createWeatherPixmap(wicon, mintemp, maxtemp).toImage();

	// Convert to RGB 565 (Byte swaped)
	uint16_t * data = new uint16_t[res.height()*res.width()];

	for ( int row = 0; row < res.height(); ++row )
	{
		for ( int col = 0; col < res.width(); ++col )
		{
			QColor clr( res.pixel( col , row ) );

			uint16_t * pixel =  &data[row*res.width()+col];

			uint16_t r = ((uint16_t)clr.red()) >> 3; // Keep 5 most significant bits
			uint16_t g = ((uint16_t)clr.green()) >> 2; // Keep 6 most significant bits
			uint16_t b = ((uint16_t)clr.blue()) >> 3; // Keep 5 most significant bits

			*pixel = (r<<11) + (g << 5) + b; // Combine to R(5)G(6)B(5) into a 16 bit integer

			// Swap bytes
			char* p = (char*) pixel;
			char tmp = p[0];
			p[0] = p[1];
			p[1] = tmp;
		}
	}

	lcdDrawImage(spi, display6, (char*) data, res.height()*res.width()*2);

	delete[] data;
}

#endif

// ============ Main ======================
int main(int argc, char ** argv)
{
#ifdef WITH_QWEATHER
   	QGuiApplication app(argc, argv);
#endif

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
#ifdef WITH_QWEATHER
			if (tim - lastUpdatedWeather > 3600) // Update each hour
				displayWeather(spi);
			lastUpdatedWeather = tim;
#else
			lcdDrawNumber(spi, display6, SPACE, digits);
#endif
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
