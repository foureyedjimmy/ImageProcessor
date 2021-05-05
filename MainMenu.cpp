/*  CSCI 261 Final Project: MainMenu.cpp
 *
 * Authors: James Baldwin and Kaleb Chhabra
 *
 * Description: Sets up image processing UI to interface with _picture objects.
 */

#include <sstream>
#include "MainMenu.h"
#include "Panel.h"
using namespace sf;


///////////////////////////////////////////////////////////
///Default constructor
///     used to create main menu object as blank main menu
//////////////////////////////////////////////////////////
MainMenu::MainMenu() {
}


/////////////////////////////////////////////////
///Perameterized conststuctor:
///     Passes renderwindow as target for menu
////////////////////////////////////////////////
MainMenu::MainMenu(RenderWindow& window) {
    // sets up default values for crutial members
    _lastManip = "default";
    _histVis = false;
    _RGBhistVis = false;
    _variable1 = 0;
    _variable2 = 0;

    // gets window _size for reference
   updateWinSize(window);

   // create Top Bar
   Panel top_panel({1000, 105}, {float(_windowSize.x) / 2 - 500, 0});
   const int TOP_SIZE = 3;
   std::string top_buttons[TOP_SIZE] = {"Reset", "Open", "Save"};
   // populates panel with _buttons
   for(int i = 0; i < TOP_SIZE; i++) {
       // places them below eachother
       float y = 10  + i * 30;
       top_panel.addButton(top_buttons[i], {100, 25}, {10, y}, false);
       top_panel.setVisible(true);
   }

   //adds to the back of _panels vector
    _panels.push_back(top_panel);

   //create first variable panel
   Panel var1({200, 105}, {top_panel.getPos().x + float(_windowSize.y) / 4 - 100, 0});
   var1.addLabel("Enter first variable", {20, 15}, {100, 25});
   var1.addLabel("Type:", {20, 0}, {100, 20});
   var1.addLabel("range", {20, 32}, {100, 20});
   var1.addEntry({100,20}, {50, 50});
   var1.addButton("Submit", {100,20}, {50, 80}, false);
   var1.setName("var1");
   var1.setVisible(false);
   _panels.push_back(var1);

   // create second variable panel
    Panel var2({200, 105}, {top_panel.getPos().x + float(_windowSize.y) / 2 - 100, 0});
    var2.addLabel("Enter first variable", {20, 15}, {100, 25});
    var2.addLabel("Type:", {20, 0}, {100, 20});
    var2.addLabel("range", {20, 32}, {100, 20});;
    var2.addEntry({100,20}, {50, 50});
    var2.addButton("Submit", {100,20}, {50, 80}, false);
    var2.setName("var2");
    var2.setVisible(false);
    _panels.push_back(var2);

   // create manip panel
   const int SIZE = 13;
   std::string manip_types[SIZE] = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
                                    "Histogram", "RGB Hist", "Invert Colors", "Contrast", "Saturate", "Outline", "Cancel"};

   // needede to tell which _buttons should be togglable
   bool manip_toggle[SIZE] = {false, false, false, true, true, false, true, true, true, false, false, false, false};
   Panel manip_panel({150, 900}, {0, float(_windowSize.y - 900) / 2});
   for(int i = 0; i < SIZE; i++) {
       float y = 10 + i * 40;
       manip_panel.addButton(manip_types[i], {140, 30}, {5 , y}, manip_toggle[i]);
   }
   manip_panel.setVisible(true);
   manip_panel.setName("manip_panel");
   _panels.push_back(manip_panel);


   // creates load/save panel
   Panel loadPanel({float(_windowSize.x) / 2, float(_windowSize.y) / 2}, {float(_windowSize.x) / 4, float(_windowSize.y) / 4});
   loadPanel.addLabel("Enter the path of the file you'd like to load", {160, 50}, {400, 50});
   loadPanel.addEntry({750, 30}, {100,200});
   loadPanel.setVisible(false);
   _panels.push_back(loadPanel);

   // creates wait panel
   Panel wait({float(_windowSize.x) / 2, float(_windowSize.y) / 2}, {float(_windowSize.x) / 4, float(_windowSize.y) / 4});
   wait.addLabel("Working...\nplease wait", {300, 200}, {160, 100});
   wait.setVisible(false);
   _panels.push_back(wait);

}

