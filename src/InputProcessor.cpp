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
        << "--output [-outputFileName=value] - provide output file name.\n"
        << "\t -outputFileName - output file name (default is output.bmp).\n\n"
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
        << "--amean - apply arithmetic mean filter.\n\n";
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


void InputProcessor::processInput() {
    for (int i = 1; i < argc; i++) {
        if (static_cast<std::string>(argv[i]) == "--help") {
            printCommands();
            return;
        }
        if (static_cast<std::string>(argv[i]) == "--output") {
            std::string paramVal = argv[++i];
            if (paramVal.substr(0, std::strlen("-outputFileName=")) != "-outputFileName=") {
                std::cout << "The --output parameter has to be of format: -outputFileName=[value]. Skipping outputFileName modification.";
            }
            else {
                outputFileName = paramVal.substr(paramVal.find('=') + 1);
            }
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
    }
}

void InputProcessor::processImage() const {
    cv::Mat image = imread(argv[INPUT_IMAGE_POS], imreadMode);
    if (!image.data) {
        std::cout << "No image data \n";
        return;
    }
    std::unique_ptr<ImageProcessor> imageProcessor;
    if (imreadMode == IMREAD_COLOR) {
        imageProcessor = std::make_unique<RGBImageProcessor>(
            image,
            brightnessModVal,
            contrastLinearModVal,
            contrastGammaModVal
            );
    }
    else {
        imageProcessor = std::make_unique<GrayscaleImageProcessor>(
            image,
            brightnessModVal,
            contrastLinearModVal,
            contrastGammaModVal
            );
    }

    if (isBrightnessModified) {
        imageProcessor->modifyBrightness();
    }
    if (isContrastLinearModified) {
        imageProcessor->mofifyContrastLinear();
    }
    if (isContrastGammaModified) {
        imageProcessor->modifyContrastGamma();
    }
    if (isNegative) {
        imageProcessor->negative();
    }
    if (isHorizontalFlip) {
        imageProcessor->flipHorizontally();
    }
    if (isVerticalFlip) {
        imageProcessor->flipVertically();
    }
    if (isDiagonalFlip) {
        imageProcessor->flipDiagonally();
    }
    if (isShrink) {
        image = imageProcessor->resize(shrinkModVal);
    }
    if (isEnlarged) {
        image = imageProcessor->resize(enlargeModVal);
    }
    if (isMidpointFilter) {
        image = imageProcessor->midpointFilter();
    }
    if (isArithmeticMeanFilter) {
        image = imageProcessor->arithmeticMeanFilter();
    }

    saveImage(image);
}


void InputProcessor::saveImage(cv::Mat image) const {
    if (!std::filesystem::is_directory("output") || !std::filesystem::exists("output")) {
        std::filesystem::create_directory("output");
    }
    imwrite("output/" + outputFileName, image);
}
