//
// Created by gluckasz on 10/9/24.
//

#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <string>
#include <opencv2/opencv.hpp>

#include "CommandOptions.h"
#include "ImageOperation.h"

constexpr int INPUT_IMAGE_POS = 1;

/**
 * @brief Processes command-line input for image manipulation operations.
 *
 * This class handles parsing and validation of command-line arguments,
 * manages image processing operations, and coordinates output generation.
 */
class InputProcessor {
    CommandOptions options_;
    std::vector<std::unique_ptr<ImageOperation>> channelOperations_;
    std::vector<std::unique_ptr<ImageOperation>> imageOperations_;
    std::vector<std::unique_ptr<ImageOperation>> statsOperations_;
    std::string inputImagePath_;

    void setupChannelProcessingPipeline();
    void setupImageProcessingPipeline();
    void setupStatsPipeline();
    void saveResults(const cv::Mat& image) const;
    static void printCommands();
public:
    InputProcessor(CommandOptions options, std::string inputImagePath)
        : options_(std::move(options)), inputImagePath_(std::move(inputImagePath)) {}

    void process();
};


#endif //INPUTPROCESSOR_H