////////////////////////////////////////////////////////////////////
///Load Font:
///     loads _font into menu which sets the _font for every other
///     object on the main menu
/// \param sfml _font object
///////////////////////////////////////////////////////////////////
void MainMenu::loadFont(sf::Font &font) {
    _font = font;
    for(Panel& panel: _panels){
        panel.setFont(font);
    }
}

//////////////////////////////////////////////////////////////
///Update Window Size:
///     gets the _size of the sfml window to scale drawables
///     appropriately.
/// \param sfml render window
//////////////////////////////////////////////////////////////

void MainMenu::updateWinSize(RenderWindow& window){
    _windowSize = window.getSize();
}

/////////////////////////////////////////////////////////////////
///loop:
///     Runs the main GUI loop for the program
/// \param sfml render window.
////////////////////////////////////////////////////////////////

void MainMenu::loop(RenderWindow& window) {
    // enters loop till window is closed
    while(window.isOpen()){
        // sets frame limiter
        window.setFramerateLimit(30);

        //clears screen to be redrawn
        window.clear(Color(0,0,0));

        //checks events
        while(window.pollEvent(_event)){
            //close condition
            if(_event.type == Event::Closed) {
                window.close();

            //mouse _button click handler
            }else if(_event.type == Event::MouseButtonPressed){

                //checks to make sure _button pressed is left
                if(sf::Mouse::isButtonPressed(Mouse::Left)){

                    //gets mouse _position
                    _mousePos = sf::Mouse::getPosition();

                    //sets to relative _position
                    _mousePos = {_mousePos.x - window.getPosition().x, _mousePos.y - window.getPosition().y - 25};
                    std::string action;
                    for(Panel& panel:_panels){

                        //check all _buttons in panel and gets _action if one is selected.
                        action = panel.checkButtons(_mousePos);

                        //if the _action is not empty or there is a panel selected,
                        // do the _action, and break loop
                        if(action != "" || panel.checkEntries(_mousePos)) {
                            doAction(action);
                            _selectedPanel = panel.getName();
                            break;
                        }
                    }
                }

            //if text is entered handler
            }else if(_event.type == Event::TextEntered){
                std::string action;
                for(Panel& panel:_panels){

                    //checks each _entry in each panel, when one returns true
                    //it will add to _entry box
                   if(panel.getActiveEntry(_event.text.unicode)){

                       //if the text is the enter key clear and submit _data
                       if(_event.text.unicode == 13) {
                           _entryPanel = panel.getName();
                           _data = panel.getEnteredData();
                           if(_entryPanel == "var1" || _entryPanel == "var2") {
                               interpretData();
                           }else{
                               action = "Submit";
                               doAction(action);
                           }
                       }

                       //breaks to save from going through every panel
                       break;
                   }
                }
            }
        }

        // if _locked it will check to see if _action has enough _data
        // if so it will start the manip and unlock
        if(_locked){
            check(window);
        }

        //checks whether to draw histograms
        if(_histVis) {
            window.draw(_hist);
        }
        if(_RGBhistVis){
            window.draw(_RGBhist);
        }

        //draws main images
        window.draw(_mainImage);

        window.draw(_origImage);

        // draws all panels
        for(Panel& panel:_panels){
            panel.draw(window);
        }

        window.display();
    }

}

///////////////////////////////////////////////////
///get Variable:
///     Used to get _data from the variable panels
///     Sets the needed panels to proper formate
///     and locks all other _buttons
/// \param string of manipulation type
//////////////////////////////////////////////////


