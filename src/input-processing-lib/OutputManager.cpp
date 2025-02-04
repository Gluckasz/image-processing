//
// Created by gluckasz on 2/3/25.
//

#include "../../include/input-processing-lib/OutputManager.h"

#include <fstream>

#include "../../include/Constants.h"


void OutputManager::saveImage(const cv::Mat &image, const std::string &filePath) {
    if (!std::filesystem::is_directory(Constants::DEFAULT_OUTPUT_DIR) || !
        std::filesystem::exists(Constants::DEFAULT_OUTPUT_DIR)) {
        std::filesystem::create_directory(Constants::DEFAULT_OUTPUT_DIR);
    }
    imwrite(filePath, image);
}

void OutputManager::saveFile(const std::string &filePath, const std::stringstream &contents) {
    if (!std::filesystem::is_directory(Constants::DEFAULT_OUTPUT_DIR) || !
        std::filesystem::exists(Constants::DEFAULT_OUTPUT_DIR)) {
        std::filesystem::create_directory(Constants::DEFAULT_OUTPUT_DIR);
        }
    std::ofstream statsFile;
    statsFile.open(filePath);
    statsFile << contents.rdbuf();
    statsFile.close();
}

std::string OutputManager::constructPath(const std::string &baseName, const std::string &suffix,
                                         const std::string &extension) {
    return Constants::DEFAULT_OUTPUT_DIR + '/' + baseName + '_' + suffix + '.' + extension;
}
