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
    typeArr = {"invert", "grey", "flip", "outline", "rotate", "saturate",
                "contrast", "rgb", "gausblur", "smooth outline", "equalize", "mirror"};

}

Picture::Picture(std::string& fileName) {

    //put picture into pixel array
    sf::Image tempImage;
    tempImage.loadFromFile(fileName);
    size = tempImage.getSize();
    loadPixArr(tempImage);

    typeArr = {"invert", "grey", "flip", "outline", "rotate", "saturate",
               "contrast", "rgb", "gausblur", "smooth outline", "equalize", "mirror"};
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
    sf::Vector2u size = image.getSize();
    std::cout << size.x << ' ' << size.y << std::endl;
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
        case 1:
            createInvert();
        case 2:
            createGrey();
        case 3:
            createFlip();
        case 4:
            createOutLine();
        case 5:
            createRotated();
        case 6:
            createSaturated();
        case 7:
            createContrast();
        case 8:
            createRGB();
        case 9:
            GaussianBlur();
        case 10:
            createSmoothOutline();
        case 11:
            createHistogramEqualization();
        case 12:
            createMirror();
        default:
            std::cerr << "not viable method" << std::endl;
    }
}

void Picture::createInvert() {


}

void Picture::createGrey() {

}

void Picture::createFlip() {

}

void Picture::createOutLine() {

}

void Picture::createRotated() {

}

void Picture::createSaturated() {

}

void Picture::createContrast() {

}

void Picture::createRGB() {

}

void Picture::GaussianBlur() {

}

void Picture::createSmoothOutline() {

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
