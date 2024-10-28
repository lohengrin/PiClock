#include "ST7735.h"
#include "Digits.h"

#include <pigpio.h>

#include <iostream>
#include <chrono>
#include <numeric>
#include <unistd.h>

#define PIN_BTNA 24 // Change Theme
#define PIN_BTNB 5 // Switch weather mode / time mode
#define PIN_BTNC -1 // Not used
#define PIN_BTND -1 // Not used

// Somme globals states
bool mode24h = true;
bool weatherForecast = false;
Theme currentTheme = Theme::VFD;

// Optionnal Weather
#ifdef WITH_QWEATHER
	#include "QWeather/DisplayImageGenerator.h"
	#include "QWeather/OpenWeatherGrabber.h"

	#include <QtGui>
	#include <QtNetwork>
	time_t lastUpdatedWeather = 0;

	const int NBDAYS = 7;
	QImage weatherImages[NBDAYS]; // 0 is current weather 1-6 is J+X
	bool redrawWeather = true;
#endif

#ifdef WITH_HTTP
#include "HttpServer.h"
#include <QCoreApplication>

bool requestCB(const HttpRequest& req)
{
        if(req.target == "/weather")
        {
			weatherForecast = !weatherForecast;
			// Force redraw and update
			lastUpdatedWeather = 0;
			redrawWeather = true;
		    return true;
        }
        else if (req.target.startsWith("/lcd"))
        {
			currentTheme = (Theme)((int)currentTheme + 1);
			if (currentTheme == Theme::Theme_Number)
				currentTheme = (Theme) 0;
			return true;
        }
        return false;
}
#endif

// call aFunction whenever GPIO changes state
void gpio_callback(int gpio, int level, uint32_t tick)
{
	if (gpio == PIN_BTNA && level == 0) // Button A cycling all themes
	{
		currentTheme = (Theme)((int)currentTheme + 1);
		if (currentTheme == Theme::Theme_Number)
			currentTheme = (Theme) 0;
	}
#ifdef WITH_QWEATHER
	else if (gpio == PIN_BTNB && level == 0) // Button B switch weather/time mode
	{

		weatherForecast = !weatherForecast;
		// Force redraw and update
		lastUpdatedWeather = 0;
		redrawWeather = true;
	}
#endif
}



// Optionnal weather display on display 6
#ifdef WITH_QWEATHER

//! Convert a Qimage to an array of uint16_t in RGB 565 (ready to use with display)
//! Caller need to free the allocated array with delete[]
uint16_t * convertToRGB565(const QImage& image)
{
	// Convert to RGB 565 (Byte swaped)
	uint16_t * data = new uint16_t[image.height()*image.width()];

	for ( int row = 0; row < image.height(); ++row )
	{
		for ( int col = 0; col < image.width(); ++col )
		{
			QColor clr( image.pixel( col , row ) );

			uint16_t * pixel =  &data[row*image.width()+col];

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

	return data;
}

void updateWeather(const struct tm& currentime)
{
    static OpenWeatherGrabber grabber;
    static DisplayImageGenerator generator(80,160);


    std::vector<int> v = {0,1,2,3,4,5,6};
	std::vector<OpenWeatherGrabber::WeatherData> results;
	grabber.grabForecastWeather(OWLAT, OWLON, v, results);

	for (int i; i < NBDAYS && i < results.size(); i++)
	{
		const OpenWeatherGrabber::WeatherData& wdata = results[i];
		weatherImages[i] = generator.createWeatherPixmap(wdata.icon, wdata.mintemp, wdata.maxtemp, wdata.date).toImage();
	}
}

void displayWeather(int spi, int display, int dayoffset, const struct tm& currentime)
{
	const QImage& img = weatherImages[dayoffset];
	uint16_t * data = convertToRGB565(img);

	lcdDrawImage(spi, display, (char*) data, img.height()*img.width()*2);

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
	gpioSetMode(PIN_BTNB, PI_INPUT);
	gpioSetAlertFunc(PIN_BTNB, gpio_callback);

	// Select all screens (to configure all at same time)
	selectDisplay(0);
	gpioWrite(PIN_RST, 1);

	// Init screens
	lcdInit(spi, st7735_initSeq);

	// Switch on backlight
	gpioWrite(PIN_BLK, 1);
	
	lcdStartPx(spi);

#ifdef WITH_HTTP
	HttpServer server(nullptr, requestCB);
#endif


	// Current digits (pointers to images)
	Digits digits;

	while (1)
	{
		// Update digit style according to current theme (changed by Button A)
		changeDigits(digits, currentTheme);

		// Get System time
		time_t tim = time(nullptr);
		struct tm t = *localtime(&tim);

#ifdef WITH_QWEATHER
		if (tim - lastUpdatedWeather > 3600) // Update each hour
		{
			updateWeather(t);
			lastUpdatedWeather = tim;
			redrawWeather = true;
		}
#endif

#ifdef WITH_QWEATHER
		if (!weatherForecast)
		{
#endif
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
				if (redrawWeather)
				{
					int dayoffset = (t.tm_hour >= 18)?1:0;
					displayWeather(spi, display6, dayoffset, t);
					redrawWeather = false;
				}
	#else
				lcdDrawNumber(spi, display6, SPACE, digits);
	#endif
			}

			// Minutes
			lcdDrawNumber(spi, display4, t.tm_min/10, digits);
			lcdDrawNumber(spi, display5, t.tm_min%10, digits);

			// Blinking colon on each second
			lcdDrawNumber(spi, display3, ((t.tm_sec % 2) == 0)? COLON:SPACE, digits);
#ifdef WITH_QWEATHER
		}
		else
		{
			// Display weather forecast one day per display
			if (redrawWeather)
			{
				for (int i = 0; i < 6; i++)
					displayWeather(spi, i+1, i, t);
				redrawWeather = false;
			}

		}
#endif

#ifdef WITH_HTTP
		QCoreApplication::processEvents();
#endif
		// Wait a little bit to reduce cpu load
		usleep(50000);
	}

	gpioTerminate();

	return 0;
}
