cmake_minimum_required(VERSION 3.16)

project(DisplayAClock)
file(GLOB SOURCES "clock*.h")

add_executable(DisplayAClock 
main.cpp 
ST7735.cpp
${SOURCES})

target_link_libraries(DisplayAClock 
pigpio
)

