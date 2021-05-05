/*  CSCI 261 Final Project : Picture.cpp
 *
 * Authors: James Baldwin, Kaleb Chhabra
 *
 * Description: _picture class to containing pixels able to communicate
 *              with Main Menu class and be edited
 *
 */

#include "Picture.h"
#include <cmath>
#include <fstream>
#include <sstream>


////////////////////////////////////////////////////
///To Color:
///     Turns Pixel to SFML Color
/// \return sfml Color with same values as pixel
///////////////////////////////////////////////////

sf::Color Pixel::toColor() const{
    return {r, g, b, a};
}

//////////////////////////////////////////////////////////
///Default Constructor:
///     creates _picture object when no params are passed
/////////////////////////////////////////////////////////


Picture::Picture() {
    //very important array
    _typeArr = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
                "Histogram", "RGB Hist", "Invert Colors", "Contrast", "Saturate", "Outline"};

}

////////////////////////////////////////////////////
///Parameterized Constructor:
///     Uses file path to create _picture object
/// \param Path to _picture
///////////////////////////////////////////////////

Picture::Picture(std::string& fileName) {

    //put _picture into pixel array
    sf::Image tempImage;
    if(tempImage.loadFromFile(fileName)){
        _size = tempImage.getSize();
        loadPixArr(tempImage);
    }else{
        std::cerr << "error loading " << fileName <<  std::endl;
    }
    _typeArr = {"Gaussian Blur", "Equalize", "Grey Scale", "Mirror", "Flip", "Rotate 90 deg",
                "Histogram", "RGB Histogram", "Invert Colors", "Contrast", "Saturate", "Outline"};
}


/////////////////////////////////////////////////////////
///in:
///     used to determine if a manip type is in a list
///     and to tell where it lives in the type array
/// \param manipulation _name
/// \return whether its in the type array
///////////////////////////////////////////////////////

bool Picture::in(std::string& str){

    //loops through types array
    for(int i = 0; i < _typeArr.size(); i++){
        if(str == _typeArr[i]){
            // if they are equal then index is set to i
            _indexInType = i;
            return true;
        }
    }
    return false;
}


///////////////////////////////////////////////////////
///Load Pixel Array:
///     Loads in the pixel array from the image passed
/// \param sfml image to be converted to pix array
//////////////////////////////////////////////////////

void Picture::loadPixArr(sf::Image& image){
    //  clears pixel arrays
    _pixArr.clear();
    _origPixArr.clear();

    // sets _size
    _size = image.getSize();

    //loops through image
    for(int i = 0; i < _size.y; i++){
        //temp array used to fill pix array
        std::vector<Pixel> tempArr(_size.x);
        for(int j = 0; j < _size.x; j++){
            //gets color at that pixel in image
            sf::Color tempColor(image.getPixel(j, i));

            //stores as pixel in temp array
            tempArr[j] = {tempColor.r, tempColor.g, tempColor.b, tempColor.a};
        }
        //pushes temp array into pixel arrays
        _pixArr.push_back(tempArr);
        _origPixArr.push_back(tempArr);
    }
    //creates histograms
    createHistogram();
}


/////////////////////////////////////////////////////////////////
///Manip:
///     public method used to tell object to manipulate pixArr
/// \param string of manipulation type
/// \param first varible (float)
/// \param second variable (float)
/////////////////////////////////////////////////////////////////////
void Picture::manip(std::string &manipType, float val1, float val2) {
    // checks if the manip type is valid
    if(in(manipType)){

        // writes into _methods vector for saveing in txt when pic is saved
        std::stringstream stream;
        stream << manipType <<  ' '  << val1 << ' '  << val2;
        _methods.push_back(stream.str());

        // interprets value
        interpret(_indexInType, val1, val2);
    }else{
        // prints out error message if the manipulation type isnt in array
        std::cerr << "manip type not found, please update type list " << std::endl;
    }
}


/////////////////////////////////////////////////////////////////////////
///create Original Image:
///     creates an sfml image from original pixal array
/// \return sfml Image of origPixArr
///////////////////////////////////////////////////////////////////

sf::Image Picture::createOrigImage() {
    //sends to pixArrToImage to make into image
    return pixArrToImage(_origPixArr);
}


///////////////////////////////////////////////
///load:
///     loads new image into _picture
/// \param file path
///////////////////////////////////////////////

void Picture::load(std::string &fileName) {
    sf::Image tempImage;

    // if cant load, error created
    if(!tempImage.loadFromFile(fileName)){
        std::cerr << "error loading " << fileName << std::endl;
    }
    loadPixArr(tempImage);
}

