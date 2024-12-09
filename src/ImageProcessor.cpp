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

bool ImageProcessor::areEqual(cv::Mat image1, cv::Mat image2) {
    if (image1.rows != image2.rows || image1.cols != image2.cols) {
        return false;
    }
    for (int x = 2; x < image1.rows - 2; x++) {
        for (int y = 2; y < image1.cols - 2; y++) {
            if (image1.at<uchar>(x, y) != image2.at<uchar>(x, y)) {
                return false;
            }
        }
    }

    return true;
}

cv::Mat ImageProcessor::imagesUnion(cv::Mat image1, cv::Mat image2) {
    cv::Mat result = image1.clone();

    for (int x = 0; x < image1.rows; x++) {
        for (int y = 0; y < image1.cols; y++) {
            result.at<uchar>(x, y) = std::max(image1.at<uchar>(x, y), image2.at<uchar>(x, y));
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

cv::Mat ImageProcessor::dilation(cv::Mat image, int maskNumber,
    std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {

    cv::Mat result = cv::Mat::zeros(image.size(), CV_8UC1);
    std::vector<std::vector<FieldType>> mask = maskMapping.find(maskNumber)->second;

    int markerX = 0, markerY = 0;
    bool foundMarker = false;
    for (int i = 0; i < mask.size() && !foundMarker; i++) {
        for (int j = 0; j < mask[i].size() && !foundMarker; j++) {
            if (mask[i][j] == FieldType::BLACK_MARKER ||
                mask[i][j] == FieldType::WHITE_MARKER) {
                markerX = i;
                markerY = j;
                foundMarker = true;
            }
        }
    }

#pragma omp parallel for collapse(2)
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            if (x + mask.size() > image.rows || y + mask[0].size() > image.cols) {
                continue;
            }

            bool matchFound = false;
            for (int i = 0; i < mask.size() && !matchFound; i++) {
                for (int j = 0; j < mask[i].size() && !matchFound; j++) {
                    if (mask[i][j] == FieldType::INACTIVE) {
                        continue;
                    }
                    if (mask[i][j] == FieldType::BLACK_MARKER ||
                        mask[i][j] == FieldType::BLACK) {
                        if (image.at<uchar>(x + i, y + j) == 0) {
                            matchFound = true;
                        }
                    }
                    else if (mask[i][j] == FieldType::WHITE_MARKER ||
                             mask[i][j] == FieldType::WHITE) {
                        if (image.at<uchar>(x + i, y + j) == 255) {
                            matchFound = true;
                        }
                    }
                }
            }

            if (!matchFound) {
                result.at<uchar>(x + markerX, y + markerY) = 255;
            }
        }
    }

    return result;
}

cv::Mat ImageProcessor::erosion(cv::Mat image, int maskNumber,
    std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {

    cv::Mat result = cv::Mat::zeros(image.size(), CV_8UC1);
    std::vector<std::vector<FieldType>> mask = maskMapping.find(maskNumber)->second;

    int markerX = 0, markerY = 0;
    bool foundMarker = false;
    for (int i = 0; i < mask.size() && !foundMarker; i++) {
        for (int j = 0; j < mask[i].size() && !foundMarker; j++) {
            if (mask[i][j] == FieldType::BLACK_MARKER ||
                mask[i][j] == FieldType::WHITE_MARKER) {
                markerX = i;
                markerY = j;
                foundMarker = true;
            }
        }
    }

#pragma omp parallel for collapse(2)
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            if (x + mask.size() > image.rows || y + mask[0].size() > image.cols) {
                continue;
            }

            bool match = false;
            for (int i = 0; i < mask.size() && !match; i++) {
                for (int j = 0; j < mask[i].size() && !match; j++) {
                    if (mask[i][j] == FieldType::INACTIVE) {
                        continue;
                    }
                    if (mask[i][j] == FieldType::BLACK_MARKER ||
                        mask[i][j] == FieldType::BLACK) {
                        if (image.at<uchar>(x + i, y + j) == 255) {
                            match = true;
                        }
                    }
                    else if (mask[i][j] == FieldType::WHITE_MARKER ||
                             mask[i][j] == FieldType::WHITE) {
                        if (image.at<uchar>(x + i, y + j) == 0) {
                            match = true;
                        }
                    }
                }
            }

            if (match) {
                result.at<uchar>(x + markerX, y + markerY) = 255;
            }
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

    result = cv::Scalar(255);
#pragma omp parallel for collapse(2)
    for (int x = 0; x < foregroundMatch.rows; x++) {
        for (int y = 0; y < foregroundMatch.cols; y++) {
            if (foregroundMatch.at<uchar>(x, y) == UCHAR_MAX && backgroundMatch.at<uchar>(x, y) == UCHAR_MAX) {
                result.at<uchar>(x, y) = 0;
            }
        }
    }

    return result;
}

// I commented out making union with original image, because it caused original image to be returned
cv::Mat ImageProcessor::taskM4(cv::Mat image) {
    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

    for (int i = 1; i <= 4; i++) {
        cv::Mat previousElement = image.clone();
        cv::Mat nextElement = hmt(previousElement, i);
        // nextElement = imagesUnion(nextElement, image);

        while (!areEqual(previousElement, nextElement)) {
            previousElement = nextElement.clone();
            nextElement = hmt(previousElement, i);
            // nextElement = imagesUnion(nextElement, image);
        }
        result = imagesUnion(result, nextElement);
    };
    return result;
}

std::vector<cv::Vec3b> ImageProcessor::createColorMap() {
    std::vector<cv::Vec3b> colorMap(256, cv::Vec3b(0, 0, 0));
    for (int i = 1; i <= 255; ++i) {
        int hue = (i * 179) / 255;
        cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255, 255));
        cv::Mat rgb;
        cv::cvtColor(hsv, rgb, cv::COLOR_HSV2BGR);
        colorMap[i] = rgb.at<cv::Vec3b>(0, 0);
    }
    return colorMap;
}

cv::Mat ImageProcessor::applyColorMap(const cv::Mat &grayscaleMask, const std::vector<cv::Vec3b> &colorMap) {
    cv::Mat colorMask(grayscaleMask.size(), CV_8UC3);
    for (int x = 0; x < grayscaleMask.rows; ++x) {
        for (int y = 0; y < grayscaleMask.cols; ++y) {
            int grayValue = grayscaleMask.at<uchar>(x, y);
            colorMask.at<cv::Vec3b>(x, y) = colorMap[grayValue];
        }
    }
    return colorMask;
}
