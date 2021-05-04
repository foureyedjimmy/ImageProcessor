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

    void doAction(std::string& manip);

    void setFrameRate(int framerate);



private:

    sf::Vector2u windowSize;
    std::string entryPanel;
    sf::Event event;
    sf::Vector2i pos, mousePos, relPos;
    sf::Font font;
    Picture picture;
    float scaleFactor, variable1, variable2;
    sf::Texture texture, textureOrig, RGBhistText, histText;
    sf::Sprite mainImage, origImage, RGBhist, hist;
    std::vector<Panel> panels;
    std::string data, lastManip, selectedPanel;
    bool locked, histVis, RGBhistVis;
    int req;

    void getVariable(std::string& name);

    void startSave();
    void save();

    void startOpen();
    void open();
    void check(sf::RenderWindow& window);
    void cancelProcess();
    void updateHists();
    void updatePicture();
    void updateWinSize(sf::RenderWindow &window);

    void interpretData();
};


#endif //IMAGEPROCESSOR_MAINMENU_H
