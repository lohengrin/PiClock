#pragma once

#include <QtGui>
#include <stdint.h>

class DisplayImageGenerator
{
public:
	DisplayImageGenerator(unsigned int width, unsigned int height);
	virtual ~DisplayImageGenerator();

	const QPixmap& createWeatherPixmap(const QPixmap& icon, float mintemp, float maxtemp );

protected:
	QPixmap myPixmap;
	QPainter myPainter;
	unsigned int myWidth;
	unsigned int myHeight;
};
