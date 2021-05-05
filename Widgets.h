
/*
 * CSCI 261 Final Project : Widgets.h
 *
 * Authors : James Baldwin and Kaleb Chhabra
 *
 *
 * Description : Widgets include buttons, entries and labels that are seen on the main menu class
 *               in the panels for the user to interact with the program. All are able to communicate
 *               with the main menu to produce desired results by user, and let them have control of the
 *               programs actions.
 */

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
    sf::RectangleShape contents, _border;
    sf::Color backgroundColor, textColor;
    sf::Text _title;
    sf::Vector2f _size, _position;
    std::string _action;
    bool hasText;
};

class Button : public Widgets{

public:

    Button(sf::Vector2f size, sf::Vector2f position, bool togglable);

    Button(sf::Vector2f size, sf::Vector2f position, bool togglable, const std::string& LABEL);

    void setPicture(sf::Image picture);

    std::string pressed();

    void draw(sf::RenderWindow& window);

private:

    sf::Image _picture;
    bool _hasPicture, _toggle, _active;
    sf::RectangleShape _button;
    int _count;
};

class Entry : public Widgets{

public:
    Entry(sf::Vector2f size, sf::Vector2f position);

    void draw(sf::RenderWindow& window);

    std::string getEntry();

    void clearEntry();

    void setEntry(char letter);

    bool typedToo() const;

    void select();

    void deselect();

private:

    sf::RectangleShape _entryField;
    sf::Text _entryText;
    std::string _entry;
    bool _typedTo;
};

class Label : public Widgets{

public:

    Label(std::string& text, sf::Vector2f size, sf::Vector2f pos);

    void draw(sf::RenderWindow& window);

};

#endif //IMAGEPROCESSOR_WIDGETS_H
