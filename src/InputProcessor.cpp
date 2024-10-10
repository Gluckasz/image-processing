//
// Created by gluckasz on 10/9/24.
//

#include "../include/InputProcessor.h"

InputProcessor::InputProcessor(int argc, char **argv) {
    this->argc = argc;
    this->argv = argv;
}


void InputProcessor::printCommands() {
        std::cout << "Available commands:\n"
        << "--help - print list of all the available commands with detailed description of their arguments.\n\n"
        << "--output [-fileName=value] - provide output file name.\n"
        << "\t -fileName - output file name (default is output.bmp).\n\n"
        << "--grayscale - read image in grayscale.\n\n"
        << "--brightness [-modVal=value] - modify brightness of an image.\n"
        << "\t -modVal - integral value to add to each pixel (can be negative).\n\n"
        << "--contrastLinear[-modVal=value] - modify contrast of an image using linear contrast stretching.\n"
        << "\t -modVal - new value added and subtracted from upper and lower boundary.\n\n"
        << "--contrastGamma [-modVal=value] - modify contrast of an image using gamma correction.\n"
        << "\t -modVal - floating-point value of gamma variable.\n\n"
        << "--negative - take negative of an image.\n\n"
        << "--hflip - flip the image horizontally.\n\n"
        << "--vflip - flip the image vertically.\n\n"
        << "--shrink [-modVal=value] - shrink an image using nearest Neighbor Method.\n"
        << "\t -modVal - floating-point scale factor of new image (has to be between 0 and 1 non-inclusive).\n\n"
        << "--enlarge [-modVal=value] - enlarge an image using nearest Neighbor Method.\n"
        << "\t -modVal - floating-point scale factor of new image (has to be greater than 1).\n\n"
        << "--mid - apply midpoint filter.\n\n"
        << "--amean - apply arithmetic mean filter.\n\n"
        <<"--noNoiseImage [-fileName=value] - provide image with no noise to compare with noisy and denoised image.\n"
        << "\t -fileName - file name of the image with no noise.\n\n"
        << "--mse - compute mean square error.\n\n"
        << "--pmse - compute peak mean square error.\n\n";
}

void InputProcessor::readIntParam(int i, bool &isModified, int &modVal) {
    std::string paramVal = argv[i];
    if (paramVal.substr(0, std::strlen("-modVal=")) != "-modVal=") {
        std::cout << "The parameter has to be of format: -modVal=[value].";
        isModified = false;
    }
    try {
        modVal = stoi(paramVal.substr(paramVal.find('=') + 1));
    }
    catch (const std::invalid_argument & e) {
        std::cout << "-modVal has to be an integer.";
        isModified = false;
    }
    catch (const std::out_of_range & e) {
        std::cout << "-modVal is out of bounds.";
        isModified = false;
    }
}

void InputProcessor::readFloatParam(int i, bool &isModified, float &modVal) {
    std::string paramVal = argv[i];
    if (paramVal.substr(0, std::strlen("-modVal=")) != "-modVal=") {
        std::cout << "The parameter has to be of format: -modVal=[value].";
        isModified = false;
    }
    try {
        modVal = stof(paramVal.substr(paramVal.find('=') + 1));
    }
    catch (const std::invalid_argument & e) {
        std::cout << "-modVal has to be an integer.";
        isModified = false;
    }
    catch (const std::out_of_range & e) {
        std::cout << "-modVal is out of bounds.";
        isModified = false;
    }
}

void InputProcessor::readStringParam(int i, std::string &paramVal) {
    std::string tryParamVal = argv[i];
    if (tryParamVal.substr(0, std::strlen("-fileName=")) != "-fileName=") {
        std::cout << "The parameter has to be of format: -fileName=[value].";
    }
    else {
        paramVal = tryParamVal.substr(tryParamVal.find('=') + 1);
    }
}



