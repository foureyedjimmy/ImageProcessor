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
    sf::Color toColor() const;


};

class Picture {
public:
    Picture();

    Picture(std::string& fileName);

    void manip(std::string& manipType, float var1, float var2);

    sf::Image createImage();

    sf::Image createOrigImage();

    sf::Image createRGBHist();

    sf::Image createHist();

    void reset();

    void save(std::string& path);

    void load(std::string& fileName);

    sf::Vector2u getSize();

private:

    std::vector<std::vector<Pixel>> pixArr, origPixArr, histRGB, hist;

    std::vector<int> intensities, rIntense, gIntense, bIntense;

    std::vector<std::string> methods, typeArr;

    int indexInType, RGBIntense, commonIntense;

    sf::Vector2u size;

    void interpret(int value, float var1, int var2);

    void createInvert();

    void createGrey();

    void createFlip();

    static uint8_t getPixAverage(const Pixel& PIXEL);

    static void getSaturated(Pixel& pixel, float satVal);

    static int8_t getBiggest(const Pixel& PIX, char& biggest, char& smallest);


    static sf::Image pixArrToImage(const std::vector<std::vector<Pixel>>& PIX_ARR);

    void updateIntensities();

    void createOutLine(uint8_t var);

    void createRotated();

    void createSaturated(float val);

    void createContrast(float intensity);


    void GaussianBlur(float intensity, int kernalSize);

    void getIntensities();

    static int calcAverage(std::vector<int>& vector);

    static int findLow(std::vector<int>& vector);

    static int findHigh(std::vector<int>& vector);

    static void fixSat(Pixel& pix);

    static void classify(int& i, int& start, int& high, int& low, int size, int& halfKernal);

    void createHistogram();

    void createHistogramEqualization();

    void createMirror();

    static void setPixAverage(Pixel& pix, int average);
    static sf::Uint8 checkPixBound(int value);

    void loadPixArr(sf::Image& image);

    bool in(std::string& str);
};


#endif //IMAGEPROCESSOR_PICTURE_H