void MainMenu::getVariable(std::string &name) {

    // grabs the last manipulation and locks all _buttons
    _lastManip = name;
    _locked = true;

    //sets first variable panel to _visible
    // and sets the _name for the label of panel
    _panels[1].setVisible(true);
    _panels[1].setLabelText(0, name);


    // if gaussian blur, 2 variables are needed
    if(name == "Gaussian Blur"){

        // sets number of required variables to 2
        _req = 2;

        // sets panel 1 and 2 to appropriate instructions
        _panels[2].setVisible(true);
        _panels[2].setLabelText(0, name);
        _panels[1].setLabelText(1, "Enter Weight:");
        _panels[1].setLabelText(2, "(Positive float)");
        _panels[2].setLabelText(1, "Enter Radius:");
        _panels[2].setLabelText(2, "(Positive int)");
    }else{

        // goes though other manipulation possibilities
        // and sets to appropriate instructions
        if(name == "Saturate" ){
            _panels[1].setLabelText(1, "Enter Intensity:");
            _panels[1].setLabelText(2, "float from -1 to 1");
        }else if(name == "Contrast"){
            _panels[1].setLabelText(1, "Enter Intensity");
            _panels[1].setLabelText(2, "(Positive float, 1 is original)");
        }else if(name == "Outline"){
            _panels[1].setLabelText(1, "Enter Threshold");
            _panels[1].setLabelText(2, "(Positive int)");
        }

        // all these only require one instruction
        _req = 1;
    }
}

void MainMenu::check(sf::RenderWindow& window){

    // checks to see if requirements are met
    if(_req == 0){

        // if they are, put up waiting panel
        _panels[5].setVisible(true);
        _panels[5].draw(window);
        window.display();

        //set lock to wait and start manip
        _locked = false;
        _picture.manip(_lastManip, _variable1, _variable2);

        // update _picture and set all values back to 0
        // and set wait screen to invisible
        updatePicture();
        _variable1 = 0;
        _variable2 = 0;
        _panels[5].setVisible(false);
        window.clear(sf::Color::Black);

        // updates histograms
        updateHists();
    }
}

///////////////////////////////////////////////////////////////////
///Interpret Data:
///     used to take data from variable panels and store them in
///     either variable 1 or variable 2
//////////////////////////////////////////////////////////////////


void MainMenu::interpretData(){
    std::stringstream input;
    input.str(_data);
    if(_selectedPanel == "var1"){
        input >> _variable1;
        std::cout << _variable1 << std::endl;
        _req -= 1;
        _panels[1].setVisible(false);
    }else{
        input >> _variable2;
        _panels[2].setVisible(false);
        _req -= 1;

    }
}

///////////////////////////////////////////////////////////////////
///Cancel Process:
///     Cancels any process that the gui is in the middle of and
///     turns back on all _buttons
//////////////////////////////////////////////////////////////////

void MainMenu::cancelProcess(){
    _panels[1].setVisible(false);
    _panels[2].setVisible(false);
    _panels[5].setVisible(false);
    _locked = false;
    _req = 0;
}

///////////////////////////////////////////////////
///do Action:
///     Main _button and _entry instruction handler
/// \param manipulation instruction
///////////////////////////////////////////////////

void MainMenu::doAction(std::string& manip) {

    // if the instruction is submit
    if (manip == "Submit") {

        // opening new _picture
        if (_selectedPanel == "Open") {
            open();
            showPic();

        // saveing current _picture
        } else if (_selectedPanel == "Save") {
            save();

        //entering variables
        } else if (_selectedPanel == "var1"){
            _data = _panels[1].getEnteredData();
            interpretData();
        }else if(_selectedPanel == "var2") {
            _data = _panels[2].getEnteredData();
            interpretData();
        }
    // if cancel then cancel processes
    } else if(manip == "Cancel"){
        cancelProcess();

    // if not locked then manip types available
    } else if(!_locked) {

        // types that require varible input
        if (manip == "Saturate" || manip == "Contrast" || manip == "Gaussian Blur" || manip == "Outline") {
            getVariable(manip);
        // changes _picture to original _picture
        }else if(manip == "Reset") {
            _picture.reset();
            _picture.createHistogram();

        // toggles histogram
        }else if(manip == "Histogram"){
            if(_histVis){
                _histVis = false;
                std::cout << "_hist not vis" << std::endl;
            }else {
                _histVis = true;
                std::cout << "_hist vis" << std::endl;
            }
        // toggles RGB histogram
        }else if(manip == "RGB Hist") {
            if(_RGBhistVis){
                _RGBhistVis = false;
            }else{
                _RGBhistVis = true;
            }

        // opens open window or save window
        }else if(manip == "Open") {
            startOpen();
        }else if(manip == "Save"){
            startSave();
        // if not of these then must be manip type so sends to manip
        }else{
            _picture.manip(manip, 0, 0);
        }
        // updates _picture
        updatePicture();
    }

    //if histogram is _visible, update histogram
    if(_histVis || _RGBhistVis){
        updateHists();
    }

}


