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

void Panel::addButton(std::string& name, sf::Vector2f size, sf::Vector2f pos, bool toggle) {
    Button buttonNew(size, {pos.x, pos.y + this -> pos.y}, toggle, name);
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
}

void Panel::addSlider(sf::Vector2f range, sf::Vector2u size, sf::Vector2u pos) {

}

void Panel::addEntry(sf::Vector2f size, sf::Vector2f position) {
    Entry entry(size,{float(pos.x + position.x), float(pos.y + position.y)});
    entry.setFont(font);
    entries.push_back(entry);
}

void Panel::checkEntries(sf::Vector2i& mousePos){
    sf::Vector2f floatPos(mousePos.x, mousePos.y);
    for(Entry& entry : entries){
        if(entry.inBounds(floatPos)){

            entry.select();
        }else{
            entry.deselect();
        }
    }
}

//void Panel::getActiveEntry(){
//    for()
//}

void Panel::draw(sf::RenderWindow &window) {
    window.draw(outsideBox);
//    buttons[0].draw(window);
    for(Button& button : buttons){
        button.draw(window);
    }
//    for(Slider slider: sliders){
//        slider.draw(window);
//    }
    for(Entry entry : entries){
        entry.draw(window);
    }

}

Entry& Panel::getEntry() {
    for(Entry& entry: entries){
        if(entry.typedToo()){
            return entry;
        }
    }
    return entries[0];
}

void Panel::resize(float x, float y) {

}
