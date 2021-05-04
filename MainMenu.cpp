//
// Created by jimmy on 4/10/2021.
//

#include <sstream>
#include "MainMenu.h"
#include "Panel.h"
using namespace sf;

MainMenu::MainMenu() {
    RenderWindow window(VideoMode(720, 900), "Window");
}

MainMenu::MainMenu(RenderWindow& window) {
    lastManip = "default";
   updateWinSize(window);

   // create Top Bar
   Panel top_panel({1000, 105}, {float(windowSize.x)/2 - 500,0});
   const int TOP_SIZE = 3;
   std::string top_buttons[TOP_SIZE] = {"Reset", "Open", "Save"};
   for(int i = 0; i < TOP_SIZE; i++) {
       float y = 10  + i * 30;
       top_panel.addButton(top_buttons[i], {100, 25}, {10, y}, false);
       top_panel.setVisible(true);
   }
    panels.push_back(top_panel);

   //create first variable panel
   Panel var1({200, 105}, {top_panel.getPos().x + float(windowSize.y)/4 - 100, 0});
   var1.addLabel("Enter first variable", {20, 15}, {100, 25});
   var1.addLabel("Type:", {20, 0}, {100, 20});
   var1.addLabel("range", {20, 32}, {100, 20});
   var1.addEntry({100,20}, {50, 50});
   var1.addButton("Submit", {100,20}, {50, 80}, false);
   var1.setName("var1");
   var1.setVisible(false);
   panels.push_back(var1);

    Panel var2({200, 105}, {top_panel.getPos().x + float(windowSize.y)/2 - 100, 0});
    var2.addLabel("Enter first variable", {20, 15}, {100, 25});
    var2.addLabel("Type:", {20, 0}, {100, 20});
    var2.addLabel("range", {20, 32}, {100, 20});;
    var2.addEntry({100,20}, {50, 50});
    var2.addButton("Submit", {100,20}, {50, 80}, false);
    var2.setName("var2");
    var2.setVisible(false);
    panels.push_back(var2);

   // create manip panel
   const int SIZE = 13;
   std::string manip_types[SIZE] = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
                                    "Histogram", "RGB Hist", "Invert Colors", "Contrast", "Saturate", "Outline", "Cancel"};

   bool manip_toggle[SIZE] = {false, false, false, true, true, false, true, true, true, false, false, false, false};
   Panel manip_panel({150, 900}, {0, float(windowSize.y - 900)/2});
   for(int i = 0; i < SIZE; i++) {
       float y = 10 + i * 40;
       manip_panel.addButton(manip_types[i], {140, 30}, {5 , y}, manip_toggle[i]);
   }
   manip_panel.setVisible(true);
   manip_panel.setName("manip_panel");
   panels.push_back(manip_panel);

   Panel loadPanel({float(windowSize.x)/2, float(windowSize.y)/2}, {float(windowSize.x)/4, float(windowSize.y)/4});
   loadPanel.addLabel("Enter the path of the file you'd like to load", {160, 50}, {400, 50});
   loadPanel.addEntry({750, 30}, {100,200});
   loadPanel.setVisible(false);
   panels.push_back(loadPanel);

   histVis = false;
   RGBhistVis = false;

   Panel wait({float(windowSize.x)/2, float(windowSize.y)/2}, {float(windowSize.x)/4, float(windowSize.y)/4});
   wait.addLabel("Working...\nplease wait", {300, 200}, {160, 100});
   wait.setVisible(false);
   panels.push_back(wait);

}


void MainMenu::loadFont(sf::Font &font) {
    this -> font = font;
    for(Panel& panel: panels){
        panel.setFont(font);
    }
}


void MainMenu::updateWinSize(RenderWindow& window){
    windowSize = window.getSize();

}

void MainMenu::loop(RenderWindow& window) {

    while(window.isOpen()){
        pos = window.getPosition();
        window.setFramerateLimit(30);
        window.clear(Color(0,0,0));
        if(sf::Keyboard::isKeyPressed(Keyboard::Add)){
            scaleFactor +=  0.01;
            mainImage.setScale(scaleFactor, scaleFactor);
        }
        if(sf::Keyboard::isKeyPressed(Keyboard::Subtract)){
            scaleFactor -= 0.01;
            mainImage.setScale(scaleFactor, scaleFactor);
        }
        while(window.pollEvent(event)){
            if(event.type == Event::Closed) {
                window.close();
            }else if(event.type == Event::MouseButtonPressed){
                if(sf::Mouse::isButtonPressed(Mouse::Left)){
                    mousePos = sf::Mouse::getPosition();
                    mousePos = {mousePos.x - window.getPosition().x, mousePos.y - window.getPosition().y - 25};
                    std::string action;
                    for(Panel& panel:panels){
                        action = panel.checkButtons(mousePos);
                        if(action != "" || panel.checkEntries(mousePos)) {
                            doAction(action);
                            selectedPanel = panel.getName();
                            break;
                        }
                    }
                }
            }else if(event.type == Event::Resized){
                updateWinSize(window);
                std::cout << windowSize.x << ' ' << windowSize.y << std::endl;
            }else if(event.type == Event::TextEntered){
                std::string action;
                for(Panel& panel:panels){
                   if(panel.getActiveEntry(event.text.unicode)){
                       if(event.text.unicode == 13) {
                           entryPanel = panel.getName();
                           data = panel.getEnteredData();
                           if(entryPanel == "var1" || entryPanel == "var2") {
                               interpretData();
                           }else{
                               action = "Submit";
                               doAction(action);
                           }
                       }
                       break;
                   }
                }
            }
        }
        if(locked){
            check(window);
        }

        if(histVis) {
            window.draw(hist);
        }
        if(RGBhistVis){
            window.draw(RGBhist);
        }
        window.draw(mainImage);

        window.draw(origImage);


        for(Panel& panel:panels){
            panel.draw(window);
        }

        window.display();
    }

}


