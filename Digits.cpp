#include "Digits.h"

#include "Clock-Faces/Clock 14SEG/clock14SEG.h"
#include "Clock-Faces/Clock BINA/clockBINA.h"
#include "Clock-Faces/Clock Digital/clockDigital.h"
#include "Clock-Faces/Clock Flip/clockFlip.h"
#include "Clock-Faces/Clock Ink/clockInk.h"
#include "Clock-Faces/Clock Lixie Cyan/clockLixieCyan.h"
#include "Clock-Faces/Clock Lixie Purple/clockLixiePurple.h"
#include "Clock-Faces/Clock Matrix/clockMatrix.h"
#include "Clock-Faces/Clock NIMO/clockNIMO.h"
#include "Clock-Faces/Clock Nixie/clockNixie.h"
#include "Clock-Faces/Clock VFD/clockVFD.h"
#include "Clock-Faces/Clock Wood/clockWood.h"

#define SETTHEME(THEME) \
case Theme::THEME: \
digits.d[0] = zero_##THEME; \
digits.d[1] = one_##THEME; \
digits.d[2] = two_##THEME; \
digits.d[3] = three_##THEME; \
digits.d[4] = four_##THEME; \
digits.d[5] = five_##THEME; \
digits.d[6] = six_##THEME; \
digits.d[7] = seven_##THEME; \
digits.d[8] = eight_##THEME; \
digits.d[9] = nine_##THEME; \
digits.d[10] = colon_##THEME; \
digits.d[11] = slash_##THEME; \
digits.d[12] = space_##THEME; \
digits.d[13] = am_##THEME; \
digits.d[14] = pm_##THEME; \
digits.d[15] = heart_##THEME; \
digits.theme = Theme::THEME; \
break;


void changeDigits(Digits& digits, Theme newTheme)
{
    if (digits.theme == newTheme)
        return; // nothing to do
        
    switch (newTheme)
    {
        default:
        SETTHEME(SEG14);
        SETTHEME(BINA);
        SETTHEME(Digital);
        SETTHEME(Flip);
        SETTHEME(Ink);
        SETTHEME(LixieCyan);
        SETTHEME(LixiePurple);
        SETTHEME(Matrix);
        SETTHEME(NIMO);
        SETTHEME(Nixie);
        SETTHEME(VFD);
        SETTHEME(Wood);
    };
}
