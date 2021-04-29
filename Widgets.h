//
// Created by jimmy on 4/14/2021.
//

#ifndef IMAGEPROCESSOR_WIDGETS_H
#define IMAGEPROCESSOR_WIDGETS_H


#include <SFML/Graphics.hpp>

class Widgets {
public:
    Widgets(sf::Vector2f size, sf::Vector2f position);

    void setFont(sf::Font& font);

    void setTextColor(sf::Color color);

    void setColor(sf::Color color);

    bool inBounds(sf::Vector2f& point) const;

    void setTitle(const std::string& title);

    void resize(sf::Vector2f& newSize);

protected:

    std::string getAction();
    sf::RectangleShape contents, border;
    sf::Color backgroundColor, textColor;
    sf::Text title;
    sf::Vector2f size, position;
    std::string action;
    bool hasText;
};

class Button : public Widgets{
public:
    Button(sf::Vector2f size, sf::Vector2f position, bool Togglable);
    Button(sf::Vector2f size, sf::Vector2f position, bool togglable, const std::string& LABEL);

    void setPicture(sf::Image picture);

    void setHighlightColor(sf::Color color);

    std::string pressed();

    void draw(sf::RenderWindow& window);

private:
    sf::Color highlightColor;
    sf::Image picture;
    bool hasPicture, toggle, active;
    sf::RectangleShape button;
    sf::Text label;
};

class Slider : public Widgets{
public:
    Slider(sf::Vector2u size, sf::Vector2u position);

    void setRange(sf::Vector2f range);

    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape sliderBar, selectionWindow;
    sf::CircleShape selector;
    sf::Text currValue;
    sf::Vector2f sliderPos, range;
};

class Entry : Widgets{

public:
    Entry(sf::Vector2u size, sf::Vector2u position);

    void draw(sf::RenderWindow& window);

    std::string getEntry();

private:
    sf::RectangleShape entryField, entrySelectionInd;
    sf::Text entryText;
    std::string entry;

};

#endif //IMAGEPROCESSOR_WIDGETS_H
