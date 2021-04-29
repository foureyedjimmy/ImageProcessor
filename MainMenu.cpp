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
   scaleFactor = 1;
   const int SIZE = 3;
   std::string buttonNames[3] = {"Gaussian Blur", "Equalize",  "Grey Scale"};
   Panel panel({100, 900}, {0, 0});
   for(int i = 0; i < SIZE; i++) {
       float y = 10 + i * 25;
       panel.addButton(buttonNames[i], {90, 20}, {5 , y}, true);
   }
   panels.push_back(panel);
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
                        action = panel.checkButtons(mousePos);
                        if(action != ""){
                            startManip(action);
                        }
                    }
                }
            }else if(event.type == Event::Resized){
                updateWinSize(window);
                std::cout << windowSize.x << ' ' << windowSize.y << std::endl;
            }
        }
        window.draw(mainImage);
        for(Panel& panel:panels){
            panel.draw(window);
        }
        window.display();
    }

}

void MainMenu::createPhoto(std::string) {

}

void MainMenu::startManip(std::string& manip) {
    if(manip = =)

}

void MainMenu::setFrameRate(int framerate) {

}

void MainMenu::showPic() {
    texture.loadFromImage(picture.createImage());
    mainImage.setTexture(texture);
    mainImage.setScale(scaleFactor, scaleFactor);
    mainImage.setPosition(0,0);
}

void MainMenu::loadPic(std::string filename) {
    picture.load(filename);
}