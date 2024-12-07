//
// Created by gluckasz on 12/7/24.
//

#include "../include/ImageProcessor.h"

cv::Mat ImageProcessor::dilation(cv::Mat image, int maskNumber) {
    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMap.find(maskNumber)->second;
    int maxMaskYSize = 0;
    for (int i = 0; i < mask.size(); i++) {
        maxMaskYSize = std::max(static_cast<int>(mask[i].size()), maxMaskYSize);
    }

    for (int x = 0; x < image.rows - mask.size() / 2; x++) {
        for (int y = 0; y < image.cols - maxMaskYSize / 2; y++) {
            int xMarkerOffset = 0;
            int yMarkerOffset = 0;
            uchar max = 0;
            for (int i = 0; i < mask.size(); i++) {
                for (int j = 0; j < mask[i].size(); j++) {
                    if (mask[i][j] == FieldType::BLACK_MARKER) {
                        xMarkerOffset = i;
                        yMarkerOffset = j;
                        max = std::max(image.at<uchar>(x + i, y + j), max);
                    } else if (mask[i][j] == FieldType::BLACK) {
                        max = std::max(image.at<uchar>(x + i, y + j), max);
                    }
                }
            }
            result.at<uchar>(x + xMarkerOffset, y + yMarkerOffset) = max;
        }
    }

    return result;
}
