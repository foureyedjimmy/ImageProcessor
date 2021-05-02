//
// Created by jimmy on 4/10/2021.
//

#include "Picture.h"
#include <cctype>
#include <cmath>





int Pixel::getAverage() const {
    return (r + g + b) / 3;
}


sf::Color Pixel::toColor() const{
    return {r, g, b, a};
}

Picture::Picture() {
    typeArr = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
               "Histogram", "RGB Hist", "Invert Colors", "Contrast", "Saturate", "Outline"};

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


void Picture::getIntensities() {

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
    std::vector<uint8_t> temp(255);
    intensities = temp;
    rIntense = temp;
    gIntense = temp;
    bIntense = temp;
    size = image.getSize();
    for(int i = 0; i < size.y; i++){
        std::vector<Pixel> tempArr(size.x), tempR(size.x), tempG(size.x), tempB(size.y);
        for(int j = 0; j < size.x; j++){
            sf::Color tempColor(image.getPixel(j, i));
            tempArr[j] = {tempColor.r, tempColor.g, tempColor.b, tempColor.a};
            tempR[j] = {tempColor.r, 0, 0, tempColor.a};
            tempG[j] = {0, tempColor.g, 0, tempColor.a};
            tempB[j] = {0,0, tempColor.b, tempColor.a};
            intensities[getPixAverage(tempArr[j])] += 1;
            rIntense[tempColor.r] += 1;
            gIntense[tempColor.r] += 1;
            bIntense[tempColor.r] += 1;
        }
        red.push_back(tempR);
        green.push_back(tempG);
        blue.push_back(tempB);
        pixArr.push_back(tempArr);
        origPixArr.push_back(tempArr);
    }
    createHist();
}

