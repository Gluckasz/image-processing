//
// Created by gluckasz on 12/7/24.
//

#include "../include/ImageProcessor.h"
cv::Mat ImageProcessor::complement(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            result.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
        }
    }

    return result;
}



const std::unordered_map<int, std::vector<std::vector<ImageProcessor::FieldType>>> ImageProcessor::maskMap = {
    {1, {{FieldType::BLACK_MARKER, FieldType::BLACK}}},
    {2, {
        {FieldType::BLACK_MARKER},
        {FieldType::BLACK}
    }},
    {3, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
        {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::BLACK},
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {4, {
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::INACTIVE}
    }},
    {5, {
        {FieldType::BLACK_MARKER, FieldType::BLACK},
        {FieldType::BLACK, FieldType::INACTIVE}
    }},
    {6, {
        {FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::BLACK, FieldType::INACTIVE}
    }},
    {7, {{FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::BLACK}}},
    {8, {{FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::BLACK}}},
    {9, {
        {FieldType::BLACK, FieldType::BLACK_MARKER},
        {FieldType::BLACK, FieldType::INACTIVE}
    }},
    {10, {
        {FieldType::BLACK, FieldType::BLACK},
        {FieldType::BLACK_MARKER , FieldType::INACTIVE}
    }},
};

const std::unordered_map<int, std::vector<std::vector<ImageProcessor::FieldType>>> ImageProcessor::hmtMaskMap = {
    {1, {
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {2, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {3, {
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK}
    }},
    {4, {
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {5, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {6, {
        {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
    }},
    {7, {
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
        {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
    }},
    {8, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
    }},
    {9, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {10, {
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
        {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
    }},
    {11, {
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
        {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
    }},
    {12, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
    }},
};

const std::unordered_map<int, std::vector<std::vector<ImageProcessor::FieldType>>> ImageProcessor::hmtComplementMaskMap = {
    {1, {
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {2, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {3, {
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE}
    }},
    {4, {
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {5, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {6, {
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
    }},
    {7, {
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
    }},
    {8, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
    }},
    {9, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {10, {
        {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
    }},
    {11, {
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
    }},
    {12, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
    }},
};

cv::Mat ImageProcessor::dilation(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMapping.find(maskNumber)->second;
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

cv::Mat ImageProcessor::erosion(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMapping.find(maskNumber)->second;
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

cv::Mat ImageProcessor::opening(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat result = image.clone();
    result = this->erosion(result, maskNumber, maskMapping);
    result = this->dilation(result, maskNumber, maskMapping);
    return result;
}

cv::Mat ImageProcessor::closing(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat result = image.clone();
    result = this->dilation(result, maskNumber, maskMapping);
    result = this->erosion(result, maskNumber, maskMapping);
    return result;
}

cv::Mat ImageProcessor::hmt(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat foregroundMatch = image.clone();
    cv::Mat backgroundMatch = image.clone();

    foregroundMatch = this->erosion(foregroundMatch, maskNumber, maskMapping);
    backgroundMatch = this->complement(backgroundMatch);

    backgroundMatch = this->erosion(backgroundMatch, maskNumber, hmtComplementMaskMap);

    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    for (int x = 0; x < foregroundMatch.rows; x++) {
        for (int y = 0; y < foregroundMatch.cols; y++) {
            if (foregroundMatch.at<uchar>(x, y) == UCHAR_MAX && backgroundMatch.at<uchar>(x, y) == UCHAR_MAX) {
                result.at<uchar>(x, y) = UCHAR_MAX;
            }
        }
    }

    return result;
}

