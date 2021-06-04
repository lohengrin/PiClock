#include "Digits.h"

#include "clockDigital.h"
#include "clockFlip.h"
#include "clockMatrix.h"
#include "clockVFD.h"
#include "clockInk.h"
#include "clockWood.h"
#include "clockNixie.h"

#define SETTHEME(THEME) \
digits.zero_Theme = zero_##THEME; \
digits.one_Theme = one_##THEME; \
digits.two_Theme = two_##THEME; \
digits.three_Theme = three_##THEME; \
digits.four_Theme = four_##THEME; \
digits.five_Theme = five_##THEME; \
digits.six_Theme = six_##THEME; \
digits.seven_Theme = seven_##THEME; \
digits.eight_Theme = eight_##THEME; \
digits.nine_Theme = nine_##THEME; \
digits.colon_Theme = colon_##THEME; \
digits.slash_Theme = slash_##THEME; \
digits.space_Theme = space_##THEME; \
digits.am_Theme = am_##THEME; \
digits.pm_Theme = pm_##THEME; \
digits.heart_Theme = heart_##THEME;

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