void Picture::manip(std::string &manipType, float val1, float val2) {
    if(in(manipType)){
        methods.push_back(manipType);
        interpret(indexInType, val1, val2);
    }else{
        std::cerr << "manip type not found, please update type list " << std::endl;
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

void Picture::interpret(int value, float var1, int var2) {
    switch(value){
        case 0:
            GaussianBlur(var1, var2);
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
            createSaturated(var1);
            break;
        case 11:
            createOutLine(var1);
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

void Picture::createOutLine(uint8_t threshold) {
    std::vector<std::vector<Pixel>> tempPix;
    for(int i = 0; i < pixArr.size(); i++){
        std::vector<Pixel> tempArr;
        for(int j = 0; j < pixArr[0].size(); j++) {
            if (i != 0 && j != 0) {
                if (abs(getPixAverage(pixArr[i][j]) - getPixAverage(pixArr[i - 1][j])) > threshold ||
                    abs(getPixAverage(pixArr[i][j]) - getPixAverage(pixArr[i][j - 1])) > threshold) {
                    tempArr.push_back({0, 0, 0});
                } else {
                    tempArr.push_back({255, 255, 255});
                }
            } else if (i == 0) {
                if (abs(getPixAverage(pixArr[i][j]) - getPixAverage(pixArr[i][j - 1])) > threshold) {
                    tempArr.push_back({0, 0, 0});
                } else {
                    tempArr.push_back({255, 255, 255});
                }

            }else{
                if(abs(getPixAverage(pixArr[i][j]) - getPixAverage(pixArr[i - 1][j])) > threshold){
                    tempArr.push_back({0, 0, 0});
                }else{
                    tempArr.push_back({255, 255, 255});

                }
            }
        }
        tempPix.push_back(tempArr);
    }
    std::cout << tempPix.size();
    pixArr = tempPix;
}

void Picture::createRotated() {
    std::vector<std::vector<Pixel>> tempRotated;
    for(int i = 0; i < pixArr[0].size(); i++){
        std::vector<Pixel> tempArr;
        for(int j = pixArr.size() - 1; j >= 0; j--){
            tempArr.push_back(pixArr[j][i]);
        }
        tempRotated.push_back(tempArr);
    }
    pixArr.clear();
    pixArr = tempRotated;

}

uint8_t Picture::getBiggest(const Pixel& PIX, char& biggest, char& smallest){
    if(PIX.r == PIX.g && PIX.r == PIX.b){
        smallest = 'r';
        biggest= 'b';
        return 0;
    }else if(PIX.r > PIX.g && PIX.r > PIX.b){
        biggest = 'r';
        if(PIX.g > PIX.b){
            smallest = 'b';
        }else{
            smallest = 'g';
        }
        return 255 - PIX.r;
    }else if(PIX.g >= PIX.r && PIX.g >= PIX.b){
        biggest = 'g';
        if(PIX.r > PIX.b){
            smallest = 'b';
        }else{
            smallest = 'r';
        }
        return 255 - PIX.g;
    }else{
        biggest = 'b';
        if(PIX.g > PIX.r){
            smallest = 'r';
        }else{
            smallest = 'g';
        }
        return 255 - PIX.b;
    }
}

void Picture::getSaturated(Pixel& pixel, float satVal){
    char big, small;
    float max = getBiggest(pixel, big, small) * satVal;
    float mid = max * satVal;

    //tests the minimum color level in pixel
    switch (small) {
        case 'r':
            //finds max color level
            if (big == 'b') {
                pixel = {pixel.r, sf::Uint8 (pixel.g + mid), sf::Uint8(pixel.b + max)};
            }else{
                pixel = {pixel.r, sf::Uint8 (pixel.g + max), sf::Uint8(pixel.b + mid)};
            }
            break;
        case 'g':
            if (big == 'b') {
                pixel = {sf::Uint8(pixel.r + mid), pixel.g, sf::Uint8(pixel.b + max)};
            } else {
                pixel = {sf::Uint8(pixel.r + max), pixel.g, sf::Uint8(pixel.b + mid)};
            }
            break;
        case 'b':
            if (big == 'r') {
                pixel = {sf::Uint8(pixel.r + max), sf::Uint8 (pixel.g + mid), pixel.b};
            } else {
                pixel = {sf::Uint8(pixel.r + mid), sf::Uint8 (pixel.g + max), pixel.b};
            }
            break;
        default:
            pixel = {255,255,255};
    }
}

void Picture::createSaturated(float satVal) {
    for(std::vector<Pixel>& tempArr: pixArr){
        for(Pixel& pixel : tempArr){
            getSaturated(pixel, satVal);
        }
    }

}

void Picture::createContrast() {

}

void Picture::classify(int& i, int& start, int& high, int& low, int size, int& halfKernal){
    //assigns start, low, and high based on the region and size of kernal surrounding pixel
    if(i >= halfKernal && i < size - halfKernal){
        start = 0;
        low = i - halfKernal;
        high = i + halfKernal + 1;
    }else if(i < halfKernal){
        start = halfKernal - i;
        low = 0;
        high = i + halfKernal  + 1;
    }else{
        start = 0;
        low = i - halfKernal;
        high = size;
    }
}


void Picture::GaussianBlur(float intensity, int kernalSize) {
    //initialize variables
    int x;
    int yStart;
    std::vector<std::vector<Pixel>> newArr;
    int kLowerBound;
    int kHighBound;
    int lLowerBound;
    int lHighBound;
    float kernalSum;

    //calculates constantly needed values to save on computing power
    float intenseSquare = intensity * intensity;
    int matSize = kernalSize * 2  + 1;
    float kernal[matSize][matSize];

    //creates gaussian matrix
    for(int i = 0; i < matSize; i++) {
        for (int j = 0; j < matSize; j++) {
            int radx = abs(i - matSize);
            int rady = abs(j - matSize);
            kernal[i][j] = 1.0 / (2 * M_PI * intenseSquare) *
                           exp(static_cast<float> (-(radx * radx + rady * rady)) / (2 * intenseSquare));
            std::cout << kernal[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    //creates actual blurred pixel array
    for(int i = 0; i < pixArr.size(); i++) {
        std::vector<Pixel> tempArr;
        for(int j = 0; j < pixArr[0].size(); j++){

            //checks for edge conditions and assigns iteration bounds accordingly
            classify(i, x, kHighBound, kLowerBound, pixArr.size(), kernalSize);
            classify(j, yStart, lHighBound, lLowerBound, pixArr[0].size(), kernalSize);

            //initializes summation variables
            kernalSum = 0;
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;

            //loops over pixels surrounding kernal locations to determine average
            for (int k = kLowerBound; k < kHighBound; k++) {
                int y = yStart;
                for (int l = lLowerBound; l < lHighBound; l++) {
                    kernalSum += kernal[x][y];
                    sumR += pixArr[k][l].r * kernal[x][y];
                    sumG += pixArr[k][l].g * kernal[x][y];
                    sumB += pixArr[k][l].b * kernal[x][y];
                    y++;
                }
                x++;
            }

            //writes averaged pixel to pixel array
            tempArr.push_back(Pixel{sf::Uint8(sumR / kernalSum), sf::Uint8(sumG / kernalSum),
                                    sf::Uint8(sumB / kernalSum)});
        }
        newArr.push_back(tempArr);
    }
    pixArr = newArr;
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

void Picture::reset(){
    pixArr = origPixArr;
}