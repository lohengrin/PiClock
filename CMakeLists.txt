cmake_minimum_required(VERSION 3.16)

project(PiClock)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### optional display weather image generation with QT and Openweather
option(WITH_QWEATHER "Build with QT based OpenWeather display" OFF)
if (WITH_QWEATHER)
    SET(OWAPPID "" CACHE STRING "Your OpenWeather appId to query weather informations")
    SET(OWLOCATION "Paris,FR" CACHE STRING "OpenWeather location for grabbig weather")
    
    find_package(Qt5 COMPONENTS Core Network Gui REQUIRED)

    SET(QWSOURCES
        QWeather/DisplayImageGenerator.h
        QWeather/DisplayImageGenerator.cpp
        QWeather/OpenWeatherGrabber.h
        QWeather/OpenWeatherGrabber.cpp
    )
endif(WITH_QWEATHER)

# Normal Clock part
file(GLOB CLOCKIMG "Clock-Faces/*/clock*.h")

add_executable(PiClock 
    main.cpp 
    ST7735.cpp
    ST7735.h
    Digits.cpp
    Digits.h
    ${CLOCKIMG}
    ${QWSOURCES} # Empty if option not activated
)

target_link_libraries(PiClock 
    pigpio
)

# Additionnal dependencies when using QT based weather display
if (WITH_QWEATHER)
    add_compile_definitions(PiClock OWAPPID="${OWAPPID}" OWLOCATION="${OWLOCATION}" WITH_QWEATHER)

    target_link_libraries(PiClock 
        Qt5::Core
        Qt5::Gui
        Qt5::Network
    )
endif(WITH_QWEATHER)
