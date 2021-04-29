cmake_minimum_required(VERSION 3.17)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 14)

include_directories("C:/mingw64/include")

add_executable(ImageProcessor main.cpp MainMenu.cpp MainMenu.h Picture.cpp Picture.h Panel.cpp Panel.h Widgets.cpp Widgets.h)

target_link_directories(ImageProcessor PUBLIC "C:/mingw64/lib")


target_link_libraries(ImageProcessor sfml-graphics sfml-main sfml-window sfml-system)
