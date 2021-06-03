cmake_minimum_required(VERSION 3.16)

project(PiClock)
file(GLOB CLOCKIMG "clock*.h")

add_executable(PiClock 
main.cpp 
ST7735.cpp
ST7735.h
${CLOCKIMG}
)

target_link_libraries(PiClock 
pigpio
)

