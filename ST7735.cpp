#include "ST7735.h"
#include <pigpio.h>
#include <unistd.h>
#include <iostream>
#include <vector>

// Structure to memorize last displayed digit/theme for a display
struct LastUpdate {
    int number = -1;
    Theme theme = Theme::Theme_Number;
};

// List of last displayed digit/theme
std::vector<LastUpdate> lastUpdate(6);

//---------------------------------------------------------------------------------
// Set the Data/Cmd output
void lcdSetDC(unsigned int dc) {
    usleep(1);
    gpioWrite(PIN_DC, dc);
    usleep(1);
}

//---------------------------------------------------------------------------------
void lcdWriteCMD(unsigned int spi, char *cmd, size_t count) 
{
    lcdSetDC(0);
    int res = spiWrite(spi, cmd++, 1);
    if (count >= 2) 
    {
        lcdSetDC(1);
        for (size_t i = 0; i < count - 1; ++i)
            res = spiWrite(spi,cmd++,1);
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

//---------------------------------------------------------------------------------
void lcdStartPx(unsigned int spi) {
    char cmd = ST7735_RAMWR;
    lcdWriteCMD(spi, &cmd, 1);
    lcdSetDC(1);
}

//---------------------------------------------------------------------------------
void selectDisplay(uint8_t display)
{
    gpioWrite(PIN_CS1, (display == 0 || display == 1)?0:1);
    gpioWrite(PIN_CS2, (display == 0 || display == 2)?0:1);
    gpioWrite(PIN_CS3, (display == 0 || display == 3)?0:1);
    gpioWrite(PIN_CS4, (display == 0 || display == 4)?0:1);
    gpioWrite(PIN_CS5, (display == 0 || display == 5)?0:1);
    gpioWrite(PIN_CS6, (display == 0 || display == 6)?0:1);
}

//---------------------------------------------------------------------------------
void lcdDrawNumber(unsigned int spi, uint8_t Display, uint8_t Number, const Digits& digits)
{
    // Check last update
    // If no change of number or theme, avoid updating
    LastUpdate& lu = lastUpdate[Display-1];

    if (lu.number == Number && lu.theme == digits.theme)
        return; // Nothing to do

    // Select the corressponding display
    selectDisplay(Display);
    // Send Data
    spiWrite(spi, digits.d[Number], 160*80*2);

    // Store last displayed digit for this screen
    lu.number = Number;
    lu.theme = digits.theme;
}

//---------------------------------------------------------------------------------
void lcdDrawImage(unsigned int spi, uint8_t Display, char * data, int size)
{
    // Select the corressponding display
    selectDisplay(Display);
    // Send Data
    spiWrite(spi, data, size);
}
