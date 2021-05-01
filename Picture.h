//
// Created by jimmy on 4/10/2021.
//

#ifndef IMAGEPROCESSOR_PICTURE_H
#define IMAGEPROCESSOR_PICTURE_H
#include <iostream>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>


struct Pixel{

    sf::Uint8 r;
    sf::Uint8 g;
    sf::Uint8 b;
    sf::Uint8 a=255;
    int getAverage() const;
    sf::Color toColor() const;

};

class Picture {
public:
    Picture();

    Picture(std::string& fileName);

    void manip(std::string& manipType);

    sf::Image createImage();

    sf::Image createOrigImage();

    void load(std::string& fileName);

    sf::Vector2u getSize();

private:

    std::vector<std::vector<Pixel>> pixArr, origPixArr;

    std::vector<std::string> methods, typeArr;

    int indexInType;

    sf::Vector2u size;

    void interpret(int value);

    void createInvert();

    void createGrey();

    void createFlip();

    static uint8_t getPixAverage(const Pixel& PIXEL);

    void createOutLine();

    void createRotated();

    void createSaturated();

    void createContrast();

    void createRGB();

    void GaussianBlur();

    void createSmoothOutline();

    void createHistogram();

    void createRGBHistogram();

    void createHistogramEqualization();

    void createMirror();

    void loadPixArr(sf::Image& image);

    bool in(std::string& str);
};


#endif //IMAGEPROCESSOR_PICTURE_H
