#include "OpenWeatherGrabber.h"

#include <iostream>

//---------------------------------------------------------------------------------------------------------
bool OpenWeatherGrabber::grabCurrentWeather(const QString &location, QPixmap &icon, float &mintemp, float &maxtemp)
{
    QString address = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&units=metric&appid=%2").arg(location, OWAPPID);
    QUrl url(address);
    QNetworkRequest req(url);

    QNetworkReply *reply = myNam.get(req);

    while (!reply->isFinished())
        qApp->processEvents();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);

        //std::cout << json.toJson().toStdString() << std::endl;

        QString iconID = json["weather"][0]["icon"].toString();
        //std::cout << "Icon: " << iconID.toStdString() << std::endl;

        mintemp = (float) json["main"]["temp_min"].toDouble();
        maxtemp = (float) json["main"]["temp_max"].toDouble();

        // Store location string to lat/lon for forecast use
        coords c;
        c.latitude = json["coord"]["lat"].toDouble();
        c.longitude = json["coord"]["lon"].toDouble();
        myLocationMap[location] = c;


        // ICON
        if (!iconID.isEmpty())
            grabIcon(iconID, icon);
    }

    reply->deleteLater();

    return true;
}

//---------------------------------------------------------------------------------------------------------
bool OpenWeatherGrabber::grabForecastWeather(const QString &location, int dayoffset, QPixmap &icon, float &mintemp, float &maxtemp, struct tm& date)
{
    // Location not initialized, use current time request to convert location to lat/lon
    auto it = myLocationMap.find(location);
    if (it == myLocationMap.end())
        grabCurrentWeather(location, icon, mintemp, maxtemp);

    // Using OneCall API
    QString address = 
        QString("https://api.openweathermap.org/data/2.5/onecall?lat=%1&lon=%2&exclude={part}&appid=%3&units=metric")
        .arg(myLocationMap[location].latitude).arg(myLocationMap[location].longitude).arg(OWAPPID);

    QUrl url(address);
    QNetworkRequest req(url);

    QNetworkReply *reply = myNam.get(req);

    while (!reply->isFinished())
        qApp->processEvents();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);

        //std::cout << json.toJson().toStdString() << std::endl;

        bool cont =true;
        int i = 0;

		// Get System time
		time_t tim = time(nullptr);
		struct tm t = *localtime(&tim);

        time_t dt = 0;
        // Search in results daily weather for current or next day accrodinh to current time
        while (cont)
        {
            dt = json["daily"][i++]["dt"].toInt();
		    struct tm dtt = *localtime(&dt);

            if ( (dt == 0) || (dtt.tm_yday == t.tm_yday+dayoffset)) 
            {
                cont = false;
                date = dtt;
                //std::cout << "Found weather for:" << dtt.tm_mday << " " << dtt.tm_mon << std::endl;
            }
        }

        if (dt == 0) // Forecast asked not found
            return false;

        i--;

        // Get results
        QString iconID = json["daily"][i]["weather"][0]["icon"].toString();
        //std::cout << "Icon: " << iconID.toStdString() << std::endl;

        mintemp = (float) json["daily"][i]["temp"]["min"].toDouble();
        maxtemp = (float) json["daily"][i]["temp"]["max"].toDouble();

        // ICON
        if (!iconID.isEmpty())
            grabIcon(iconID, icon);
    }

    reply->deleteLater();

    return true;
}

//---------------------------------------------------------------------------------------------------------
bool OpenWeatherGrabber::grabIcon(const QString &iconname, QPixmap &iconpixmap)
{
    QString iconaddress = QString("http://openweathermap.org/img/w/%1.png").arg(iconname);
    QUrl iconurl(iconaddress);
    QNetworkRequest reqicon(iconurl);

    QNetworkReply *replyIcon = myNam.get(reqicon);

    while (!replyIcon->isFinished())
    {
        qApp->processEvents();
    }

    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = replyIcon->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl = replyIcon->attribute(QNetworkRequest::RedirectionTargetAttribute);

    // no error received?
    if (replyIcon->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here
        QByteArray bytes = replyIcon->readAll(); // bytes
        iconpixmap.loadFromData(bytes);
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    delete replyIcon;

    return true;
}

