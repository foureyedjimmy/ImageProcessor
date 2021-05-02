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

    void manip(std::string& manipType, float var1, float var2);

    sf::Image createImage();

    sf::Image createOrigImage();

    void reset();

    void load(std::string& fileName);

    sf::Vector2u getSize();

private:

    std::vector<std::vector<Pixel>> pixArr, origPixArr, histRBG, hist, red, blue, green;

    std::vector<uint8_t> intensities, rIntense, gIntense, bIntense;

    std::vector<std::string> methods, typeArr;

    int indexInType;

    sf::Vector2u size;

    void interpret(int value, float var1, int var2);

    void createInvert();

    void createGrey();

    void createFlip();

    static uint8_t getPixAverage(const Pixel& PIXEL);

    static void getSaturated(Pixel& pixel, float satVal);

    static uint8_t getBiggest(const Pixel& PIX, char& biggest, char& smallest);

    void createOutLine(uint8_t var);

    void createRotated();

    void createSaturated(float val);

    void createContrast();

    void createRGB();

    void GaussianBlur(float intensity, int kernalSize);

    void getIntensities();

    void createSmoothOutline();

    void classify(int& i, int& start, int& high, int& low, int size, int& halfKernal);

    void createHistogram();

    void createRGBHistogram();

    void createHistogramEqualization();

    void createMirror();

    void loadPixArr(sf::Image& image);

    bool in(std::string& str);
};


#endif //IMAGEPROCESSOR_PICTURE_H