//////////////////////////////////////////////////////
///Interpret:
///     Used to interpret manipulation types and
///     initiate manip methods
/// \param index in type array
/// \param variable 1
/// \param variable 2
//////////////////////////////////////////////////////

void Picture::interpret(int value, float var1, int var2) {
    // assigns the proper method to the index in types array

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
    //updates histograms
    createHistogram();
}



//////////////////////////////////////////////////////////
///Get Pixel Average:
///     Calculates the average intensity for a pixel
/// \param PIXEL - pixel to calc average
/// \return average of r g and b
//////////////////////////////////////////////////////////
uint8_t Picture::getPixAverage(const Pixel& PIXEL){
    return (PIXEL.r + PIXEL.g + PIXEL.b)/3;
}


/////////////////////////////////////////
///Create Invert:
///     Inverts the colors of pixArr
////////////////////////////////////////

void Picture::createInvert() {

    for(std::vector<Pixel>& tempArr : _pixArr){
        for(Pixel& pixel : tempArr) {
            pixel = {sf::Uint8 (255 - pixel.r), sf::Uint8(255 - pixel.g), sf::Uint8 (255 - pixel.b)};
        }
    }
}

////////////////////////////////////////
///Create Grey:
///     Turns pixArr into grey scale
//////////////////////////////////////

void Picture::createGrey() {
    for(std::vector<Pixel>& tempArr : _pixArr){
        for(Pixel& pixel : tempArr){
            uint8_t average = getPixAverage(pixel);
            pixel = {average, average, average};
        }
    }
}

///////////////////////////////////////
///Create Flip:
///     Flips the image (equivalent
///     to rotate 180 deg and mirror)
///////////////////////////////////////

void Picture::createFlip() {
    // stores pixel array into temp array
    std::vector<std::vector<Pixel>> tempArr = _pixArr;
    for(int i = _pixArr.size() - 1; i >= 0; i --){
        int k = _pixArr.size() - i - 1;
        for(int j = 0; j < _pixArr[0].size(); j++){
            // flips the columns in pixArr
            _pixArr[i][j] = tempArr[k][j];
        }
    }
}


////////////////////////////////////////////////////////////////
///Create Outline:
///     Creates an outline of objects where there a difference
///     larger than threshold
/// \param threshold - difference before outline
///////////////////////////////////////////////////////////////
void Picture::createOutLine(uint8_t threshold) {
    // temp pix to store new image
    std::vector<std::vector<Pixel>> tempPix;
    for(int i = 0; i < _pixArr.size(); i++){
        //stores pixles in temp arra
        std::vector<Pixel> tempArr;
        for(int j = 0; j < _pixArr[0].size(); j++) {
            // if not at edges
            if (i != 0 && j != 0) {
                if (abs(getPixAverage(_pixArr[i][j]) - getPixAverage(_pixArr[i - 1][j])) > threshold ||
                    abs(getPixAverage(_pixArr[i][j]) - getPixAverage(_pixArr[i][j - 1])) > threshold) {
                    tempArr.push_back({0, 0, 0});
                } else {
                    tempArr.push_back({255, 255, 255});
                }

            // if at adge only look to one side for difference
            } else if (i == 0) {
                if (abs(getPixAverage(_pixArr[i][j]) - getPixAverage(_pixArr[i][j - 1])) > threshold) {
                    tempArr.push_back({0, 0, 0});
                } else {
                    tempArr.push_back({255, 255, 255});
                }

            }else{
                if(abs(getPixAverage(_pixArr[i][j]) - getPixAverage(_pixArr[i - 1][j])) > threshold){
                    tempArr.push_back({0, 0, 0});
                }else{
                    tempArr.push_back({255, 255, 255});

                }
            }
        }
        tempPix.push_back(tempArr);
    }

    // makes the temp array into pixarray
    _pixArr = tempPix;
}



////////////////////////////////////////////
///Create Rotated:
///     Rotates pixArr 90 degrees clockwise
////////////////////////////////////////////

void Picture::createRotated() {

    //creates temparary array to compare to
    std::vector<std::vector<Pixel>> tempRotated;
    for(int i = 0; i < _pixArr[0].size(); i++){
        std::vector<Pixel> tempArr;
        for(int j = _pixArr.size() - 1; j >= 0; j--){
            tempArr.push_back(_pixArr[j][i]);
        }
        tempRotated.push_back(tempArr);
    }
    //clears pixArray ass dimesions will likely change
    _pixArr.clear();
    //makes temp rotated into pixArr
    _pixArr = tempRotated;

}


