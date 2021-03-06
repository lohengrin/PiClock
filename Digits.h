#pragma once

// All Clock Face themes
enum class Theme {
    SEG14 = 0,
    BINA,
    Digital,
    Flip,
    Ink,
    LixieCyan,
    LixiePurple,
    Matrix,
    NIMO,
    Nixie ,
    VFD,
    Wood,
    Theme_Number
};

// Current theme image pointers
struct Digits {
    Theme theme = Theme::Theme_Number;
    char* d[16]; // Point to corresponding char arrays
};

// Update digits structure with image array pointers according to the theme
void changeDigits(Digits& digits, Theme newTheme);
