//
// Created by gluckasz on 10/9/24.
//

#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <string>
#include <cstring>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>

#include "../include/ImageProcessor.h"
#include "../include/RGBImageProcessor.h"
#include "../include/GrayscaleImageProcessor.h"

// Constants
constexpr int INPUT_IMAGE_POS = 1;

class InputProcessor {
public:
    InputProcessor(int argc, char **argv);
    // Process user input from command-line.
    void processInput();
    void processImage() const;
private:
    // Command-line input variables
    int argc;
    char **argv;
    std::string outputFileName = "output.bmp";
    cv::ImreadModes imreadMode = cv::IMREAD_COLOR;
    bool isBrightnessModified = false;
    int brightnessModVal;
    bool isContrastLinearModified = false;
    int contrastLinearModVal;
    bool isContrastGammaModified = false;
    float contrastGammaModVal;
    bool isNegative = false;
    bool isHorizontalFlip = false;
    bool isVerticalFlip = false;
    bool isDiagonalFlip = false;
    bool isShrink = false;
    float shrinkModVal;
    bool isEnlarged = false;
    float enlargeModVal;
    bool isMidpointFilter = false;
    bool isArithmeticMeanFilter = false;
    bool isNoNoise = false;
    std::string noNoiseImage;
    bool isMeanSquareError = false;
    bool isPeakMeanSquareError = false;
    bool isSignalToNoise = false;
    bool isPeakSignalToNoise = false;
    bool isMaximumDifference = false;

    /**
     * Prints the information about all the available functions.
    */
    static void printCommands();
    /**
     * Reads an integer parameter to one command-line command.
     * @param i Position of parameter to read.
     * @param isModified Boolean value indicating whether to use selected the command. Passed by reference.
     * @param modVal Integer parameter to read. Passed by reference.
    */
    void readIntParam(int i, bool &isModified, int &modVal);
    /**
     * Reads a float parameter to one command-line command.
     * @param i Position of parameter to read.
     * @param isModified Boolean value indicating whether to use selected the command. Passed by reference.
     * @param modVal Float parameter to read. Passed by reference.
    */
    void readFloatParam(int i, bool &isModified, float &modVal);
    void readStringParam(int i, std::string &paramVal);
    /**
     * Saves given image to the output/outputFileName file.
    */
    void saveImage(cv::Mat image) const;
};



#endif //INPUTPROCESSOR_H
