#include "DisplayImageGenerator.h"

DisplayImageGenerator::DisplayImageGenerator(unsigned int width, unsigned int height) :
    myPixmap(width, height),
    myPainter(&myPixmap),
    myWidth(width),
    myHeight(height)
{
    QFont font;
    font.setPixelSize(20);
    font.setBold(true);
    myPainter.setFont(font);
}

DisplayImageGenerator::~DisplayImageGenerator()
{

}

const QPixmap& DisplayImageGenerator::createWeatherPixmap(const QPixmap& icon, float mintemp, float maxtemp )
{
    QPixmap scaled = icon.scaledToWidth(myWidth, Qt::SmoothTransformation);

    myPainter.fillRect(QRect(0,0,myWidth, myHeight), QColor("black") );
    myPainter.drawPixmap(QPoint(0,0), scaled);
    myPainter.setPen(QColor("red") );
    myPainter.drawText(QRect(0,scaled.height()+10, myWidth, 30), QString(" %1 °C").arg(std::round(maxtemp)) );
    myPainter.setPen(QColor("blue") );
    myPainter.drawText(QRect(0,scaled.height()+40, myWidth, 30), QString(" %1 °C").arg(std::round(mintemp)) );

    return myPixmap;
}