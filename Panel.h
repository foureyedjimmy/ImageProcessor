//
// Created by jimmy on 4/14/2021.
//

#ifndef IMAGEPROCESSOR_PANEL_H
#define IMAGEPROCESSOR_PANEL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Widgets.h"


class Panel {
public:
    Panel(sf::Vector2f size, sf::Vector2f location);

    void addButton(std::string& name, sf::Vector2f size, sf::Vector2f pos, bool toggle);

    void addSlider(sf::Vector2f range, sf::Vector2u size, sf::Vector2u pos);

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



private:
    std::vector<Button> buttons;
    std::vector<Slider> sliders;
    std::vector<Entry> entries;
    std::vector<Label> labels;
    bool visible;
    sf::Font font;
    sf::Vector2f pos;
    sf::RectangleShape outsideBox, insideBox;
    float boarderWidth;
    sf::Color backgroundColor, foregroundColor;
    std::string lastEntry, name;
};


#endif //IMAGEPROCESSOR_PANEL_H
