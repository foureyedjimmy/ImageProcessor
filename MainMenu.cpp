//
// Created by jimmy on 4/10/2021.
//

#include "MainMenu.h"
#include "Panel.h"
using namespace sf;

MainMenu::MainMenu() {
    RenderWindow window(VideoMode(720, 900), "Window");
}

MainMenu::MainMenu(RenderWindow& window) {
   updateWinSize(window);
   // create Top Bar
   Panel top_panel({1000, 105}, {float(windowSize.x)/2 - 500,0});
   const int TOP_SIZE = 3;
   std::string top_buttons[TOP_SIZE] = {"Undo", "Open", "Save"};
   for(int i = 0; i < TOP_SIZE; i++) {
       float y = 10  + i * 30;
       top_panel.addButton(top_buttons[i], {100, 25}, {10, y}, true);
       top_panel.setVisible(true);
   }
    panels.push_back(top_panel);

   //create first variable panel
   Panel var1({200, 105}, {top_panel.getPos().x + float(windowSize.y)/4 - 100, 0});
   var1.addLabel("Enter first variable", {100, 30}, {50, 10});
   var1.addEntry({100,20}, {50, 50});
   var1.setName("var1");
   var1.setVisible(true);
   panels.push_back(var1);

   // create manip panel
   const int SIZE = 12;
   std::string manip_types[SIZE] = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
                                    "Histogram", "RGB Histogram", "Invert Colors", "Contrast", "Saturate", "Outline"};
   Panel manip_panel({150, 900}, {0, float(windowSize.y - 900)/2});
   for(int i = 0; i < SIZE; i++) {
       float y = 10 + i * 40;
       manip_panel.addButton(manip_types[i], {140, 30}, {5 , y}, true);
   }
   manip_panel.setVisible(true);
   manip_panel.setName("manip_panel");
   manip_panel.addEntry({90, 20}, {5, 800});
   panels.push_back(manip_panel);
}

void MainMenu::loadFont(sf::Font &font) {
    this -> font = font;
    for(Panel& panel: panels){
        panel.setFont(font);
    }
}


void MainMenu::updateWinSize(RenderWindow& window){
    windowSize = window.getSize();
//    for(Panel& panel : panels){
//        panel.resize()
//    }
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
                    for(Panel& panel:panels){
                        std::string action;
                        panel.checkEntries(mousePos);
                        action = panel.checkButtons(mousePos);
                        if(action != "") {
                            doAction(action);
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
                           action = "Submit";
                           doAction(action);
                       }
                       break;
                   }
                }
            }
        }
        window.draw(mainImage);
        window.draw(origImage);
        for(Panel& panel:panels){
            panel.draw(window);
        }
        window.display();
    }

}

void MainMenu::createPhoto(std::string) {

}

void MainMenu::doAction(std::string& manip) {
    if(manip == "Submit"){

    }else{
        picture.manip(manip);
    }
    showPic();
}

void MainMenu::setFrameRate(int framerate) {
    framerate = framerate;
}

void MainMenu::showPic() {
    texture.loadFromImage(picture.createImage());
    mainImage.setTexture(texture);
    scaleFactor = 810.0/picture.getSize().x;
    mainImage.setScale(scaleFactor, scaleFactor);
    mainImage.setPosition(windowSize.x/10, windowSize.y/4);
    textureOrig.loadFromImage(picture.createOrigImage());
    origImage.setTexture(textureOrig);
    origImage.setScale(scaleFactor, scaleFactor);
    origImage.setPosition(windowSize.x/1.8, windowSize.y/4);
}

void MainMenu::loadPic(std::string filename) {
    picture.load(filename);
}