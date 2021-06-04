#pragma once

enum class Theme {
    Digital,
    Flip,
    Ink,
    Matrix,
    Nixie,
    VFD,
    Wood,
    Theme_Number
};

struct Digits {
    Theme theme = Theme::Theme_Number;
    char* d[16];
};

void changeDigits(Digits& digits, Theme newTheme);