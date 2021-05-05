/*
 * CSCI 261 Final Project : Picture.h
 *
 * Authors : James Baldwin and Kaleb Chhabra
 *
 * Description : Picture class to store pixel arrays for histogram and picture as well as
 *               all the manipulations, save methods and load methods.
 */

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
    void createHistogram();

private:

    std::vector<std::vector<Pixel>> _pixArr, _origPixArr, histRGB, hist;

    std::vector<int> _intensities, _rIntense, _gIntense, _bIntense;

    std::vector<std::string> _methods, _typeArr;

    int _indexInType, _RGBIntense, _commonIntense;

    sf::Vector2u _size;

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


    static int calcAverage(std::vector<int>& vector);

    static int findLow(std::vector<int>& vector);

    static int findHigh(std::vector<int>& vector);

    static void fixSat(Pixel& pix);

    static void classify(int& i, int& start, int& high, int& low, int size, int& halfKernal);

    void createHistogramEqualization();

    void createMirror();

    static void setPixAverage(Pixel& pix, int average);

    static sf::Uint8 checkPixBound(int value);

    void loadPixArr(sf::Image& image);

    bool in(std::string& str);
};


#endif //IMAGEPROCESSOR_PICTURE_H
