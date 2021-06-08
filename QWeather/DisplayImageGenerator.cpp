#include "DisplayImageGenerator.h"

DisplayImageGenerator::DisplayImageGenerator(unsigned int width, unsigned int height) :
    myPixmap(width, height),
    myPainter(&myPixmap),
    myWidth(width),
    myHeight(height)
{
}

DisplayImageGenerator::~DisplayImageGenerator()
{

}

const QPixmap& DisplayImageGenerator::createWeatherPixmap(const QPixmap& icon, float mintemp, float maxtemp, const struct tm& date )
{
    QPixmap scaled = icon.scaledToWidth(myWidth, Qt::SmoothTransformation);

    QFont fontTemp;
    fontTemp.setPixelSize(20);
    fontTemp.setBold(true);

    QFont fontDate;
    fontTemp.setPixelSize(20);

    // Weather icon
    myPainter.fillRect(QRect(0,0,myWidth, myHeight), QColor("black") );
    myPainter.drawPixmap(QPoint(0,0), scaled);

    // Date of forecast
    myPainter.setPen(QColor("white") );
    myPainter.setFont(fontDate);
    myPainter.drawText(QRect(20,scaled.height()-10, myWidth, 30), QString("%1/%2").arg(date.tm_mday).arg(date.tm_mon) );

    myPainter.setFont(fontTemp);

    // Max temp
    myPainter.setPen(QColor("red") );
    myPainter.drawText(QRect(0,scaled.height()+10, myWidth, 30), QString(" %1 °C").arg(std::round(maxtemp)) );
    // Min temp
    myPainter.setPen(QColor("blue") );
    myPainter.drawText(QRect(0,scaled.height()+40, myWidth, 30), QString(" %1 °C").arg(std::round(mintemp)) );

    return myPixmap;
}