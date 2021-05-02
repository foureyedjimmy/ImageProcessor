//
// Created by jimmy on 4/14/2021.
//

#include "Panel.h"
#include "Widgets.h"
#include "Picture.h"

Panel::Panel(sf::Vector2f size, sf::Vector2f location) {
    backgroundColor = sf::Color(200,200,200);
    foregroundColor = sf::Color(50,50,50);
    boarderWidth = 1;
    pos = location;
    outsideBox.setSize(size);
    outsideBox.setPosition(location);
    outsideBox.setFillColor(backgroundColor);
    outsideBox.setOutlineThickness(boarderWidth);
    outsideBox.setOutlineColor(foregroundColor);
}

std::string Panel::getEnteredData() {
    return lastEntry;
}

void Panel::addButton(const std::string& NAME, sf::Vector2f size, sf::Vector2f pos, bool toggle) {
    Button buttonNew(size, {pos.x + this -> pos.x, pos.y + this -> pos.y}, toggle, NAME);
    buttonNew.setFont(font);
    buttons.push_back(buttonNew);
}

std::string Panel::checkButtons(sf::Vector2i &pos) {
    sf::Vector2f floatPos(pos.x, pos.y);
    for(Button& button: buttons){
        if(button.inBounds(floatPos)){
            return button.pressed();
        }
    }
    return "";
}

void Panel::setFont(sf::Font& font){
    this -> font = font;
    for(Button& button: buttons){
        button.setFont(font);
    }
    for(Entry& entry: entries){
        entry.setFont(font);
    }
    for(Label& label : labels){
        label.setFont(font);
    }
}

void Panel::addSlider(sf::Vector2f range, sf::Vector2u size, sf::Vector2u pos) {

}

void Panel::addEntry(sf::Vector2f size, sf::Vector2f position) {
    Entry entry(size,{float(pos.x + position.x), float(pos.y + position.y)});
    entry.setFont(font);
    entries.push_back(entry);
}

void Panel::setVisible(bool vis){
    visible = vis;
}

bool Panel::isVisible() const {
    return visible;
}

bool Panel::checkEntries(sf::Vector2i& mousePos){
    sf::Vector2f floatPos(mousePos.x, mousePos.y);
    for(Entry& entry : entries){
        if(entry.inBounds(floatPos)){
            entry.select();
            return true;
        }else{
            entry.deselect();
        }
    }
    return false;
}

bool Panel::getActiveEntry(char letter){
    for(Entry& entry : entries){
        if(entry.typedToo()){
            if(letter == 13){
                entry.clearEntry();
            }else {
                entry.setEntry(letter);
                lastEntry = entry.getEntry();
            }
            return true;
        }
    }
    return false;
}

void Panel::setName(const std::string& NEW_NAME){
    name = NEW_NAME;
}

std::string Panel::getName() const{
    return name;
}

sf::Vector2f Panel::getPos() {
    return pos;
}

void Panel::draw(sf::RenderWindow &window) {
    if(visible) {
        window.draw(outsideBox);
        for (Button &button : buttons) {
            button.draw(window);
        }
        for (Entry entry : entries) {
            entry.draw(window);
        }
        for (Label &label : labels){
            label.draw(window);
        }
    }

}


void Panel::addLabel(std::string name, sf::Vector2f position, sf::Vector2f size){
    Label label(name, size, {float(pos.x + position.x), float(pos.y + position.y)});
    label.setFont(font);
    labels.push_back(label);
}
