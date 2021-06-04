#pragma once

struct Digits {
    char* zero_Theme = nullptr;
    char* one_Theme = nullptr;
    char* two_Theme = nullptr;
    char* three_Theme = nullptr;
    char* four_Theme = nullptr;
    char* five_Theme = nullptr;
    char* six_Theme = nullptr;
    char* seven_Theme = nullptr;
    char* eight_Theme = nullptr;
    char* nine_Theme = nullptr;
    char* colon_Theme = nullptr;
    char* slash_Theme = nullptr;
    char* space_Theme = nullptr;
    char* am_Theme = nullptr;
    char* pm_Theme = nullptr;
    char* heart_Theme = nullptr;
    char* amAlarm_Theme = nullptr;
    char* pmAlarm_Theme = nullptr;
};

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

void changeDigits(Digits& digits, Theme newTheme);