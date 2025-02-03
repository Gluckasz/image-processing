//
// Created by gluckasz on 2/3/25.
//

#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H
#include <string>
#include <opencv2/opencv.hpp>
#include <filesystem>


class OutputManager {
public:
    static void saveImage(const cv::Mat &image, const std::string &filename);

    static std::string constructPath(const std::string &baseName, const std::string &suffix,
                                     const std::string &extension);
};


#endif //OUTPUTMANAGER_H
