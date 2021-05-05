/* CSCI 261 Final Project: Panel.cpp
 *
 * Authors: James Baldwin and Kaleb Chhabra
 *
 * Description: Panel class definitions - panels on Main Menu class
 */

#include "Panel.h"
#include "Widgets.h"
#include "Picture.h"

//////////////////////////////////////////////////////////////////////
///Panel Constructor:
///     constructor to create panel of size size at position position
///
/// \param size - size of panel
/// \param location - location of pannel
//////////////////////////////////////////////////////////////////////


Panel::Panel(sf::Vector2f size, sf::Vector2f location) {

    // set up needed variables
    _backgroundColor = sf::Color(200, 200, 200);
    _foregroundColor = sf::Color(50, 50, 50);
    _boarderWidth = 1;
    _pos = location;

    //set up outside box
    _outsideBox.setSize(size);
    _outsideBox.setPosition(location);
    _outsideBox.setFillColor(_backgroundColor);
    _outsideBox.setOutlineThickness(_boarderWidth);
    _outsideBox.setOutlineColor(_foregroundColor);
}

/////////////////////////////////////////////////////////
///Get Entered Data:
///     returns the last entered data in pannel
///
/// \return entry string
/////////////////////////////////////////////////////////

std::string Panel::getEnteredData() {
    return _lastEntry;
}

////////////////////////////////////////////////////////
///Add Button:
///     Adds a Button onto the panel
///
/// \param NAME - text to show on button
/// \param size - size of button
/// \param pos - position relative to panel
/// \param toggle - whether it needs to be toggleable
////////////////////////////////////////////////////////

void Panel::addButton(const std::string& NAME, sf::Vector2f size, sf::Vector2f pos, bool toggle) {

    // builds new button
    Button buttonNew(size, {pos.x + _pos.x, pos.y + _pos.y}, toggle, NAME);
    buttonNew.setFont(_font);

    // puts into buttons
    _buttons.push_back(buttonNew);
}

///////////////////////////////////////////////////////
///Check Buttons:
///     Checks all buttons to find active and then
///     presses them if pressed
///
/// \param pos - mouse position
/// \return the action of the button that's pressed
//////////////////////////////////////////////////////

std::string Panel::checkButtons(sf::Vector2i &pos) {
    //converts to float vector
    sf::Vector2f floatPos(pos.x, pos.y);
    for(Button& button: _buttons){

        //checks if position is in button
        if(button.inBounds(floatPos)){

            //returns actions
            return button.pressed();
        }
    }
    return "";
}

////////////////////////////////////////////////////
/// set Font:
///     sets font for the panel as well as all
///     widgets on the panel
///
/// \param font - sfml font object
//////////////////////////////////////////////////

void Panel::setFont(sf::Font& font){
    _font = font;
    for(Button& button: _buttons){
        button.setFont(font);
    }
    for(Entry& entry: _entries){
        entry.setFont(font);
    }
    for(Label& label : _labels){
        label.setFont(font);
    }
}

/////////////////////////////////////////////////////////
///Add Entry:
///     adds entry onto panel
///
/// \param size - size of entry
/// \param position - position relative to position of
///         panel
////////////////////////////////////////////////////////

void Panel::addEntry(sf::Vector2f size, sf::Vector2f position) {
    Entry entry(size,{float(_pos.x + position.x), float(_pos.y + position.y)});
    entry.setFont(_font);
    _entries.push_back(entry);
}

/////////////////////////////////////////////////////////
///Set Visible:
///     sets whether the panel is drawn or not
///
/// \param vis - boolean value of whether or the panel
///         is drawn.
///////////////////////////////////////////////////////

void Panel::setVisible(bool vis){
    _visible = vis;
}

////////////////////////////////////////////////////////
///Is Visible:
///     used to tell if a panel is _visible or not
///
/// \return whether the panel is _visible
///////////////////////////////////////////////////////

bool Panel::isVisible() const {
    return _visible;
}

/////////////////////////////////////////////////////
///Check Entries:
///     Checks to see if any entries are at the mouses
///     position, if so it selects the entry to be typed
///     in.
///
/// \param mousePos - position of mouse click
/// \return whether the panel contains an entry thats
///         clicked.
///////////////////////////////////////////////////////

bool Panel::checkEntries(sf::Vector2i& mousePos){
    // convert to float
    sf::Vector2f floatPos(mousePos.x, mousePos.y);
    for(Entry& entry : _entries){

        // if click was inbounds
        if(entry.inBounds(floatPos)){
            // select that entry
            entry.select();
            return true;
        }else{
            // if not clicked deselect that entry
            entry.deselect();
        }
    }

    // if none are clicked return false
    return false;
}

///////////////////////////////////////////////////////
///Get Active Entry:
///     adds to the entry that's selected.
///
/// \param letter thats typed
/// \return if entry is active in panel
//////////////////////////////////////////////////////

bool Panel::getActiveEntry(char letter){
    for(Entry& entry : _entries){
        if(entry.typedToo()){
            // if active and == 13 then entered
            if(letter == 13){
                entry.clearEntry();
            }else {
                // if not 13 then letter added to entry
                entry.setEntry(letter);
                _lastEntry = entry.getEntry();
            }
            // if active in panel return true
            return true;
        }
    }
    // if none are active return false
    return false;
}


//////////////////////////////////////////////////
///Set Name:
///     sets the _name of the panel to be refered
///     to by main menu
///
/// \param NEW_NAME - _name of the panel
//////////////////////////////////////////////////

void Panel::setName(const std::string& NEW_NAME){
    _name = NEW_NAME;
}

////////////////////////////////////////////////
///Get Name:
///     Gets the _name of the panel
///
/// \return panels _name
////////////////////////////////////////////////

std::string Panel::getName() const{
    return _name;
}

///////////////////////////////////////////////
///Get Position:
///     gets the position of the panel on the
///     screen
///
/// \return position on screen
////////////////////////////////////////////////

sf::Vector2f Panel::getPos() {
    return _pos;
}

///////////////////////////////////////////////////
/// Draw:
///     draws the panel to the window
///
/// \param window - sfml render window to draw to
//////////////////////////////////////////////////

void Panel::draw(sf::RenderWindow &window) {
    // if visible draw buttons entries and labels
    if(_visible) {
        window.draw(_outsideBox);
        for (Button &button : _buttons) {
            button.draw(window);
        }
        for (Entry entry : _entries) {
            entry.draw(window);
        }
        for (Label &label : _labels){
            label.draw(window);
        }
    }

}


/////////////////////////////////////////////////////////
///Add Label:
///     adds label to panel
///
/// \param name - text of the label
/// \param position - position of the label relative to
///         the panel
/// \param size - size of the label
/////////////////////////////////////////////////////////

void Panel::addLabel(std::string name, sf::Vector2f position, sf::Vector2f size){
    Label label(name, size, {float(_pos.x + position.x), float(_pos.y + position.y)});
    label.setFont(_font);
    _labels.push_back(label);
}

////////////////////////////////////////////////////////
///Set Label Text:
///     sets the text in the label
///
/// \param index - the index of the label in _labels
/// \param text - the string to change the text to
//////////////////////////////////////////////////////

void Panel::setLabelText(int index, std::string text) {
    _labels[index].setTitle(text);
}
