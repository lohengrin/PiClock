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
    bool grab(const QString& location, QPixmap& icon, float &mintemp, float &maxtemp);

protected:
    bool grabIcon(const QString& iconname, QPixmap& icon);


    QNetworkAccessManager myNam;
};
