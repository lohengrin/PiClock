#include "ST7735.h"
#include <pigpio.h>
#include <unistd.h>
#include <iostream>
#include <vector>

// Structure to memorize last displayed digit/theme
struct LastUpdate {
    int number = -1;
    Theme theme = Theme::Theme_Number;
};

// List of last displayed digit/theme
std::vector<LastUpdate> lastUpdate(6);


void lcdSetDC(unsigned int dc) {
    usleep(1);
    gpioWrite(PIN_DC, dc);
//    gpioWrite_masked((1u << PIN_DC) , !!dc << PIN_DC  );
    usleep(1);
}

void lcdWriteCMD(unsigned int spi, char *cmd, size_t count) 
{
    lcdSetDC(0);
    //spiWrite(spi, *cmd++);
    int res = spiWrite(spi, cmd++, 1);
    if (count >= 2) {
        lcdSetDC(1);
        for (size_t i = 0; i < count - 1; ++i)
        {
            res = spiWrite(spi,cmd++,1);
        }
    }
    lcdSetDC(1);
}

//---------------------------------------------------------------------------------
void lcdInit(unsigned int spi, char *init_seq)
{
    char *cmd = init_seq;
    while (*cmd) 
    {
        lcdWriteCMD(spi, cmd + 2, *cmd);
        usleep(*(cmd + 1) * 5000);
        cmd += *cmd + 2;
    }
}

void lcdStartPx(unsigned int spi) {
    char cmd = ST7735_RAMWR;
    lcdWriteCMD(spi, &cmd, 1);
    lcdSetDC(1);
}

void selectDisplay(uint8_t display){
    switch(display){
        case 0:
            gpioWrite(PIN_CS1, 0);
            gpioWrite(PIN_CS2, 0);
            gpioWrite(PIN_CS3, 0);
            gpioWrite(PIN_CS4, 0);
            gpioWrite(PIN_CS5, 0);
            gpioWrite(PIN_CS6, 0);
        break;
        case 1:
            gpioWrite(PIN_CS1, 0);
            gpioWrite(PIN_CS2, 1);
            gpioWrite(PIN_CS3, 1);
            gpioWrite(PIN_CS4, 1);
            gpioWrite(PIN_CS5, 1);
            gpioWrite(PIN_CS6, 1);
        break;
        case 2:
            gpioWrite(PIN_CS1, 1);
            gpioWrite(PIN_CS2, 0);
            gpioWrite(PIN_CS3, 1);
            gpioWrite(PIN_CS4, 1);
            gpioWrite(PIN_CS5, 1);
            gpioWrite(PIN_CS6, 1);
        break;
        case 3:
            gpioWrite(PIN_CS1, 1);
            gpioWrite(PIN_CS2, 1);
            gpioWrite(PIN_CS3, 0);
            gpioWrite(PIN_CS4, 1);
            gpioWrite(PIN_CS5, 1);
            gpioWrite(PIN_CS6, 1);
        break;
        case 4:
            gpioWrite(PIN_CS1, 1);
            gpioWrite(PIN_CS2, 1);
            gpioWrite(PIN_CS3, 1);
            gpioWrite(PIN_CS4, 0);
            gpioWrite(PIN_CS5, 1);
            gpioWrite(PIN_CS6, 1);
        break;
        case 5:
            gpioWrite(PIN_CS1, 1);
            gpioWrite(PIN_CS2, 1);
            gpioWrite(PIN_CS3, 1);
            gpioWrite(PIN_CS4, 1);
            gpioWrite(PIN_CS5, 0);
            gpioWrite(PIN_CS6, 1);
        break;
        case 6:
            gpioWrite(PIN_CS1, 1);
            gpioWrite(PIN_CS2, 1);
            gpioWrite(PIN_CS3, 1);
            gpioWrite(PIN_CS4, 1);
            gpioWrite(PIN_CS5, 1);
            gpioWrite(PIN_CS6, 0);
        break;
        default:
            gpioWrite(PIN_CS1, 1);
            gpioWrite(PIN_CS2, 1);
            gpioWrite(PIN_CS3, 1);
            gpioWrite(PIN_CS4, 1);
            gpioWrite(PIN_CS5, 1);
            gpioWrite(PIN_CS6, 1);
        break;
    }
}

void lcdDrawNumber(unsigned int spi, uint8_t Display, uint8_t Number, const Digits& digits)
{
    LastUpdate& lu = lastUpdate[Display-1];

    if (lu.number == Number && lu.theme == digits.theme)
        return; // Nothing to do

    selectDisplay(Display);
    spiWrite(spi, digits.d[Number], 160*80*2);

    lu.number = Number;
    lu.theme = digits.theme;
}
