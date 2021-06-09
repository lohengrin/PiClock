#pragma once

#include <QtGui>
#include <stdint.h>

//! Class to generate weather images
class DisplayImageGenerator
{
public:
	//! Constructor
	//! (width and height only validated for 80x160, other resolution may need adaptations)
	DisplayImageGenerator(unsigned int width, unsigned int height);
	virtual ~DisplayImageGenerator();

	//! Pixmap generator from weather informations
	//! Warning not reentrant as the returned pixmap is not copied but just return by reference
	const QPixmap& createWeatherPixmap(const QPixmap& icon, float mintemp, float maxtemp, const struct tm& date );

protected:
	QPixmap myPixmap;
	QPainter myPainter;
	unsigned int myWidth;
	unsigned int myHeight;
	QFont myFontTemp;
	QFont myFontDate;
};
