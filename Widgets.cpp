/* CSCI 261 Final Project : Widgets.cpp
 *
 * Authors: James Baldwin and Kaleb Chhabra
 *
 * Description: definitions for widgets superclass
 *              and classes which inherit from widgets
 *
 */

#include "Widgets.h"
#include <iostream>

/////////////////////////////////////////////////////////////////
///Widgits Constructor
///     used to initialize widgit super class
/// \param size - size of the widgit
/// \param position - _position of widgit on the screen
////////////////////////////////////////////////////

Widgets::Widgets(sf::Vector2f size, sf::Vector2f position) {
    this -> _size = size;
    this -> _position = position;

    // set up _title size and location
    _title.setOrigin(size.x / 2.1, 0);
    _title.setPosition(position.x + size.x / 2, position.y);
    float charSize = size.y/1.5;
    _title.setCharacterSize(charSize);
    _title.setFillColor(sf::Color::Black);
}

///////////////////////////////
///set _font:
///     sets the widgets _font
///
/// \param sfml _font object
//////////////////////////////

void Widgets::setFont(sf::Font& font) {
    _title.setFont(font);
}


////////////////////////////////////
///set Text Color:
///     sets the color of the text
/// \param sfml color object
/////////////////////////////////////

void Widgets::setTextColor(sf::Color color) {
    _title.setFillColor(color);
}

////////////////////////////////////////////////
///Set Color:
///     sets the background color of the widget
/// \param color
///////////////////////////////////////////////

void Widgets::setColor(sf::Color color) {
    _border.setFillColor(color);
}

////////////////////////////////////////////////////////
///In Bounds:
///     checks to see if a point is in a bounding box
/// \param point - sfml vector of point
/// \return boolean of whether its in the widget or not
///////////////////////////////////////////////////////

bool Widgets::inBounds(sf::Vector2f &point) const {
    if(point.x > _position.x && point.x < _position.x + _size.x && point.y > _position.y && point.y < _position.y + _size.y){
        return true;
    }else{
        return false;
    }
}

////////////////////////////////////////////////
///resize:
///     used to resize the widgit
///
/// \param newSize - sfml vector of new size
///////////////////////////////////////////////

void Widgets::resize(sf::Vector2f& newSize) {
    _size.x = newSize.x;
    _size.y = newSize.y;
    _border.setSize(_size);
}

////////////////////////////////////////////////////
///get Action:
///     gets the action associated with that widget
/// \return string of action to preform
////////////////////////////////////////////////////

std::string Widgets::getAction() {
    return _action;
}

////////////////////////////////////////////////////
///set title:
///     sets the string of the text in the widget
///
/// \param NAME - string to set text to
///////////////////////////////////////////////////

void Widgets::setTitle(const std::string& NAME) {
    _title.setString(NAME);
}

/////////////////////////////////////////////////////
///Button Constructor (basic):
///     used to create the _button widget (inherits
///     from widget class)
///
/// \param size - size of _button
/// \param position - position of _button
/// \param togglable - whether its toggleable or not
/////////////////////////////////////////////////////

Button::Button(sf::Vector2f size, sf::Vector2f position, bool togglable) : Widgets(size, position){
    _toggle = togglable;
    _hasPicture = false;
    _title.setString("test");

    //set button rects stuff
    _button.setSize(size);
    _button.setPosition(position);
    _button.setFillColor({200, 200, 200});
    _button.setOutlineColor({100, 100, 100});
    _button.setOutlineThickness(3);
}

/////////////////////////////////////////////////////////////////
///set Picture:
///     used to set a _picture in a button (not yet implemented)
/// \param picture - sfml image to put on button
/////////////////////////////////////////////////////////////////

void Button::setPicture(sf::Image picture) {
    _hasPicture = true;
    _picture = picture;
}


///////////////////////////////////////////////////
///Pressed:
///     handles the pressing of a button
///
/// \return the action associated with the button
//////////////////////////////////////////////////

std::string Button::pressed() {
    if(_toggle){
        if(_active){
            _active = false;
        }else{
            _active = true;
        }
    }else{
        _active = true;
        _count = 0;
    }
    return _action;
}

