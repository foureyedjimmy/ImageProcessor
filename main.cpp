#include <SFML/Graphics.hpp>
#include <iostream>
#include "MainMenu.h"
using namespace sf;
int main() {
    RenderWindow window(VideoMode(720, 500), "Image thing", Style::Close | Style::Titlebar);
    MainMenu menu(window);
    sf::Font font;
    font.loadFromFile("..\\arial.ttf");
    menu.loadFont(font);
    menu.loadPic("..\\test.png");
    menu.showPic();

    menu.loop(window);
}
