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
#include <chrono>

#include "../include/SpatialDomainProcessor.h"
#include "../include/FourierProcessor.h"
#include "../include/HistogramProcessor.h"
#include "../include/MorphologicalProcessor.h"
#include "../include/ImageComparer.h"

constexpr int INPUT_IMAGE_POS = 1;

/**
 * @brief Processes command-line input for image manipulation operations.
 *
 * This class handles parsing and validation of command-line arguments,
 * manages image processing operations, and coordinates output generation.
 */
class InputProcessor {
public:
    /**
     * @brief Construct a new Input Processor object
     * @param argc Number of command-line arguments
     * @param argv Array of command-line argument strings
     */
    InputProcessor(int argc, char **argv);

    /**
     * @brief Process command-line arguments and set corresponding flags
     */
    void processInput();

    /**
     * @brief Execute image processing operations based on set flags
     */
    void processImage();

private:
    // Command-line input state
    int argc;
    char **argv;

    /**
     * @brief Print usage information and available commands
     */
    void printCommands();

    /**
     * @brief Apply selected image transformations to the input image
     */
    void applyImageTransformations(cv::Mat &image);

    /**
     * @brief Calculate and save image comparison statistics
     */
    void calculateAndSaveComparisonImageStatistics(
        const cv::Mat &compareImage,
        const cv::Mat &originalImage,
        const cv::Mat &newImage
    );

    /**
     * @brief Calculate and save image statistics
     */
    void calculateAndSaveImageStats(
        const cv::Mat &newImage
    );

    /**
     * @brief Save processed image to output directory
     * @param image Image to save
     * @param outputFileName Name of output file
     */
    void saveImage(cv::Mat image, std::string outputFileName) const;

    /**
     * @brief Apply Fast Fourier Transform and save visualization
     * @param image Input image
     * @param fourierVisPath Path to save FFT visualization
     * @param imageProcessor Image processor instance
     * @return Transformed image
     */
    cv::Mat applyFastFourier(const cv::Mat& image, const std::string &fourierVisPath) const;
};


#endif //INPUTPROCESSOR_H
