#pragma once

#include "Digits.h"

#include <stdint.h>

#define PIN_BLK     15
#define PIN_RST     25
#define PIN_DC      23
#define PIN_CS1      14
#define PIN_CS2      18
#define PIN_CS3      22
#define PIN_CS4      27
#define PIN_CS5      17
#define PIN_CS6      4

#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   160

#define SERIAL_CLK_DIV 1.f

// Color definitions
#define BLACK 	        0x0000
#define BLUE 	        0x001F
#define RED 	        0xF800
#define GREEN 	        0x07E0
#define CYAN 	        0x07FF
#define MAGENTA         0xF81F
#define YELLOW 	        0xFFE0
#define ORANGE	        0xFB20
#define WHITE 	        0xFFFF
#define GRAY 	        0x4A69
#define DARK 	        0x18C3

// Display Definitions
#define display1        1
#define display2        2
#define display3        3
#define display4        4
#define display5        5
#define display6        6

#define COLON           10
#define SLASH           11
#define SPACE           12
#define AM              13
#define PM              14
#define HEART           15

#define ST7735_TFTWIDTH 	80
#define ST7735_TFTHEIGHT 	160

#define ST7735_XSTART_H         0x00
#define ST7735_XSTART_L         0x1A
#define ST7735_YSTART_H         0x00
#define ST7735_YSTART_L         0x01
#define ST7735_XEND_H           0x00
#define ST7735_XEND_L           0x69
#define ST7735_YEND_H           0x00
#define ST7735_YEND_L           0xA0
#define ST7735_16BIT            0x05

#define ST7735_NOP 	        0x00
#define ST7735_SWRESET 		0x01
#define ST7735_RDDID  		0x04
#define ST7735_RDDST  		0x09

#define ST7735_SLPIN  		0x10
#define ST7735_SLPOUT  		0x11
#define ST7735_PTLON   		0x12
#define ST7735_NORON   		0x13

#define ST7735_INVOFF 		0x20
#define ST7735_INVON  		0x21
#define ST7735_DISPOFF 		0x28
#define ST7735_DISPON 		0x29
#define ST7735_CASET 		0x2A
#define ST7735_RASET 		0x2B
#define ST7735_RAMWR 		0x2C
#define ST7735_RAMRD 		0x2E
#define ST7735_VSCRDEF 		0x33
#define ST7735_VSCSAD 		0x37

#define ST7735_COLMOD 		0x3A
#define ST7735_MADCTL 		0x36

#define ST7735_FRMCTR1 		0xB1
#define ST7735_FRMCTR2 		0xB2
#define ST7735_FRMCTR3 		0xB3
#define ST7735_INVCTR 		0xB4
#define ST7735_DISSET5 		0xB6

#define ST7735_PWCTR1 		0xC0
#define ST7735_PWCTR2 		0xC1
#define ST7735_PWCTR3 		0xC2
#define ST7735_PWCTR4 		0xC3
#define ST7735_PWCTR5 		0xC4
#define ST7735_VMCTR1 		0xC5

#define ST7735_RDID1 		0xDA
#define ST7735_RDID2 		0xDB
#define ST7735_RDID3 		0xDC
#define ST7735_RDID4 		0xDD

#define ST7735_PWCTR6 		0xFC

#define ST7735_GMCTRP1 		0xE0
#define ST7735_GMCTRN1 		0xE1

static char st7735_initSeq[] = {  // Format: cmd length (including cmd byte), post delay in units of 5 ms, then cmd payload
        1, 20, ST7735_SWRESET,                                      // Software reset
        1, 10, ST7735_SLPOUT,                                       // Exit sleep mode
        4, 0 , ST7735_FRMCTR1,0x01,0x2C,0x2D,                       // Frame rate control  
        4, 0 , ST7735_FRMCTR2,0x01,0x2C,0x2D,                       
        7, 4 , ST7735_FRMCTR3,0x01,0x2C,0x2D,0x01,0x2C,0x2D,
        2, 0 , ST7735_INVCTR,0x07,                                  // Inversion control  - Frame inversion
        4, 0 , ST7735_PWCTR1,0xA2,0x02,0x84,                        // Power control
        2, 0 , ST7735_PWCTR2,0xC5,
        3, 0 , ST7735_PWCTR3,0x0A,0x00,
        3, 0 , ST7735_PWCTR4,0x8A,0xEE,
        2, 0 , ST7735_VMCTR1,0x0E,                                  // Set VCOMH Voltage at 2.850
        2, 0 , ST7735_MADCTL,0xC8,                                  // Set MADCTL: row then column BGR order 0xC8, 0xA8, 0x68, 0x08
        2, 0 , ST7735_COLMOD,ST7735_16BIT,                          // Set colour mode to 16 bit
        5, 0 , ST7735_CASET,ST7735_XSTART_H,ST7735_XSTART_L,ST7735_XEND_H,ST7735_XEND_L,    // CASET: column addresses from 26 to 105 (0x69) 80 px
        5, 0 , ST7735_RASET,ST7735_YSTART_H,ST7735_YSTART_L,ST7735_YEND_H,ST7735_YEND_L,    // RASET: row addresses from 1 to 160 (0x00A0) 160 px
        17,0 , ST7735_GMCTRP1,0x02,0x1C,0x07,0x12,0x37,0x32,0x29,0x2D,0x29,0x25,0x2B,0x39,0x00,0x01,0x03,0x10,    // Polarity (+) gamma correction
        17,0 , ST7735_GMCTRN1,0x03,0x1D,0x07,0x06,0x2E,0x2C,0x29,0x2D,0x2E,0x2E,0x37,0x3F,0x00,0x00,0x02,0x10,    // Polarity (-) gamma correction 
        1, 2 , ST7735_INVON,                                        // Inversion on, then 10 ms delay 
        1 ,2 , ST7735_NORON,                                        // Normal display on, then 10 ms delay
        1 ,10, ST7735_DISPON,                                       // Main screen turn on, then wait 500 ms
        0                                                           // Terminate list
};

void lcdInit(unsigned int spi, char *init_seq);
void lcdStartPx(unsigned int spi);
void lcdDrawNumber(unsigned int spi, uint8_t Display, uint8_t Number, const Digits& digits);

