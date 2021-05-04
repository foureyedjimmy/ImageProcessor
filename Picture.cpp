//
// Created by jimmy on 4/10/2021.
//

#include "Picture.h"
#include <cmath>
#include <fstream>
#include <sstream>


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
    pixArr.clear();
    origPixArr.clear();
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
    createHistogram();
}

void Picture::manip(std::string &manipType, float val1, float val2) {
    if(in(manipType)){
        std::stringstream stream;
        stream << manipType <<  ' '  << val1 << ' '  << val2;
        methods.push_back(stream.str());
        interpret(indexInType, val1, val2);
    }else{
        std::cerr << "manip type not found, please update type list " << std::endl;
    }
}

sf::Image Picture::createOrigImage() {
    return pixArrToImage(origPixArr);
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
        case 8:
            createInvert();
            break;
        case 9:
            createContrast(var1);
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
    createHistogram();
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

int8_t Picture::getBiggest(const Pixel& PIX, char& biggest, char& smallest){
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

void Picture::createContrast(float intensity) {
    updateIntensities();
    int average = calcAverage(intensities);

    for(std::vector<Pixel>& tempArr : pixArr){
        for(Pixel& pix : tempArr){
            setPixAverage(pix, (int)abs((float)(getPixAverage(pix) - average)*intensity + (float)average));
        }
    }
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
        }

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


void Picture::updateIntensities(){
    intensities.clear();
    rIntense.clear();
    gIntense.clear();
    bIntense.clear();
    rIntense.resize(256);
    gIntense.resize(256);
    bIntense.resize(256);
    intensities.resize(256);

    commonIntense = 0;
    RGBIntense = 0;
    for(std::vector<Pixel>& tempPix : pixArr){
        for(Pixel& pix : tempPix){
            uint8_t average = getPixAverage(pix);
            intensities[average] ++;
            rIntense[pix.r] ++;
            bIntense[pix.b] ++;
            gIntense[pix.g] ++;
            if(commonIntense < intensities[average]){
                commonIntense = intensities[average];
            }
            if(RGBIntense < rIntense[pix.r]){
                RGBIntense = rIntense[pix.r];

            }
            if(RGBIntense < bIntense[pix.b]){
                RGBIntense = bIntense[pix.b];

            }
            if(RGBIntense < gIntense[pix.g]){
                RGBIntense = gIntense[pix.g];

            }

        }
    }

}

void Picture::save(std::string& save){
    sf::Image image = createImage();
    image.saveToFile(save);
    while(save[save.size()-1] != '.'){
        save.pop_back();
    }
    save += "txt";
    std::ofstream out;
    out.open(save);
    if(out.is_open()){
        if(!methods.empty()){
            for(std::string& item : methods){
                out << item << std::endl;
            }
        }
        out.close();
    }


}

void Picture::createHistogram() {
    hist.clear();
    histRGB.clear();
    updateIntensities();

    for(int i = 0; i < 255; i++){
        std::vector<Pixel> tempPix(255);
        std::vector<Pixel> tempRGB(255);
        for(int j = 0; j < 255; j++){
            sf::Uint8 r, g, b;
            if(intensities[j] * 255.0 / commonIntense >=  255 - i){
                tempPix[j] = {255,255,255};
            }else{
                tempPix[j] = {0,0,0};
            }
            if(rIntense[j] * 255.0 / RGBIntense >=  255 - i){
                r = 255;
            }else{
                r = 0;
            }
            if(gIntense[j] * 255.0 / RGBIntense >=  255 - i){
                g = 255;
            }else{
                g = 0;
            }
            if(bIntense[j] * 255.0 / RGBIntense>=  255 - i){
                b = 255;
            }else{
                b = 0;
            }
            tempRGB[j] = {r, g, b};
        }
        hist.push_back(tempPix);
        histRGB.push_back(tempRGB);
    }
}


void Picture::createHistogramEqualization() {
    int average = calcAverage(intensities);
    int low = findLow(intensities);
    int high = findHigh(intensities);

    for(std::vector<Pixel>& tempArr : pixArr){
        for(Pixel& pix : tempArr){
            int pixAve = getPixAverage(pix);
            if(pixAve > average){
                pix = {(sf::Uint8)round(((float)(pix.r - average)/(high - average))*(255 - 127.5) + 127.5), (sf::Uint8)round(((float)(pix.g - average)/(high - average))*(high - 127.5) + 127.5), (sf::Uint8)round(((float)(pix.b - average)/(high - average))*(high - 127.5) + 127.5)};
            }else if(pixAve < average){
                pix = {(sf::Uint8)round(((float)(pix.r - low)/(average - low))* 127.5), (sf::Uint8)round(((float)(pix.g - low)/(average - low)) * 127.5), (sf::Uint8)round(((float)(pix.b - low)/(average - low))*127.5)};

            }
            fixSat(pix);
        }
    }
}

void Picture::fixSat(Pixel& pix){
    if(pix.r > 255 || pix.g > 255 || pix.b > 255){
        char biggest, lowest;
        uint8_t offset = getBiggest(pix, biggest, lowest);
        pix = {(sf::Uint8)(pix.r + offset), (sf::Uint8)(pix.g + offset), (sf::Uint8) (pix.b + offset)};
    }
}

void Picture::createMirror() {
    for(int i = 0; i < pixArr.size(); i ++){
        std::vector<Pixel> tempMirror(pixArr[0].size());
        for(int j = 0; j < pixArr[0].size(); j++){
            tempMirror[pixArr[0].size() - j - 1] = pixArr[i][j];
        }
        pixArr[i] = tempMirror;
    }
}

sf::Image Picture::createImage(){
    return pixArrToImage(pixArr);
}

sf::Image Picture::pixArrToImage(const std::vector<std::vector<Pixel>> &PIX_ARR) {
    sf::Image image;
    image.create(PIX_ARR[0].size(), PIX_ARR.size(),
                 sf::Color(0,0,0, 0));
    for(unsigned int i = 0; i < PIX_ARR.size(); i++) {
        for (unsigned int j = 0; j < PIX_ARR[0].size(); j++) {
            image.setPixel(j, i, PIX_ARR[i][j].toColor());
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

sf::Image Picture::createRGBHist() {
    return pixArrToImage(histRGB);
}

sf::Image Picture::createHist() {
    return pixArrToImage(hist);
}


int Picture::calcAverage(std::vector<int> &vector) {
    int sum = 0;
    for(int& item : vector){
        sum += item;
    }
    int secondSum  = 0;
    sum /= 2;
    for(int i = 0; i < vector.size();  i++){
        secondSum += vector[i];
        if(secondSum >= sum){
            return i;
        }
    }
    return -1;
}

int Picture::findLow(std::vector<int> &vector) {
    if(!vector.empty()) {
        for (int i = 0; i < vector.size(); i++) {
            if (vector[i] != 0) {
                return i;
            }
        }
    }
    return -1;
}

int Picture::findHigh(std::vector<int> &vector) {
    if(!vector.empty()){
        for(int i = vector.size(); i >=0; i--){
            if(vector[i] != 0){
                return i;
            }
        }
    }
    return -1;
}

void Picture::setPixAverage(Pixel &pix, int average) {
    float scaler = (float)(3 * average)/ (float)(pix.r + pix.g + pix.b);
    pix = {checkPixBound(pix.r * scaler),
           checkPixBound(pix.g * scaler),
           checkPixBound(pix.b * scaler)};

}

sf::Uint8 Picture::checkPixBound(int value) {
    if(value > 255){
        return 255;
    }else{
        return value;
    }
}