void InputProcessor::processInput() {
    for (int i = 1; i < argc; i++) {
        if (static_cast<std::string>(argv[i]) == "--help") {
            printCommands();
            return;
        }
        if (static_cast<std::string>(argv[i]) == "--output") {
            readStringParam(++i, outputFileName);
        }
        else if (static_cast<std::string>(argv[i]) == "--grayscale") {
            imreadMode = cv::IMREAD_GRAYSCALE;
        }
        else if (static_cast<std::string>(argv[i]) == "--brightness") {
            isBrightnessModified = true;
            readIntParam(++i, isBrightnessModified, brightnessModVal);
        }
        else if (static_cast<std::string>(argv[i]) == "--contrastLinear") {
            isContrastLinearModified = true;
            readIntParam(++i, isContrastLinearModified, contrastLinearModVal);
        }
        else if (static_cast<std::string>(argv[i]) == "--contrastGamma") {
            isContrastGammaModified = true;
            readFloatParam(++i, isContrastGammaModified, contrastGammaModVal);
        }
        else if (static_cast<std::string>(argv[i]) == "--negative") {
            isNegative = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--hflip") {
            isHorizontalFlip = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--vflip") {
            isVerticalFlip = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--dflip") {
            isDiagonalFlip = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--shrink") {
            isShrink = true;
            readFloatParam(++i, isShrink, shrinkModVal);
        }
        else if (static_cast<std::string>(argv[i]) == "--enlarge") {
            isEnlarged = true;
            readFloatParam(++i, isEnlarged, enlargeModVal);
        }
        else if (static_cast<std::string>(argv[i]) == "--mid") {
            isMidpointFilter = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--amean") {
            isArithmeticMeanFilter = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--noNoiseImage") {
            readStringParam(++i, noNoiseImage);
            isNoNoise = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--mse") {
            isMeanSquareError = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--pmse") {
            isPeakMeanSquareError = true;
        }
    }
}

void InputProcessor::processImage() const {
    cv::Mat originalImage = imread(argv[INPUT_IMAGE_POS], imreadMode);
    if (!originalImage.data) {
        std::cout << "No image data \n";
        return;
    }
    std::unique_ptr<ImageProcessor> imageProcessor;
    if (imreadMode == cv::IMREAD_COLOR) {
        imageProcessor = std::make_unique<RGBImageProcessor>();
    }
    else {
        imageProcessor = std::make_unique<GrayscaleImageProcessor>();
    }

    cv::Mat newImage;
    originalImage.copyTo(newImage);
    if (isBrightnessModified) {
        newImage = imageProcessor->modifyBrightness(newImage, brightnessModVal);
    }
    if (isContrastLinearModified) {
        newImage = imageProcessor->mofifyContrastLinear(newImage, contrastLinearModVal);
    }
    if (isContrastGammaModified) {
        newImage = imageProcessor->modifyContrastGamma(newImage, contrastGammaModVal);
    }
    if (isNegative) {
        newImage = imageProcessor->negative(newImage);
    }
    if (isHorizontalFlip) {
        newImage = imageProcessor->flipHorizontally(newImage);
    }
    if (isVerticalFlip) {
        newImage = imageProcessor->flipVertically(newImage);
    }
    if (isDiagonalFlip) {
        newImage = imageProcessor->flipDiagonally(newImage);
    }
    if (isShrink) {
        newImage = imageProcessor->resize(newImage, shrinkModVal);
    }
    if (isEnlarged) {
        newImage = imageProcessor->resize(newImage, enlargeModVal);
    }
    if (isMidpointFilter) {
        newImage = imageProcessor->midpointFilter(newImage);
    }
    if (isArithmeticMeanFilter) {
        newImage = imageProcessor->arithmeticMeanFilter(newImage);
    }
    saveImage(newImage);

    if (isNoNoise) {
        cv::Mat compareImage = imread(noNoiseImage, imreadMode);
        std::stringstream ss;
        ss << "Image stats:\n";
        if (isMeanSquareError) {
            ss << imageProcessor->meanSquareError(compareImage, originalImage, newImage);
        }
        if (isPeakMeanSquareError) {
            ss << imageProcessor->peakMeanSquareError(compareImage, originalImage, newImage);
        }
        std::ofstream statsFile;
        statsFile.open("output/" + outputFileName.substr(0, outputFileName.length() - 4) + ".txt");
        statsFile << ss.rdbuf();
        statsFile.close();
    }
}


void InputProcessor::saveImage(cv::Mat image) const {
    if (!std::filesystem::is_directory("output") || !std::filesystem::exists("output")) {
        std::filesystem::create_directory("output");
    }
    imwrite("output/" + outputFileName, image);
}
