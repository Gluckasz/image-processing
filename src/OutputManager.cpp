//
// Created by gluckasz on 2/3/25.
//

#include "../include/OutputManager.h"
#include "Constants.h"


void OutputManager::saveImage(const cv::Mat &image, const std::string &filename) {
    if (!std::filesystem::is_directory(Constants::DEFAULT_OUTPUT_DIR) || !
        std::filesystem::exists(Constants::DEFAULT_OUTPUT_DIR)) {
        std::filesystem::create_directory(Constants::DEFAULT_OUTPUT_DIR);
    }
    imwrite(filename, image);
}

std::string OutputManager::constructPath(const std::string &baseName, const std::string &suffix,
                                         const std::string &extension) {
    return Constants::DEFAULT_OUTPUT_DIR + '/' + baseName + '_' + suffix + '.' + extension;
}
