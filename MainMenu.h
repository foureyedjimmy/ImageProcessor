/*
 * CSCI 261 Final Project : MainMenu.h
 *
 * Authors : James Baldwin and Kaleb Chhabra
 *
 * Description: Main Menu for the GUI, contains all the panels, and runs the main loop as well as handles all user
 *              Input and reacts accordingly
 */

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

    void doAction(std::string& manip);



private:

    sf::Vector2u _windowSize;
    std::string _entryPanel;
    sf::Event _event;
    sf::Vector2i _pos, _mousePos;
    sf::Font _font;
    Picture _picture;
    float _scaleFactor, _variable1, _variable2;
    sf::Texture _texture, _textureOrig, _RGBhistText, _histText;
    sf::Sprite _mainImage, _origImage, _RGBhist, _hist;
    std::vector<Panel> _panels;
    std::string _data, _lastManip, _selectedPanel;
    bool _locked, _histVis, _RGBhistVis;
    int _req;

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