////////////////////////////////////////////////////////////////////////
///Show Picture:
///     Called when new _picture is loaded, creates new manip pic
///     and new original _picture.
///////////////////////////////////////////////////////////////////////

void MainMenu::showPic() {
    //updates manipulatable pictre
    updatePicture();

    //updates constant _picture
    _textureOrig.loadFromImage(_picture.createOrigImage());
    _origImage.setTexture(_textureOrig, true);
    _origImage.setScale(_scaleFactor, _scaleFactor);
    _origImage.setPosition(_windowSize.x / 1.8, _windowSize.y / 4);
}

////////////////////////////////////////////////////////////
///Update Histograms:
///     used to update the histogram images when the
///     manip image has changed.
///////////////////////////////////////////////////////////

void MainMenu::updateHists() {

    // loads normal histogram and places at bottom of screen
    _histText.loadFromImage(_picture.createHist());
    _hist.setTexture(_histText);
    _hist.setScale(2, 1);
    _hist.setPosition(_windowSize.x / 1.2 - 2 * _histText.getSize().x, _windowSize.y - _histText.getSize().y);

    // loads RGB histogram and loads at bottom of screen
    _RGBhistText.loadFromImage(_picture.createRGBHist());
    _RGBhist.setTexture(_RGBhistText);
    _RGBhist.setScale(2, 1);
    _RGBhist.setPosition(_windowSize.x / 1.5 - 2 * (_histText.getSize().x + _RGBhistText.getSize().x), _windowSize.y - _RGBhistText.getSize().y);

}

//////////////////////////////////////////////////////////////////////////
///Update Picture:
///     Used to update the manipulatable image after manip is preformed
/////////////////////////////////////////////////////////////////////////

void MainMenu::updatePicture() {

    // loads _pixArr as image from _picture class
    _texture.loadFromImage(_picture.createImage());
    _mainImage.setTexture(_texture, true);
    _scaleFactor = 810.0 / _picture.getSize().x;
    _mainImage.setScale(_scaleFactor, _scaleFactor);
    _mainImage.setPosition(_windowSize.x / 10, _windowSize.y / 4);
}

//////////////////////////////////////////////////////////////////////////
///Load Pic:
///     used to load a new image into _picture object
/////////////////////////////////////////////////////////////////////////
void MainMenu::loadPic(std::string filename) {
    _picture.load(filename);
}

///////////////////////////////////////////////////////////////////
///Start Save:
///     sets up GUI's save window to ask user for pathname to save
//////////////////////////////////////////////////////////////////

void MainMenu::startSave() {
    // sets up panels[4] to take in save info
    _panels[4].setName("Save");
    _panels[4].setVisible(true);
    _panels[4].setLabelText(0, "Enter the path for the new image:");
}


/////////////////////////////////////////////////
///Save:
///     gets path and saves _picture at path
/////////////////////////////////////////////////

void MainMenu::save() {
    std::string savePath = _panels[4].getEnteredData();
    _picture.save(savePath);
    _panels[4].setVisible(false);
}

////////////////////////////////////////////////////////////
///Start Open:
///     Sets up load/save screen to get open path from user
////////////////////////////////////////////////////////////

void MainMenu::startOpen() {
    _panels[4].setName("Open");
    _panels[4].setVisible(true);
    _panels[4].setLabelText(0, "Enter the path of the file you'd like to load:");
}

//////////////////////////////////////////////////////////////
///Open:
///     loads in _picture at path entered into save/load panel
//////////////////////////////////////////////////////////////

void MainMenu::open() {
    std::string path = _panels[4].getEnteredData();
    loadPic(path);
    _panels[4].setVisible(false);
}
