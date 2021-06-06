#include "OpenWeatherGrabber.h"

#include <iostream>

bool OpenWeatherGrabber::grab(const QString &location, QPixmap &icon, float &mintemp, float &maxtemp)
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
        std::cout << "Icon: " << iconID.toStdString() << std::endl;

        mintemp = (float) json["main"]["temp_min"].toDouble();
        maxtemp = (float) json["main"]["temp_max"].toDouble();

        // ICON
        if (!iconID.isEmpty())
            grabIcon(iconID, icon);
    }

    reply->deleteLater();

    return true;
}

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
