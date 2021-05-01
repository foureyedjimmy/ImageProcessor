//
// Created by jimmy on 4/10/2021.
//

#include "Picture.h"
#include <cctype>






int Pixel::getAverage() const {
    return (r + g + b) / 3;
}


sf::Color Pixel::toColor() const{
    return {r, g, b, a};
}

Picture::Picture() {
    typeArr = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
               "Histogram", "RGB Histogram", "Invert Colors", "Contrast", "Saturate", "Outline"};

}

Picture::Picture(std::string& fileName) {

    //put picture into pixel array
    sf::Image tempImage;
    tempImage.loadFromFile(fileName);
    size = tempImage.getSize();
    loadPixArr(tempImage);

    typeArr = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
               "Histogram", "RGB Histogram", "Invert Colors", "Contrast", "Saturate", "Outline"};
}


bool Picture::in(std::string& str){
    for(int i = 0; i < typeArr.size(); i++){
        if(str == typeArr[i]){
            indexInType = i;
            return true;
        }
    }
    return false;
}

void Picture::loadPixArr(sf::Image& image){
    size = image.getSize();
    for(int i = 0; i < size.y; i++){
        std::vector<Pixel> tempArr(size.x);
        for(int j = 0; j < size.x; j++){
            sf::Color tempColor(image.getPixel(j, i));
            tempArr[j] = {tempColor.r, tempColor.g, tempColor.b, tempColor.a};

        }
        pixArr.push_back(tempArr);
        origPixArr.push_back(tempArr);
    }
}

void Picture::manip(std::string &manipType) {
    if(in(manipType)){
        methods.push_back(manipType);
        interpret(indexInType);
    }else{
        std::cerr << "manip type not found, please update type list" << std::endl;
    }
}

sf::Image Picture::createOrigImage() {
    sf::Image image;
    image.create(origPixArr[0].size(), origPixArr.size(), sf::Color(0,0,0, 0));
    for(unsigned int i = 0; i < origPixArr.size(); i++) {
        for (unsigned int j = 0; j < origPixArr[0].size(); j++) {
            image.setPixel(j, i, origPixArr[i][j].toColor());
        }
    }
    return image;
}


void Picture::load(std::string &fileName) {
    sf::Image tempImage;
    if(!tempImage.loadFromFile(fileName)){
        std::cerr << "error loading " << fileName << std::endl;
    }
    loadPixArr(tempImage);


}

void Picture::interpret(int value) {
    switch(value){
        case 0:
            GaussianBlur();
            break;
        case 1:
            createHistogramEqualization();
            break;
        case 2:
            createGrey();
            break;
        case 3:
            createMirror();
            break;
        case 4:
            createFlip();
            break;
        case 5:
            createRotated();
            break;
        case 6:
            createHistogram();
            break;
        case 7:
            createRGBHistogram();
            break;
        case 8:
            createInvert();
            break;
        case 9:
            createContrast();
            break;
        case 10:
            createSaturated();
            break;
        case 11:
            createOutLine();
            break;
        default:
            std::cerr << "not viable method" << std::endl;
    }
}

uint8_t Picture::getPixAverage(const Pixel& PIXEL){
    return (PIXEL.r + PIXEL.g + PIXEL.b)/3;
}

void Picture::createInvert() {
    for(std::vector<Pixel>& tempArr : pixArr){
        for(Pixel& pixel : tempArr) {
            pixel = {sf::Uint8 (255 - pixel.r), sf::Uint8(255 - pixel.g), sf::Uint8 (255 - pixel.b)};
        }
    }
}

void Picture::createGrey() {
    for(std::vector<Pixel>& tempArr : pixArr){
        for(Pixel& pixel : tempArr){
            uint8_t average = getPixAverage(pixel);
            pixel = {average, average, average};
        }
    }
}

void Picture::createFlip() {
    std::vector<std::vector<Pixel>> tempArr = pixArr;
    for(int i = pixArr.size() - 1; i >= 0; i --){
        int k = pixArr.size() - i - 1;
        for(int j = 0; j < pixArr[0].size(); j++){
            pixArr[i][j] = tempArr[k][j];
        }
    }
}

void Picture::createOutLine() {

}

void Picture::createRotated() {
    std::vector<std::vector<Pixel>> tempArr = pixArr;
    for(int i = pixArr.size() - 1; i >= 0; i --){
        int k = pixArr.size() - i - 1;
        for(int j = pixArr[0].size(); j >= 0; j--){
            pixArr[i][j] = tempArr[k][j];
        }
    }
}

void Picture::createSaturated() {

}

void Picture::createContrast() {

}


void Picture::GaussianBlur() {

}

void Picture::createHistogram() {

}

void Picture::createRGBHistogram() {

}

void Picture::createHistogramEqualization() {

}

void Picture::createMirror() {

}

sf::Image Picture::createImage(){
    sf::Image image;
    image.create(pixArr[0].size(), pixArr.size(), sf::Color(0,0,0, 0));
    for(unsigned int i = 0; i < pixArr.size(); i++) {
        for (unsigned int j = 0; j < pixArr[0].size(); j++) {
            image.setPixel(j, i, pixArr[i][j].toColor());
        }
    }
    return image;
}

sf::Vector2u Picture::getSize() {
    return size;
}

