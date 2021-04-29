//
// Created by jimmy on 4/10/2021.
//

#ifndef IMAGEPROCESSOR_MAINMENU_H
#define IMAGEPROCESSOR_MAINMENU_H
#include<iostream>
#include "Picture.h"
#include "Panel.h"
#include <SFML/Graphics.hpp>

class MainMenu {
public:

    MainMenu();

    MainMenu(sf::RenderWindow &window);

    void loop(sf::RenderWindow &window);

    void loadPic(std::string filename);

    void showPic();

    void loadFont(sf::Font& font);

    void createPhoto(std::string);

    void startManip(std::string& manip);

    void setFrameRate(int framerate);



private:

    sf::Vector2u windowSize;
    sf::Keyboard keys;
    sf::Mouse mouse;
    sf::Event event;
    sf::Vector2i pos, mousePos, relPos;
    sf::Font font;
    Picture picture;
    float scaleFactor;
    sf::Texture texture;
    sf::Sprite mainImage;
    void create(sf::RenderWindow &window);
    std::vector<Panel> panels;

    void updateWinSize(sf::RenderWindow &window);
};


#endif //IMAGEPROCESSOR_MAINMENU_H
