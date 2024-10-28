#pragma once

#include <QtNetwork>
#include <QtGui>

//! Class to grab weather information from OpenWeather
class OpenWeatherGrabber 
{
public:
    struct WeatherData {
        QPixmap icon;       //! icon: filled with OpenWeather icon
        float mintemp=0;    //! mintemp: minimum temperature in °C
        float maxtemp=0;    //! maxtemp: maximum temperature in °C
        struct tm date;     //! date: forecast date/time
        bool valid=false;
    };

    OpenWeatherGrabber() {}
    virtual ~OpenWeatherGrabber() {}

    //! Grab forecast weather for given location
    //! location: in Openweather format (ex: "Paris,FR")
    //! dayoffsets: list of offsets in days from today for the daily forecast
    //! results: list of asked forecast
    bool grabForecastWeather(const QString& lat, const QString& lon, const std::vector<int>& dayoffsets, std::vector<WeatherData> & results);

protected:
    bool grabIcon(const QString& iconname, QPixmap& icon);

    QNetworkAccessManager myNam;
};
