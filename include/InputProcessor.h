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
    constexpr std::string OUTPUT_DIR_NAME = "output";
    cv::ImreadModes imreadMode = cv::IMREAD_COLOR;
    std::optional<int> brightnessModVal;
    std::optional<int> contrastLinearModVal;
    std::optional<float> contrastGammaModVal;
    bool isNegative = false;
    bool isHorizontalFlip = false;
    bool isVerticalFlip = false;
    bool isDiagonalFlip = false;
    std::optional<float> shrinkModVal;
    std::optional<float> enlargeModVal;
    std::optional<int> midpointKernelSize;
    std::optional<int> arithmeticMeanKernelSize;
    bool isNoNoise = false;
    std::string noNoiseImage;
    bool isMeanSquareError = false;
    bool isPeakMeanSquareError = false;
    bool isSignalToNoise = false;
    bool isPeakSignalToNoise = false;
    bool isMaximumDifference = false;
    std::optional<int> histogramChannel;

    enum class CommandType {
        HELP,
        OUTPUT,
        GRAYSCALE,
        BRIGHTNESS,
        CONTRAST_LINEAR,
        CONTRAST_GAMMA,
        NEGATIVE,
        HORIZONTAL_FLIP,
        VERTICAL_FLIP,
        DIAGONAL_FLIP,
        SHRINK,
        ENLARGE,
        MIDPOINT_FILTER,
        ARITHMETIC_MEAN_FILTER,
        NO_NOISE_IMAGE,
        MSE,
        PMSE,
        SNR,
        PSNR,
        MAXIMUM_DIFFERENCE,
        HISTOGRAM,
        UNKNOWN // For unrecognized commands
     };

    const std::unordered_map<std::string, CommandType> commandMap = {
      {"--help", CommandType::HELP},
      {"--output", CommandType::OUTPUT},
      {"--grayscale", CommandType::GRAYSCALE},
      {"--brightness", CommandType::BRIGHTNESS},
      {"--contrastLinear", CommandType::CONTRAST_LINEAR},
      {"--contrastGamma", CommandType::CONTRAST_GAMMA},
      {"--negative", CommandType::NEGATIVE},
      {"--hflip", CommandType::HORIZONTAL_FLIP},
      {"--vflip", CommandType::VERTICAL_FLIP},
      {"--dflip", CommandType::DIAGONAL_FLIP},
      {"--shrink", CommandType::SHRINK},
      {"--enlarge", CommandType::ENLARGE},
      {"--mid", CommandType::MIDPOINT_FILTER},
      {"--amean", CommandType::ARITHMETIC_MEAN_FILTER},
      {"--noNoiseImage", CommandType::NO_NOISE_IMAGE},
      {"--mse", CommandType::MSE},
      {"--pmse", CommandType::PMSE},
      {"--snr", CommandType::SNR},
      {"--psnr", CommandType::PSNR},
      {"--md", CommandType::MAXIMUM_DIFFERENCE},
      {"--histogram", CommandType::HISTOGRAM}
    };

    const std::unordered_map<CommandType, std::string> commandToStringMap = {
      {CommandType::HELP, "--help"},
      {CommandType::OUTPUT, "--output"},
      {CommandType::GRAYSCALE, "--grayscale"},
      {CommandType::BRIGHTNESS, "--brightness"},
      {CommandType::CONTRAST_LINEAR, "--contrastLinear"},
      {CommandType::CONTRAST_GAMMA, "--contrastGamma"},
      {CommandType::NEGATIVE, "--negative"},
      {CommandType::HORIZONTAL_FLIP, "--hflip"},
      {CommandType::VERTICAL_FLIP, "--vflip"},
      {CommandType::DIAGONAL_FLIP, "--dflip"},
      {CommandType::SHRINK, "--shrink"},
      {CommandType::ENLARGE, "--enlarge"},
      {CommandType::MIDPOINT_FILTER, "--mid"},
      {CommandType::ARITHMETIC_MEAN_FILTER, "--amean"},
      {CommandType::NO_NOISE_IMAGE, "--noNoiseImage"},
      {CommandType::MSE, "--mse"},
      {CommandType::PMSE, "--pmse"},
      {CommandType::SNR, "--snr"},
      {CommandType::PSNR, "--psnr"},
      {CommandType::MAXIMUM_DIFFERENCE, "--md"},
      {CommandType::HISTOGRAM, "--histogram"}
    };

    /**
     * Prints the information about all the available functions.
    */
    void printCommands();

    /**
     * Reads an integer parameter to one command-line command.
     * @param i Position of parameter to read.
     * @param modVal Optional integer parameter to read. Passed by reference.
    */
    void readIntParam(int i, std::optional<int> &modVal);

    /**
     * Reads a float parameter to one command-line command.
     * @param i Position of parameter to read.
     * @param modVal Optional float parameter to read. Passed by reference.
    */
    void readFloatParam(int i, std::optional<float> &modVal);

    void readStringParam(int i, std::string &paramVal);

    void applyImageTransformations(cv::Mat &image, std::unique_ptr<ImageProcessor> &imageProcessor) const;

    void InputProcessor::calculateAndSaveImageStatistics(
        const cv::Mat &compareImage,
        const cv::Mat &originalImage,
        const cv::Mat &newImage,
        std::unique_ptr<ImageProcessor> &imageProcessor) const;

    /**
     * Saves given image to the output/outputFileName file.
    */
    void saveImage(cv::Mat image, std::string outputFileName) const;
};



#endif //INPUTPROCESSOR_H
