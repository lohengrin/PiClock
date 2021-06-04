#include "Digits.h"

#include "clockDigital.h"
#include "clockFlip.h"
#include "clockMatrix.h"
#include "clockVFD.h"
#include "clockInk.h"
#include "clockWood.h"
#include "clockNixie.h"

#define SETTHEME(THEME) \
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
digits.theme = Theme::THEME;

void changeDigits(Digits& digits, Theme newTheme)
{
    switch (newTheme)
    {
        case Theme::Digital:
            SETTHEME(Digital);
            break;
        case Theme::Flip:
            SETTHEME(Flip);
            break;
        case Theme::Ink:
            SETTHEME(Ink);
            break;
        case Theme::Matrix:
            SETTHEME(Matrix);
            break;
        case Theme::Nixie:
            SETTHEME(Nixie);
            break;
        case Theme::VFD:
            SETTHEME(VFD);
            break;
        case Theme::Wood:
            SETTHEME(Wood);
            break;
        default:
            SETTHEME(VFD);
            break;
    };
}
