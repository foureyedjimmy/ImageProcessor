/*
 * CSCI 261 Final Project: Panels.h
 *
 * Authors: James Baldwin and Kaleb Chhabra
 *
 * Description: Panel is an object to put widgets onto in the Main Menu GUI for the user to interface with
 *              to effect the picture class.
 */

#ifndef IMAGEPROCESSOR_PANEL_H
#define IMAGEPROCESSOR_PANEL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Widgets.h"


class Panel {
public:
    Panel(sf::Vector2f size, sf::Vector2f location);

    void addButton(const std::string& NAME, sf::Vector2f size, sf::Vector2f pos, bool toggle);

    void addEntry(sf::Vector2f size, sf::Vector2f pos);

    void draw(sf::RenderWindow& window);

    std::string checkButtons(sf::Vector2i& pos);

    void setFont(sf::Font& font);

    void addLabel(std::string name, sf::Vector2f size, sf::Vector2f pos);

    bool getActiveEntry(char letter);

    std::string getEnteredData();

    bool checkEntries(sf::Vector2i& mousePos);

    sf::Vector2f getPos();

    void setName(const std::string& NEW_NAME);

    std::string getName() const;

    void setVisible(bool vis);

    bool isVisible() const;

    void setLabelText(int index, std::string text);

private:
    std::vector<Button> _buttons;;
    std::vector<Entry> _entries;
    std::vector<Label> _labels;
    bool _visible;
    sf::Font _font;
    sf::Vector2f _pos;
    sf::RectangleShape _outsideBox;
    float _boarderWidth;
    sf::Color _backgroundColor, _foregroundColor;
    std::string _lastEntry, _name;
};


#endif //IMAGEPROCESSOR_PANEL_H