void MainMenu::getVariable(std::string &name) {
    lastManip = name;
    locked = true;
    panels[1].setVisible(true);
    panels[1].setLabelText(0, name);
    if(name == "Gaussian Blur"){
        req = 2;
        panels[2].setVisible(true);
        panels[2].setLabelText(0, name);
        panels[1].setLabelText(1, "Enter Weight:");
        panels[1].setLabelText(2, "(Positive float)");
        panels[2].setLabelText(1, "Enter Radius:");
        panels[2].setLabelText(2, "(Positive int)");
    }else{

        if(name == "Saturate" ){
            panels[1].setLabelText(1, "Enter Intensity:");
            panels[1].setLabelText(2, "float from -1 to 1");
        }else if(name == "Contrast"){
            panels[1].setLabelText(1, "Enter Intensity");
            panels[1].setLabelText(2, "(Positive float, 1 is original)");
        }else if(name == "Outline"){
            panels[1].setLabelText(1, "Enter Threshold");
            panels[1].setLabelText(2, "(Positive int)");
        }
        req = 1;
    }
}

void MainMenu::check(sf::RenderWindow& window){
    if(req == 0){
        panels[5].setVisible(true);
        panels[5].draw(window);
        window.display();
        locked = false;
        picture.manip(lastManip, variable1, variable2);
        updatePicture();
        variable1 = 0;
        variable2 = 0;
        panels[5].setVisible(false);
        window.clear(sf::Color::Black);
        updateHists();
    }
}

void MainMenu::interpretData(){
    std::stringstream input;
    input.str(data);
    if(selectedPanel == "var1"){
        input >> variable1;
        std::cout << variable1 << std::endl;
        req -= 1;
        panels[1].setVisible(false);
    }else{
        input >> variable2;
        panels[2].setVisible(false);
        req -= 1;

    }
}

void MainMenu::cancelProcess(){
    panels[1].setVisible(false);
    panels[2].setVisible(false);
    locked = false;
    req = 0;
}

void MainMenu::doAction(std::string& manip) {
    std::cout << manip << std::endl;
    if (manip == "Submit") {
        if (selectedPanel == "Open") {
            open();
        } else if (selectedPanel == "Save") {
            save();
        } else if (selectedPanel == "var1" || selectedPanel == "var2") {
            interpretData();
        }
        showPic();
    } else if(manip == "Cancel"){
        cancelProcess();
    } else if(!locked) {
        if (manip == "Saturate" || manip == "Contrast" || manip == "Gaussian Blur" || manip == "Outline") {
            getVariable(manip);
        }else if(manip == "Reset") {
            picture.reset();
            picture.createHistogram();
        }else if(manip == "Histogram"){
            if(histVis){
                histVis = false;
                std::cout << "hist not vis" << std::endl;
            }else {
                histVis = true;
                std::cout << "hist vis" << std::endl;
            }

        }else if(manip == "RGB Hist") {
            if(RGBhistVis){
                RGBhistVis = false;
            }else{
                RGBhistVis = true;
            }
        }else if(manip == "Open") {
            startOpen();
        }else if(manip == "Save"){
            startSave();
        }else{
            picture.manip(manip, 0, 0);
        }
        updatePicture();
    }
    if(histVis || RGBhistVis){
        updateHists();
    }

}

void MainMenu::setFrameRate(int framerate) {
    framerate = framerate;
}

void MainMenu::showPic() {
    texture.loadFromImage(picture.createImage());
    mainImage.setTexture(texture, true);
    scaleFactor = 810.0/picture.getSize().x;
    mainImage.setScale(scaleFactor, scaleFactor);
    mainImage.setPosition(windowSize.x/10, windowSize.y/4);

    textureOrig.loadFromImage(picture.createOrigImage());
    origImage.setTexture(textureOrig, true);
    origImage.setScale(scaleFactor, scaleFactor);
    origImage.setPosition(windowSize.x/1.8, windowSize.y/4);
}

void MainMenu::updateHists() {
    histText.loadFromImage(picture.createHist());
    hist.setTexture(histText);
    hist.setScale(2, 1);
    hist.setPosition(windowSize.x / 1.2 - 2 * histText.getSize().x, windowSize.y - histText.getSize().y);

    RGBhistText.loadFromImage(picture.createRGBHist());
    RGBhist.setTexture(RGBhistText);
    RGBhist.setScale(2, 1);
    RGBhist.setPosition(windowSize.x/1.5  - 2 * (histText.getSize().x + RGBhistText.getSize().x), windowSize.y - RGBhistText.getSize().y);

}

void MainMenu::updatePicture() {
    texture.loadFromImage(picture.createImage());
    mainImage.setTexture(texture, true);
    scaleFactor = 810.0/picture.getSize().x;
    mainImage.setScale(scaleFactor, scaleFactor);
    mainImage.setPosition(windowSize.x/10, windowSize.y/4);
}

void MainMenu::loadPic(std::string filename) {
    picture.load(filename);
}

void MainMenu::startSave() {
    panels[4].setName("Save");
    panels[4].setVisible(true);
    panels[4].setLabelText(0, "Enter the path for the new image:");
}

void MainMenu::save() {
    std::string savePath = panels[4].getEnteredData();
    picture.save(savePath);
    panels[4].setVisible(false);
}

void MainMenu::startOpen() {
    panels[4].setName("Open");
    panels[4].setVisible(true);
    panels[4].setLabelText(0, "Enter the path of the file you'd like to load:");
}

void MainMenu::open() {
    std::string path = panels[4].getEnteredData();
    loadPic(path);
    panels[4].setVisible(false);
}
