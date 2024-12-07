//
// Created by gluckasz on 12/7/24.
//

#include "../include/ImageProcessor.h"

cv::Mat ImageProcessor::dilation(cv::Mat image, int maskNumber) {
    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMap.find(maskNumber)->second;
    int maxMaskYSize = 0;
    for (const auto & i : mask) {
        maxMaskYSize = std::max(static_cast<int>(i.size() - 1), maxMaskYSize);
    }

    for (int x = 0; x < image.rows - mask.size(); x++) {
        for (int y = 0; y < image.cols - maxMaskYSize; y++) {
            int xMarkerOffset = 0;
            int yMarkerOffset = 0;
            uchar max = 0;
            for (int i = 0; i < mask.size(); i++) {
                for (int j = 0; j < mask[i].size(); j++) {
                    if (mask[i][j] == FieldType::BLACK_MARKER) {
                        xMarkerOffset = i;
                        yMarkerOffset = j;
                        max = std::max(image.at<uchar>(x + i, y + j), max);
                    } else if (mask[i][j] == FieldType::WHITE_MARKER) {
                        xMarkerOffset = i;
                        yMarkerOffset = j;
                    }  else if (mask[i][j] == FieldType::BLACK) {
                        max = std::max(image.at<uchar>(x + i, y + j), max);
                    }
                }
            }
            result.at<uchar>(x + xMarkerOffset, y + yMarkerOffset) = max;
        }
    }

    return result;
}

cv::Mat ImageProcessor::erosion(cv::Mat image, int maskNumber) {
    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMap.find(maskNumber)->second;
    int maxMaskYSize = 0;
    for (const auto & i : mask) {
        maxMaskYSize = std::max(static_cast<int>(i.size() - 1), maxMaskYSize);
    }
    for (int x = 0; x < image.rows - mask.size(); x++) {
        for (int y = 0; y < image.cols - maxMaskYSize; y++) {
            int xMarkerOffset = 0;
            int yMarkerOffset = 0;
            uchar min = 255;
            for (int i = 0; i < mask.size(); i++) {
                for (int j = 0; j < mask[i].size(); j++) {
                    if (mask[i][j] == FieldType::BLACK_MARKER) {
                        xMarkerOffset = i;
                        yMarkerOffset = j;
                        min = std::min(image.at<uchar>(x + i, y + j), min);
                    } else if (mask[i][j] == FieldType::WHITE_MARKER) {
                        xMarkerOffset = i;
                        yMarkerOffset = j;
                    } else if (mask[i][j] == FieldType::BLACK) {
                        min = std::min(image.at<uchar>(x + i, y + j), min);
                    }
                }
            }
            result.at<uchar>(x + xMarkerOffset, y + yMarkerOffset) = min;
        }
    }

    return result;
}
