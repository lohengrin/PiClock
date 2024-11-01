#include "OpenWeatherGrabber.h"

#include <iostream>

//---------------------------------------------------------------------------------------------------------
bool OpenWeatherGrabber::grabForecastWeather(const QString &lat, const QString &lon, const std::vector<int>& dayoffsets, std::vector<WeatherData> & results)
{
    // Using OneCall API
    QString address = QString("https://api.openweathermap.org/data/3.0/onecall?lat=%1&lon=%2&units=metric&appid=%3&exclude={part}").arg(48.8289).arg(2.2652).arg(OWAPPID);

    QUrl url(address);
    QNetworkRequest req(url);

    QNetworkReply *reply = myNam.get(req);

    while (!reply->isFinished())
        qApp->processEvents();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);

		// Get System time
		time_t tim = time(nullptr);
		struct tm t = *localtime(&tim);

        for (int d_offset : dayoffsets)
        {
            int i = 0;
            bool cont =true;

            WeatherData wdata;
            time_t dt = 0;
            // Search in results daily weather for current or next day accrodinh to current time
            while (cont)
            {
                dt = json["daily"][i++]["dt"].toInt();
                struct tm dtt = *localtime(&dt);

                if ( (dt == 0) || (dtt.tm_yday == t.tm_yday+d_offset)) 
                {
                    cont = false;
                    wdata.date = dtt;
                    //std::cout << "Found weather for:" << dtt.tm_mday << " " << dtt.tm_mon << std::endl;
                }
            }

            wdata.valid = (dt != 0);

            if (wdata.valid)
            {
                i--;

                // Get results
                QString iconID = json["daily"][i]["weather"][0]["icon"].toString();
                //std::cout << "Icon: " << iconID.toStdString() << std::endl;

                wdata.mintemp = (float) json["daily"][i]["temp"]["min"].toDouble();
                wdata.maxtemp = (float) json["daily"][i]["temp"]["max"].toDouble();

                // ICON
                if (!iconID.isEmpty())
                    grabIcon(iconID, wdata.icon);
            }

            results.push_back(wdata);
        }

        reply->deleteLater();
    }
	else
	{
		std::cerr << "Reply Error" << std::endl;
        reply->deleteLater();
        return false;
	}


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

