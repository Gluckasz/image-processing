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
    << commandToStringMap.find(CommandType::HELP)->second
        << " - print list of all the available commands with detailed description of their arguments.\n\n"
    << commandToStringMap.find(CommandType::OUTPUT)->second
        << "[-fileName=value] - provide output file name.\n"
        << "\t -fileName - output file name (default is output.bmp).\n\n"
    << commandToStringMap.find(CommandType::GRAYSCALE)->second
        << " - read image in grayscale.\n\n"
    << commandToStringMap.find(CommandType::BRIGHTNESS)->second
        << "[-val=value] - modify brightness of an image.\n"
        << "\t -val - integral value to add to each pixel (can be negative).\n\n"
    << commandToStringMap.find(CommandType::CONTRAST_LINEAR)->second
        << "[-val=value] - modify contrast of an image using linear contrast stretching.\n"
        << "\t -val - new value added and subtracted from upper and lower boundary.\n\n"
    << commandToStringMap.find(CommandType::CONTRAST_GAMMA)->second
        << "[-val=value] - modify contrast of an image using gamma correction.\n"
        << "\t -val - floating-point value of gamma variable.\n\n"
    << commandToStringMap.find(CommandType::NEGATIVE)->second
        << " - take negative of an image.\n\n"
    << commandToStringMap.find(CommandType::HORIZONTAL_FLIP)->second
        << " - flip the image horizontally.\n\n"
    << commandToStringMap.find(CommandType::VERTICAL_FLIP)->second
        << " - flip the image vertically.\n\n"
    << commandToStringMap.find(CommandType::DIAGONAL_FLIP)->second
        << " - flip the image diagonally.\n\n"
    << commandToStringMap.find(CommandType::SHRINK)->second
        << "[-val=value] - shrink an image using nearest Neighbor Method.\n"
        << "\t -val - floating-point scale factor of new image (has to be between 0 and 1 non-inclusive).\n\n"
    << commandToStringMap.find(CommandType::ENLARGE)->second
        << "[-val=value] - enlarge an image using nearest Neighbor Method.\n"
        << "\t -val - floating-point scale factor of new image (has to be greater than 1).\n\n"
    << commandToStringMap.find(CommandType::MIDPOINT_FILTER)->second
        << "[-val=value] - apply midpoint filter.\n"
        << "\t -val - integer kernel size value.\n\n"
    << commandToStringMap.find(CommandType::ARITHMETIC_MEAN_FILTER)->second
        << " - apply arithmetic mean filter.\n"
        << "\t -val - integer kernel size value.\n\n"
    << commandToStringMap.find(CommandType::NO_NOISE_IMAGE)->second
        <<"[-fileName=value] - provide image with no noise to compare with noisy and denoised image.\n"
        << "\t -fileName - file name of the image with no noise.\n\n"
    << commandToStringMap.find(CommandType::MSE)->second
        << "- compute mean square error.\n\n"
    << commandToStringMap.find(CommandType::PMSE)->second
        << " - compute peak mean square error.\n\n"
    << commandToStringMap.find(CommandType::SNR)->second
        << " - compute signal to noise ratio.\n\n"
    << commandToStringMap.find(CommandType::PSNR)->second
        << " - compute peak signal to noise ratio.\n\n"
    << commandToStringMap.find(CommandType::MAXIMUM_DIFFERENCE)->second
        << " - compute maximum difference.\n\n"
    << commandToStringMap.find(CommandType::HISTOGRAM)->second
        << " - save histogram of an image to output_file_name_histogram.bmp\n"
        << "\t -val - channel of an image for which to compute and save histogram. "
            << "For grayscale images channel does not matter.\n\n";

}

void InputProcessor::readIntParam(int i, bool &isModified, int &modVal) {
    std::string paramVal = argv[i];
    std::string paramName = "-val=";
    if (paramVal.substr(0, paramName.length()) != paramName) {
        std::cout << "The parameter has to be of format: " + paramName + "[value].";
        isModified = false;
    }
    try {
        modVal = stoi(paramVal.substr(paramVal.find('=') + 1));
    }
    catch (const std::invalid_argument & e) {
        std::cout << paramName + " has to be an integer.";
        isModified = false;
    }
    catch (const std::out_of_range & e) {
        std::cout << paramName + " is out of bounds.";
        isModified = false;
    }
}

void InputProcessor::readFloatParam(int i, bool &isModified, float &modVal) {
    std::string paramVal = argv[i];
    std::string paramName = "-val=";
    if (paramVal.substr(0, paramName.length()) != paramName) {
        std::cout << "The parameter has to be of format: " +  paramName + "[value].";
        isModified = false;
    }
    try {
        modVal = stof(paramVal.substr(paramVal.find('=') + 1));
    }
    catch (const std::invalid_argument & e) {
        std::cout << paramName + " has to be a float.";
        isModified = false;
    }
    catch (const std::out_of_range & e) {
        std::cout << paramName + " is out of bounds.";
        isModified = false;
    }
}

void InputProcessor::readStringParam(int i, std::string &paramVal) {
    std::string tryParamVal = argv[i];
    std::string paramName = "--fileName=";
    if (tryParamVal.substr(0, paramName.length()) != paramName) {
        std::cout << "The parameter has to be of format: " + paramName + "[value].";
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
            readStringParam(++i, this->outputFileName);
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
            readIntParam(++i, isMidpointFilter, midpointKernelSize);
        }
        else if (static_cast<std::string>(argv[i]) == "--amean") {
            isArithmeticMeanFilter = true;
            readIntParam(++i, isArithmeticMeanFilter, arithmeticMeanKernelSize);
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
        else if (static_cast<std::string>(argv[i]) == "--snr") {
            isSignalToNoise = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--psnr") {
            isPeakSignalToNoise = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--md") {
            isMaximumDifference = true;
        }
        else if (static_cast<std::string>(argv[i]) == "--histogram") {
            isHistogram = true;
            readIntParam(++i, isHistogram, histogramChannel);
            if (histogramChannel < 0 || histogramChannel > 2) {
                std::cout << " Histogram channel parameter must be between 0 and 2. Skipping histogram creation.";
                isHistogram = false;
            }
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
        newImage = imageProcessor->midpointFilter(newImage, midpointKernelSize);
    }
    if (isArithmeticMeanFilter) {
        newImage = imageProcessor->arithmeticMeanFilter(newImage, arithmeticMeanKernelSize);
    }
    if (isHistogram) {
        std::string histogramFileName = outputFileName.substr(0, outputFileName.length()- 4) + "_histogram.png";
        saveImage(imageProcessor->histogram(newImage, histogramChannel), histogramFileName);
    }
    saveImage(newImage, this->outputFileName);

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
        if (isSignalToNoise) {
            ss << imageProcessor->signalToNoiseRatio(compareImage, originalImage, newImage);
        }
        if (isPeakSignalToNoise) {
            ss << imageProcessor->peakSignalToNoiseRatio(compareImage, originalImage, newImage);
        }
        if (isMaximumDifference) {
            ss << imageProcessor->maximumDifference(compareImage, originalImage, newImage);
        }
        std::ofstream statsFile;
        statsFile.open("output/" + outputFileName.substr(0, outputFileName.length() - 4) + ".txt");
        statsFile << ss.rdbuf();
        statsFile.close();
    }
}


void InputProcessor::saveImage(cv::Mat image, std::string outputFileName) const {
    if (!std::filesystem::is_directory("output") || !std::filesystem::exists("output")) {
        std::filesystem::create_directory("output");
    }
    imwrite("output/" + outputFileName, image);
}
