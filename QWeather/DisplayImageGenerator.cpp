#include "DisplayImageGenerator.h"

DisplayImageGenerator::DisplayImageGenerator(unsigned int width, unsigned int height) :
    myPixmap(width, height),
    myPainter(&myPixmap),
    myWidth(width),
    myHeight(height)
{

    myFontTemp.setPixelSize(20);
    myFontTemp.setBold(true);

    myFontDate.setPixelSize(16);
}

DisplayImageGenerator::~DisplayImageGenerator()
{

}

const QPixmap& DisplayImageGenerator::createWeatherPixmap(const QPixmap& icon, float mintemp, float maxtemp, const struct tm& date )
{
    QPixmap scaled = icon.scaledToWidth(myWidth, Qt::SmoothTransformation);

    // Weather icon
    myPainter.fillRect(QRect(0,0,myWidth, myHeight), QColor("black") );
    myPainter.drawPixmap(QPoint(0,0), scaled);

    // Date of forecast
    myPainter.setPen(QColor("white") );
    myPainter.setFont(myFontDate);
    myPainter.drawText(QRect(10,scaled.height()-10, myWidth, 30), QString("%1/%2").arg(date.tm_mday,2,10,QChar('0')).arg(date.tm_mon+1,2,10,QChar('0')) );

    myPainter.setFont(myFontTemp);

    // Max temp
    myPainter.setPen(QColor("red") );
    myPainter.drawText(QRect(0,scaled.height()+10, myWidth, 30), QString(" %1 °C").arg(std::round(maxtemp)) );
    // Min temp
    myPainter.setPen(QColor("blue") );
    myPainter.drawText(QRect(0,scaled.height()+40, myWidth, 30), QString(" %1 °C").arg(std::round(mintemp)) );

    return myPixmap;
}