//////////////////////////////////////////////////////////////
///getBiggest:
///     gets the biggest value difference from the maximum,
///     and assigns the biggest and smallest colors
/// \param PIX - pixel to analyze
/// \param biggest - for biggest color to store
/// \param smallest - for smallest color to store
/// \return the difference between the max value and the max
///         in pixel
////////////////////////////////////////////////////////////////

int8_t Picture::getBiggest(const Pixel& PIX, char& biggest, char& smallest){
    // if all are same then do white scale and do nothing
    if(PIX.r == PIX.g && PIX.r == PIX.b){
        smallest = 'r';
        biggest= 'b';
        return 0;

    // else assign biggest and smallest accordingly
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


//////////////////////////////////////////////////////
///get Saturated:
///     changes passed pixel into a more saturated
///     version of itself
/// \param pixel - pixel to saturate
/// \param satVal - scale of how much to saturate pixel
////////////////////////////////////////////////////////


void Picture::getSaturated(Pixel& pixel, float satVal){

    // needed temporary variables
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

////////////////////////////////////////////////////
///Create Saturated:
///     creates saturates each pixel in pixArr
///////////////////////////////////////////////////


void Picture::createSaturated(float satVal) {
    for(std::vector<Pixel>& tempArr: _pixArr){
        for(Pixel& pixel : tempArr){
            getSaturated(pixel, satVal);
        }
    }

}

//////////////////////////////////////////////////////
///create Contrast:
///     pushes pixels intensites to the edges of
///     histogram to increase contrast
//////////////////////////////////////////////////////

void Picture::createContrast(float intensity) {
    // updates intensity vector
    updateIntensities();
    // gets average value of _intensities
    int average = calcAverage(_intensities);

    // sets each pixels average relative to their _pos from average
    for(std::vector<Pixel>& tempArr : _pixArr){
        for(Pixel& pix : tempArr){
            //set average to  |pixAve - average| * _intensities + average
            setPixAverage(pix, (int)abs((float)(getPixAverage(pix) - average)*intensity + (float)average));
        }
    }
}

///////////////////////////////////////////////////////////////////
///classify:
///     used to create starting conditions for each point in
///     gaussian blur to take in account edges and such
/// \param current i _position
/// \param start _position for loop
/// \param high point of matrix
/// \param low point of matrix
/// \param size of matrix
/// \param halfKernal kernal radius
///////////////////////////////////////////////////////////////////

void Picture::classify(int& i, int& start, int& high, int& low, int size, int& halfKernal){
    //assigns start, low, and high based on the region and _size of kernal surrounding pixel
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

///////////////////////////////////////////////////////////////////
///Gaussian Blur:
///     uses gaussian matrix using sigma of intensity with a _size
///     of 2 * kernalSize + 1 x 2 * kernalSize + 1 to take an average
///     of sourrounding pixels to find the color of center pixel
///     inorder to create a blur effect.
/// \param intensity - sigma value when creating matrix
/// \param kernalSize - half _size of matrix
///////////////////////////////////////////////////////////////////

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
    for(int i = 0; i < _pixArr.size(); i++) {
        std::vector<Pixel> tempArr;
        for(int j = 0; j < _pixArr[0].size(); j++){

            //checks for edge conditions and assigns iteration bounds accordingly
            classify(i, x, kHighBound, kLowerBound, _pixArr.size(), kernalSize);
            classify(j, yStart, lHighBound, lLowerBound, _pixArr[0].size(), kernalSize);

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
                    sumR += _pixArr[k][l].r * kernal[x][y];
                    sumG += _pixArr[k][l].g * kernal[x][y];
                    sumB += _pixArr[k][l].b * kernal[x][y];
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
    _pixArr = newArr;
}

//////////////////////////////////////////////////////////////////////////
///Update Intensites:
///     updates intensity vectors to current _intensities in pixArr
/////////////////////////////////////////////////////////////////////////

void Picture::updateIntensities(){

    //clears previous _intensities
    _intensities.clear();
    _rIntense.clear();
    _gIntense.clear();
    _bIntense.clear();

    //resizes to avoid copying
    _rIntense.resize(256);
    _gIntense.resize(256);
    _bIntense.resize(256);
    _intensities.resize(256);


    //sets max values to 0
    _commonIntense = 0;
    _RGBIntense = 0;

    for(std::vector<Pixel>& tempPix : _pixArr){
        for(Pixel& pix : tempPix){

            //calc average for less computations
            uint8_t average = getPixAverage(pix);

            // increments arrays at values
            _intensities[average] ++;
            _rIntense[pix.r] ++;
            _bIntense[pix.b] ++;
            _gIntense[pix.g] ++;

            // assigns max to maxes average and per color differ for accurate representation in histogram
            if(_commonIntense < _intensities[average]){
                _commonIntense = _intensities[average];
            }
            if(_RGBIntense < _rIntense[pix.r]){
                _RGBIntense = _rIntense[pix.r];

            }
            if(_RGBIntense < _bIntense[pix.b]){
                _RGBIntense = _bIntense[pix.b];

            }
            if(_RGBIntense < _gIntense[pix.g]){
                _RGBIntense = _gIntense[pix.g];

            }

        }
    }

}

///////////////////////////////////////////////////////////////////
///Save:
///     saves image and txt file at given file location and _name
/// \param save path
///////////////////////////////////////////////////////////////////

void Picture::save(std::string& save){
    // creates image to save out of pixArr
    sf::Image image = createImage();
    // check to see if save successfull
    if(!image.saveToFile(save)){
        std::cerr << "_picture save failed at " << save << std::endl;

    // if it is it will save txt file with manips with same _name
    }else {

        // clears _picture file extension
        while (save[save.size() - 1] != '.') {
            save.pop_back();
        }

        //creates txt file path at same location and same _name
        save += "txt";
        std::ofstream out;
        out.open(save);

        // if opens successfully it will write methods to file
        if (out.is_open()) {
            if (!_methods.empty()) {
                for (std::string &item : _methods) {
                    out << item << std::endl;
                }
            }
            out.close();

        // else it will error message
        } else {
            std::cerr << "manip txt file failed to save at " << save << std::endl;
        }
    }
}

////////////////////////////////////////////////////
///Create Histograms:
///     Populates histograms with intensity data
////////////////////////////////////////////////////

void Picture::createHistogram() {

    //clears previous histograms
    hist.clear();
    histRGB.clear();

    //updates intensity vectors
    updateIntensities();

    for(int i = 0; i < 255; i++){

        // creates temparary 1d pix vectors for each row
        std::vector<Pixel> tempPix(256);
        std::vector<Pixel> tempRGB(256);

        for(int j = 0; j < 256; j++){
            sf::Uint8 r, g, b;
            // populates with proper color at proper point in pic
            if(_intensities[j] * 255.0 / _commonIntense >= 255 - i){
                tempPix[j] = {255,255,255};
            }else{
                tempPix[j] = {0,0,0};
            }
            if(_rIntense[j] * 255.0 / _RGBIntense >= 255 - i){
                r = 255;
            }else{
                r = 0;
            }
            if(_gIntense[j] * 255.0 / _RGBIntense >= 255 - i){
                g = 255;
            }else{
                g = 0;
            }
            if(_bIntense[j] * 255.0 / _RGBIntense >= 255 - i){
                b = 255;
            }else{
                b = 0;
            }
            tempRGB[j] = {r, g, b};
        }
        //pushes back onto histograms
        hist.push_back(tempPix);
        histRGB.push_back(tempRGB);
    }
}


/////////////////////////////////////////////////////////////////
///Create Histogram Equalization:
///     uses intensites to shift average intensity 127 (255/2)
///     high get pushed low and low get pushed high.
/////////////////////////////////////////////////////////////////

void Picture::createHistogramEqualization() {

    //gets high and low and average values
    int average = calcAverage(_intensities);
    int low = findLow(_intensities);
    int high = findHigh(_intensities);

    // goes through and redistribute to achieve average of 127
    for(std::vector<Pixel>& tempArr : _pixArr){
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

/////////////////////////////////////////////////
///fix Saturation:
///     used to fix over saturation in pixels
/// \param Pixel to check and fix saturation on
////////////////////////////////////////////////
void Picture::fixSat(Pixel& pix){
    if(pix.r > 255 || pix.g > 255 || pix.b > 255){
        char biggest, lowest;

        //gets offset for pixel

        uint8_t offset = getBiggest(pix, biggest, lowest);
        // changes pix to be not over saturated
        pix = {(sf::Uint8)(pix.r + offset), (sf::Uint8)(pix.g + offset), (sf::Uint8) (pix.b + offset)};
    }
}

/////////////////////////////
///Create Mirror:
///     Mirrors the pixArr
////////////////////////////

void Picture::createMirror() {
    for(int i = 0; i < _pixArr.size(); i ++){

        //creates temp array to hold pixels of _size of row
        std::vector<Pixel> tempMirror(_pixArr[0].size());
        for(int j = 0; j < _pixArr[0].size(); j++){
            // puts the pixel at the end equal to pix at start
            tempMirror[_pixArr[0].size() - j - 1] = _pixArr[i][j];
        }
        //assigns row to pixarray
        _pixArr[i] = tempMirror;
    }
}


//////////////////////////////////////////
///Create Image:
///     Creates SFML image from pixArr
/// \return SFML Image of pixArr
/////////////////////////////////////////
sf::Image Picture::createImage(){
    return pixArrToImage(_pixArr);
}


////////////////////////////////////////////////////////
///Pixel Array To Image:
///     turns a pixel array into an sfml image (static)
/// \param PIX_ARR - pixel array to turn into image
/// \return SFML Image
////////////////////////////////////////////////////////


sf::Image Picture::pixArrToImage(const std::vector<std::vector<Pixel>> &PIX_ARR) {
    // creates image to return
    sf::Image image;
    image.create(PIX_ARR[0].size(), PIX_ARR.size(),
                 sf::Color(0,0,0, 0));

    // goes through image and assigns to value of pixel in pixel array at same location
    for(unsigned int i = 0; i < PIX_ARR.size(); i++) {
        for (unsigned int j = 0; j < PIX_ARR[0].size(); j++) {
            image.setPixel(j, i, PIX_ARR[i][j].toColor());
        }
    }
    return image;
}

///////////////////////////////////////////////
///Get Size:
///     getter for the _size of the image
/// \return image _size as sf::Vector2f
///////////////////////////////////////////////

sf::Vector2u Picture::getSize() {
    return _size;
}


///////////////////////////////////////////////////
///reset:
///     assigns pixel array to original pixel array
////////////////////////////////////////////////////
void Picture::reset(){
    _pixArr = _origPixArr;
}

/////////////////////////////////////////////////////////
///Create RGB Histogram:
///     turns RGB histogram array to sfml image
/// \return sfml image of rgb histogram
////////////////////////////////////////////////////////

sf::Image Picture::createRGBHist() {
    return pixArrToImage(histRGB);
}

///////////////////////////////////////////////////////
///Create Hist:
///     turns histogram pixel array into sfml image
/// \return sfml image of histogram array
//////////////////////////////////////////////////////

sf::Image Picture::createHist() {
    return pixArrToImage(hist);
}

////////////////////////////////////////////////////////////////
///calculate Average:
///     Calculates the average value of integer vector (static)
///
/// \param vector to calculate average intensity of
/// \return average index in vector
///////////////////////////////////////////////////////////////

int Picture::calcAverage(std::vector<int> &vector) {
    int sum = 0;
    //gets sum of all values
    for(int& item : vector){
        sum += item;
    }
    int secondSum  = 0;
    sum /= 2;
    for(int i = 0; i < vector.size();  i++){
        secondSum += vector[i];
        //when second sum reaches half the sum return index
        if(secondSum >= sum){
            return i;
        }
    }
    return -1;
}

//////////////////////////////////////////////////////
///Find Low:
///     get the lowest index that is non-zero
///
/// \param vector - vector to find low index of
/// \return index of first non-zero value
///////////////////////////////////////////////////////

int Picture::findLow(std::vector<int> &vector) {
    //check if vector is not empty before looping
    if(!vector.empty()) {
        for (int i = 0; i < vector.size(); i++) {
            if (vector[i] != 0) {
                return i;
            }
        }
    }
    return -1;
}

/////////////////////////////////////////////////////
///Find High:
///     returns the highest non-zero value in vector
///
/// \param vector - vector to find high value
/// \return highest non-zero values index
///////////////////////////////////////////////////


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

//////////////////////////////////////////////////////
///set Pixel Average:
///     sets the pixels intensity. (static)
///
/// \param pix - pixel to set average of
/// \param average - desired intensity of pixel
////////////////////////////////////////////////////

void Picture::setPixAverage(Pixel &pix, int average) {

    //gets constant scaler
    float scaler = (float)(3 * average)/ (float)(pix.r + pix.g + pix.b);

    //sets pixel to that value * the scaler
    pix = {checkPixBound(pix.r * scaler),
           checkPixBound(pix.g * scaler),
           checkPixBound(pix.b * scaler)};

}


//////////////////////////////////////////////////////////////
///Check Pixel Bounds:
///     checks to see if the value is an over saturated value
/// \param value to check
/// \return value thats not over saturated
//////////////////////////////////////////////////////////////

sf::Uint8 Picture::checkPixBound(int value) {
    if(value > 255){
        return 255;
    }else{
        return value;
    }
}
