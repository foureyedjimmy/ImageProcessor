/*
 * CSCI 261 Final Project : main.cpp
 *
 * Authors: James Baldwin and Kaleb Chhabra
 *
 * Description: Entry point for the gui. creates the render window, and a allows the fonts to be loaded as well as
 *              the default image
 *
 */



#include <SFML/Graphics.hpp>
#include <iostream>
#include "MainMenu.h"
using namespace sf;
int main() {
    RenderWindow window(VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
                        "Super Epic Image Processor", Style::Close | Style::Titlebar);
    MainMenu menu(window);
    sf::Font font;
    font.loadFromFile("arial.ttf");
    menu.loadFont(font);
    menu.loadPic("test.png");
    menu.showPic();

    menu.loop(window);

    return 0;
}
