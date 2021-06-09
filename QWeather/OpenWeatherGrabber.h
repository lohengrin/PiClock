#pragma once

#include <QtNetwork>
#include <QtGui>

//! Class to grab weather information from OpenWeather
class OpenWeatherGrabber 
{
public:
    OpenWeatherGrabber() {}
    virtual ~OpenWeatherGrabber() {}

    //! Grab current weather for given location
    //! location: in Openweather format (ex: "Paris,FR")
    //! icon: filled with OpenWeather icon
    //! mintemp: minimum temperature in °C
    //! mantemp: maximum temperature in °C
    bool grabCurrentWeather(const QString& location, QPixmap& icon, float &mintemp, float &maxtemp);

    //! Grab forecast weather for given location
    //! If current time is 0-17h59 => return forecast for current day
    //! If current time >= 18h => return forecast for next day
    //! location: in Openweather format (ex: "Paris,FR")
    //! dayoffset: offset in days from today for the daily forecast
    //! icon: filled with OpenWeather icon
    //! mintemp: minimum temperature in °C
    //! mantemp: maximum temperature in °C
    //! date: forecast date/time
    bool grabForecastWeather(const QString& location, int dayoffset, QPixmap& icon, float &mintemp, float &maxtemp, struct tm& date);


protected:
    bool grabIcon(const QString& iconname, QPixmap& icon);

    // Cache of location/coordinates (only curentWeather works with location)
    struct coords {
        double latitude = 0;
        double longitude= 0;
    };
    QMap<QString, coords> myLocationMap;

	// 
	double myLat;
	double myLon;



    QNetworkAccessManager myNam;
};
