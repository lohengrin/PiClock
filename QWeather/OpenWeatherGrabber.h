#pragma once

#include <QtNetwork>
#include <QtGui>

//! Class to grab weather information from OpenWeather
class OpenWeatherGrabber 
{
public:
    OpenWeatherGrabber() {}
    virtual ~OpenWeatherGrabber() {}

    //! Grab forecast weather for given location
    //! location: in Openweather format (ex: "Paris,FR")
    //! dayoffset: offset in days from today for the daily forecast
    //! icon: filled with OpenWeather icon
    //! mintemp: minimum temperature in °C
    //! mantemp: maximum temperature in °C
    //! date: forecast date/time
    bool grabForecastWeather(const QString& lat, const QString& lon, int dayoffset, QPixmap& icon, float &mintemp, float &maxtemp, struct tm& date);


protected:
    bool grabIcon(const QString& iconname, QPixmap& icon);

    QNetworkAccessManager myNam;
};
