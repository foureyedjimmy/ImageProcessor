//
// Created by jimmy on 4/14/2021.
//

#include "Widgets.h"
#include <iostream>

Widgets::Widgets(sf::Vector2f size, sf::Vector2f position) {
    this -> size = size;
    this -> position = position;
    title.setOrigin(size.x/2.1, 0);
    title.setPosition(position.x + size.x/2, position.y);
    float charSize = size.y/1.5;
    title.setCharacterSize(charSize);
    title.setFillColor(sf::Color::Black);
}

void Widgets::setFont(sf::Font& font) {
    title.setFont(font);
}

void Widgets::setTextColor(sf::Color color) {
    textColor = color;
    title.setFillColor(color);
}

void Widgets::setColor(sf::Color color) {
    backgroundColor = color;
}

bool Widgets::inBounds(sf::Vector2f &point) const {
    if(point.x > position.x && point.x < position.x + size.x && point.y > position.y && point.y < position.y + size.y){
        return true;
    }else{
        return false;
    }
}

void Widgets::resize(sf::Vector2f& newSize) {
    size.x = newSize.x;
    size.y = newSize.y;
}

std::string Widgets::getAction() {
    return action;
}

void Widgets::setTitle(const std::string& NAME) {
    title.setString(NAME);
}

Button::Button(sf::Vector2f size, sf::Vector2f position, bool Togglable) : Widgets(size, position){
    toggle = Togglable;
    hasPicture = false;
    title.setString("test");
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor({200,200,200});
    button.setOutlineColor({100,100,100});
    button.setOutlineThickness(3);
}

void Button::setPicture(sf::Image picture) {
    hasPicture = true;
    this -> picture = picture;
}

void Button::setHighlightColor(sf::Color color) {
    highlightColor = color;
}

std::string Button::pressed() {
    if(toggle){
        if(active){
            active = false;
        }else{
            active = true;
        }
    }else{
        active = true;
        count = 0;
    }
    return getAction();
}

void Button::draw(sf::RenderWindow &window) {
    if(active){
        button.setFillColor({200,200,200});
    }else{
        button.setFillColor({255,255,255});
    }
    window.draw(button);
    window.draw(title);
    if(!toggle && active){
        count ++;
        if(count > 5){
            active = false;
        }
    }
}

Button::Button(sf::Vector2f size, sf::Vector2f position, bool togglable, const std::string &LABEL) : Widgets(size, position) {
    title.setString(sf::String(LABEL));
    title.setFillColor(sf::Color::Black);
    toggle = togglable;
    hasPicture = false;
    active = false;
    action = LABEL;
    button.setSize(size);
    button.setPosition(position );
    button.setFillColor({230,230,230});
    button.setOutlineColor({100,100,100});
    button.setOutlineThickness(1);
    highlightColor = sf::Color(255,255,255);
}

Entry::Entry(sf::Vector2f size, sf::Vector2f position) : Widgets(size, position) {
    sf::Vector2f entrySize(size.x - 5, size.y - 5);
    entryField.setSize(entrySize);
    entryField.setPosition(position.x + 2.5, position.y + 2.5);
    entryField.setFillColor(sf::Color::White);
    entryField.setOutlineColor(sf::Color::Black);
    entryField.setOutlineThickness(1);

}

void Entry::draw(sf::RenderWindow &window) {
    window.draw(entryField);
    window.draw(title);
}

void Entry::select() {
    entryField.setOutlineThickness(3);
    typedTo = true;
}

void Entry::deselect() {
    entryField.setOutlineThickness(1);
    typedTo = false;
}

bool Entry::typedToo() const{
    return typedTo;
}


void Entry::clearEntry() {
    entry = "";
    title.setString(entry);
}

void Entry::setEntry(char text){
    if(text==8) {
        if(!entry.empty()){
            entry.pop_back();
        }
    }else{
        entry += text;
    }
    title.setString(entry);

}
std::string Entry::getEntry() {
    return entry;
}


Label::Label(std::string &text, sf::Vector2f size, sf::Vector2f pos) : Widgets(size, pos){
    float charSize = size.y/1.5;
    title.setCharacterSize(charSize);
    title.setString(text);
    title.setFillColor({0,0,0});
    title.setString(text);
    std::cout << title.getPosition().x << std::endl;
}

void Label::draw(sf::RenderWindow& window){
    window.draw(title);
}