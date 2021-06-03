#include "ST7735.h"

#include <pigpio.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

#define HRS 0
#define MINS 1

#define OFF 0
#define ON 1

#define PIN_BTNA 6
#define PIN_BTNB 7
#define PIN_BTNC 8
#define PIN_BTND 9

bool Backlight = OFF;
bool ConfigureTime_MODE = OFF;
uint Selection = HRS;

// Start on Friday 26th of March 2021 12:34:00
struct tm t = {
         .tm_sec   = 00,
         .tm_min   = 34,
         .tm_hour  = 12,
         .tm_mday   = 26,
         .tm_mon = 03,
         .tm_year  = 121,
         .tm_wday  = 5, // 0 is Sunday, so 5 is Friday
         .tm_yday  = 12,
         .tm_isdst  = 12
   };

#if 0

void gpio_callback(uint gpio, uint32_t events) {

   if(gpio == PIN_BTND){
      if(ConfigureTime_MODE == ON){
         ConfigureTime_MODE = OFF;
         t.sec = 0;
         rtc_set_datetime(&t);
      }else if(ConfigureTime_MODE == OFF){
         ConfigureTime_MODE = ON; // Configuration Mode Enabled
      }
    }

    if(ConfigureTime_MODE){

      if(gpio == PIN_BTNA){
         if(Selection  == HRS){
            if(t.hour < 23){
               t.hour += 1;}}
         else if(Selection == MINS){
            if(t.min < 59){
               t.min += 1;}}
      }

      if(gpio == PIN_BTNB){
         if(Selection  == HRS){
            if(t.hour > 0 ){
               t.hour -= 1;}}
         else if(Selection == MINS){
            if(t.min > 0){
               t.min -= 1;}}
      }

      if(gpio == PIN_BTNC){
         if(Selection  == HRS){
            Selection = MINS;}
         else if(Selection == MINS){
            Selection = HRS;}
      }

    }else if(ConfigureTime_MODE == OFF){

       if(gpio == PIN_BTNC){
         if(Backlight  == ON){
            gpio_put(PIN_BLK, OFF);
            Backlight = OFF;
         }else if(Backlight == OFF){
            gpio_put(PIN_BLK, ON);
            Backlight = ON;
            }
      }

    }
}
#endif

int main()
{

   int res = gpioInitialise();
   if (res < 0)
   {
      std::cerr << "Error in gpioInitialise(): " << res << std::endl;
      exit(1);
   }

   // Init SPI port
   int spi = spiOpen(0, 10000000, 0);

   // Configure GPIO directions
   gpioSetMode(PIN_CS1, PI_OUTPUT);
   gpioSetMode(PIN_CS2, PI_OUTPUT);
   gpioSetMode(PIN_CS3, PI_OUTPUT);
   gpioSetMode(PIN_CS4, PI_OUTPUT);
   gpioSetMode(PIN_CS5, PI_OUTPUT);
   gpioSetMode(PIN_CS6, PI_OUTPUT);
   gpioSetMode(PIN_DC, PI_OUTPUT);
   gpioSetMode(PIN_RST, PI_OUTPUT);
   gpioSetMode(PIN_BLK, PI_OUTPUT);

   // Select all screens (to configure all at same time)
   gpioWrite(PIN_CS1, 0);
   gpioWrite(PIN_CS2, 0);
   gpioWrite(PIN_CS3, 0);
   gpioWrite(PIN_CS4, 0);
   gpioWrite(PIN_CS5, 0);
   gpioWrite(PIN_CS6, 0);
   gpioWrite(PIN_RST, 1);

   lcdInit(spi, st7735_initSeq);

   gpioWrite(PIN_BLK, ON);
   Backlight = ON;


    lcdStartPx(spi);

    while(1)
    {

       if(ConfigureTime_MODE == OFF)
       {
	 time_t tt = time(nullptr);
	 struct tm *ttt = localtime(&tt);

	 t = *ttt;
       }

       if(t.tm_hour >= 1 & t.tm_hour <= 9){
          lcdDrawNumber(spi,display1,0);
          lcdDrawNumber(spi,display2,t.tm_hour);
       }else if(t.tm_hour >= 10 & t.tm_hour <= 12){
          lcdDrawNumber(spi,display1,1);
          lcdDrawNumber(spi,display2,t.tm_hour-10);
       }else if(t.tm_hour >= 13 & t.tm_hour <= 21){
          lcdDrawNumber(spi,display1,0);
          lcdDrawNumber(spi,display2,t.tm_hour-12);
       }else if(t.tm_hour >= 22 ){
          lcdDrawNumber(spi,display1,1);
          lcdDrawNumber(spi,display2,t.tm_hour-22);
       }else if (t.tm_hour == 0){
          lcdDrawNumber(spi,display1,1);
          lcdDrawNumber(spi,display2,2);
       }

       if(t.tm_min >= 0 & t.tm_min <= 9){
          lcdDrawNumber(spi,display4,0);
          lcdDrawNumber(spi,display5,t.tm_min);
       }else if(t.tm_min >= 10 & t.tm_min <= 19){
          lcdDrawNumber(spi,display4,1);
          lcdDrawNumber(spi,display5,t.tm_min-10);
       }else if(t.tm_min >= 20 & t.tm_min <= 29){
          lcdDrawNumber(spi,display4,2);
          lcdDrawNumber(spi,display5,t.tm_min-20);
       }else if(t.tm_min >= 30 & t.tm_min <= 39 ){
          lcdDrawNumber(spi,display4,3);
          lcdDrawNumber(spi,display5,t.tm_min-30);
       }else if(t.tm_min >= 40 & t.tm_min <= 49 ){
          lcdDrawNumber(spi,display4,4);
          lcdDrawNumber(spi,display5,t.tm_min-40);
       }else if(t.tm_min >= 50 & t.tm_min <= 59 ){
          lcdDrawNumber(spi,display4,5);
          lcdDrawNumber(spi,display5,t.tm_min-50);
       }


       if(t.tm_hour < 12 )
          lcdDrawNumber(spi,display6,AM);
      else
          lcdDrawNumber(spi,display6,PM);


      if(ConfigureTime_MODE == OFF){
         lcdDrawNumber(spi,display3,COLON);
         usleep(500000);
         lcdDrawNumber(spi,display3,SPACE);
         usleep(500000);
      }else{
         if(Selection  == HRS){
            usleep(200000);
            lcdDrawNumber(spi,display1,SPACE);
            lcdDrawNumber(spi,display2,SPACE);
            lcdDrawNumber(spi,display3,COLON);
            usleep(200000);
         }else if(Selection == MINS){
            usleep(200000);
            lcdDrawNumber(spi,display3,COLON);
            lcdDrawNumber(spi,display4,SPACE);
            lcdDrawNumber(spi,display5,SPACE);
            usleep(200000);
         }
      }


      usleep(200000);
    }

   gpioTerminate();

   return 0;
}