///////////////////////////////////////////////////////////
///Draw:
///     draws button onto window
///
/// \param window - sfml Render Window to draw button on
///////////////////////////////////////////////////////////
void Button::draw(sf::RenderWindow &window) {
    // if _active set fill to dark color
    if(_active){
        _button.setFillColor({200, 200, 200});
    }else{
        // else set to normal color (white)
        _button.setFillColor({255, 255, 255});
    }

    //draws button
    window.draw(_button);
    window.draw(_title);

    // checks the non-toggleable _buttons
    if(!_toggle && _active){
        _count ++;
        if(_count > 5){
            _active = false;
        }
    }
}

/////////////////////////////////////////////////////////////
///Button Constructor with text:
///     sets up button object with text given to it
///
/// \param size - button size
/// \param position - position of button
/// \param togglable - whether its toggleable or not
/// \param LABEL - text to be written on button (and action)
///////////////////////////////////////////////////////////////

Button::Button(sf::Vector2f size, sf::Vector2f position, bool togglable, const std::string &LABEL) : Widgets(size, position) {

    // sets text
    _title.setString(sf::String(LABEL));
    _title.setFillColor(sf::Color::Black);

    // sets other attributes required
    _toggle = togglable;
    _hasPicture = false;
    _active = false;
    _action = LABEL;

    // sets up button rect
    _button.setSize(size);
    _button.setPosition(position );
    _button.setFillColor({230, 230, 230});
    _button.setOutlineColor({100, 100, 100});
    _button.setOutlineThickness(1);
}

////////////////////////////////////////////////////////
///Entry Constructor:
///     constructor for _entry object
/// \param size - size of _entry
/// \param position - position of _entry on the screen
////////////////////////////////////////////////////////


Entry::Entry(sf::Vector2f size, sf::Vector2f position) : Widgets(size, position) {
    sf::Vector2f entrySize(size.x - 5, size.y - 5);

    // initialzes _entry field
    _entryField.setSize(entrySize);
    _entryField.setPosition(position.x + 2.5, position.y + 2.5);
    _entryField.setFillColor(sf::Color::White);
    _entryField.setOutlineColor(sf::Color::Black);
    _entryField.setOutlineThickness(1);

}

////////////////////////////////////////////////////
///Draw:
///     draws _entry on the screen
///
/// \param window - sfml Render Window to draw to
////////////////////////////////////////////////////

void Entry::draw(sf::RenderWindow &window) {
    window.draw(_entryField);
    window.draw(_title);
}

/////////////////////////////////////////////////
///Select:
///     makes the _entry feild selected on GUI
////////////////////////////////////////////////

void Entry::select() {

    //thickens outline
    _entryField.setOutlineThickness(3);

    // allows text to be typed to it
    _typedTo = true;
}

///////////////////////////////////////////////
///Deselect:
///     Deselects the _entry field in the GUI
///////////////////////////////////////////////

void Entry::deselect() {
    _entryField.setOutlineThickness(1);
    _typedTo = false;
}

////////////////////////////////////////////////
///Typed Too:
///     gets whether the Entry is active or not
///
/// \return active boolean
///////////////////////////////////////////////

bool Entry::typedToo() const{
    return _typedTo;
}

///////////////////////////////////////////////
///Clear Entry:
///     Clears the _entry field
///////////////////////////////////////////////

void Entry::clearEntry() {
    _entry = "";
    _title.setString(_entry);
}

/////////////////////////////////////////////
///Set Entry:
///     used to update text in entry field
///
/// \param text - character to add into entry
/////////////////////////////////////////////

void Entry::setEntry(char text){
    // if deleted is typed
    if(text==8) {
        // if the entry isnt empty
        if(!_entry.empty()){
            // delete the last thing in entry
            _entry.pop_back();
        }
    }else{
        // else add character to entry
        _entry += text;
    }
    // update the text displayed in box
    _title.setString(_entry);

}

///////////////////////////////////////////
///Get Entry:
///     getter for the entry string
///
/// \return entry string
//////////////////////////////////////////

std::string Entry::getEntry() {
    return _entry;
}

///////////////////////////////////////////////////
///Label Constructor:
///     sets basic text label to be drawn on window
///
/// \param text - text to be shown
/// \param size - size of widget
/// \param pos - position on screen
/////////////////////////////////////////////////////

Label::Label(std::string &text, sf::Vector2f size, sf::Vector2f pos) : Widgets(size, pos){
    _title.setString(text);
}

////////////////////////////////////////////////////
///Draw:
///     draws label on screen
/// \param window - sfml window to draw to
////////////////////////////////////////////////////

void Label::draw(sf::RenderWindow& window){
    window.draw(_title);
}