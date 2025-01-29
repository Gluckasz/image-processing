//
// Created by gluckasz on 1/29/25.
//

#include "../include/MorphologicalProcessor.h"

cv::Mat MorphologicalProcessor::complement(cv::Mat image) {
    cv::Mat result = image.clone();
#pragma omp parallel for collapse(2)
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            result.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
        }
    }

    return result;
}

bool MorphologicalProcessor::areEqual(cv::Mat image1, cv::Mat image2) {
    if (image1.rows != image2.rows || image1.cols != image2.cols) {
        return false;
    }
    for (int x = 0; x < image1.rows; x++) {
        for (int y = 0; y < image1.cols; y++) {
            if (image1.at<uchar>(x, y) != image2.at<uchar>(x, y)) {
                return false;
            }
        }
    }
    return true;
}

cv::Mat MorphologicalProcessor::imagesUnion(cv::Mat image1, cv::Mat image2) {
    cv::Mat result = image1.clone();
#pragma omp parallel for collapse(2)
    for (int x = 0; x < image1.rows; x++) {
        for (int y = 0; y < image1.cols; y++) {
            result.at<uchar>(x, y) = std::max(image1.at<uchar>(x, y), image2.at<uchar>(x, y));
        }
    }

    return result;
}


const std::unordered_map<int, std::vector<std::vector<MorphologicalProcessor::FieldType> > >
MorphologicalProcessor::maskMap = {
    {1, {{FieldType::WHITE_MARKER, FieldType::WHITE}}},
    {
        2, {
            {FieldType::WHITE_MARKER},
            {FieldType::WHITE}
        }
    },
    {
        3, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
        }
    },
    {
        4, {
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::INACTIVE}
        }
    },
    {
        5, {
            {FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::WHITE, FieldType::INACTIVE}
        }
    },
    {
        6, {
            {FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::WHITE, FieldType::INACTIVE}
        }
    },
    {7, {{FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE}}},
    {8, {{FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::WHITE}}},
    {
        9, {
            {FieldType::WHITE, FieldType::WHITE_MARKER},
            {FieldType::WHITE, FieldType::INACTIVE}
        }
    },
    {
        10, {
            {FieldType::WHITE, FieldType::WHITE},
            {FieldType::WHITE_MARKER, FieldType::INACTIVE}
        }
    },
};

const std::unordered_map<int, std::vector<std::vector<MorphologicalProcessor::FieldType> > >
MorphologicalProcessor::hmtMaskMap = {
    {
        1, {
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE}
        }
    },
    {
        2, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
        }
    },
    {
        3, {
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE}
        }
    },
    {
        4, {
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
        }
    },
    {
        5, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
        }
    },
    {
        6, {
            {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
            {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
        }
    },
    {
        7, {
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
        }
    },
    {
        8, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
        }
    },
    {
        9, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
        }
    },
    {
        10, {
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
        }
    },
    {
        11, {
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
            {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
        }
    },
    {
        12, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
        }
    },
};

const std::unordered_map<int, std::vector<std::vector<MorphologicalProcessor::FieldType> > >
MorphologicalProcessor::hmtComplementMaskMap = {
    {
        1, {
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE}
        }
    },
    {
        2, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
        }
    },
    {
        3, {
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK}
        }
    },
    {
        4, {
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
        }
    },
    {
        5, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
        }
    },
    {
        6, {
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
        }
    },
    {
        7, {
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
        }
    },
    {
        8, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
        }
    },
    {
        9, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
        }
    },
    {
        10, {
            {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
            {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
        }
    },
    {
        11, {
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
        }
    },
    {
        12, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
        }
    },
};

cv::Mat MorphologicalProcessor::dilation(cv::Mat image, const int maskNumber,
                                         const std::unordered_map<int, std::vector<std::vector<FieldType> > > &
                                         maskMapping) {
    if (image.empty()) {
        throw std::invalid_argument("Image cannot be empty");
    }
    if (maskMapping == maskMap) {
        if (maskNumber < 1 || maskNumber > 10) {
            throw std::out_of_range("Mask number has to be between 1 and 10 inclusive for maskMap");
        }
    } else {
        if (maskNumber < 1 || maskNumber > 12) {
            throw std::out_of_range("Mask number has to be between 1 and 12 inclusive for hmtMaskMap");
        }
    }


    cv::Mat result = image.clone();
    const std::vector<std::vector<FieldType> > mask = maskMapping.find(maskNumber)->second;

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
    for (int x = 0; x < image.rows; ++x) {
        for (int y = 0; y < image.cols; ++y) {
            bool shouldDilate = false;
            for (int i = 0; i < mask.size() && !shouldDilate; ++i) {
                for (int j = 0; j < mask[i].size() && !shouldDilate; ++j) {
                    if (mask[i][j] == FieldType::INACTIVE) continue;

                    const int imgX = x + (i - markerX);
                    const int imgY = y + (j - markerY);
                    uchar pixelValue = 0;
                    if (imgX >= 0 && imgX < image.rows && imgY >= 0 && imgY < image.cols) {
                        pixelValue = image.at<uchar>(imgX, imgY);
                    }

                    if (const FieldType ft = mask[i][j];
                        ((ft == FieldType::BLACK_MARKER || ft == FieldType::BLACK) && pixelValue == 0) || (
                            (ft == FieldType::WHITE_MARKER || ft == FieldType::WHITE) && pixelValue == 255)) {
                        shouldDilate = true;
                    }
                }
            }
            result.at<uchar>(x, y) = shouldDilate ? 255 : 0;
        }
    }
    return result;
}

cv::Mat MorphologicalProcessor::erosion(cv::Mat image, const int maskNumber,
                                        const std::unordered_map<int, std::vector<std::vector<FieldType> > > &
                                        maskMapping) {
    if (image.empty()) {
        throw std::invalid_argument("Image cannot be empty");
    }
    if (maskMapping == maskMap) {
        if (maskNumber < 1 || maskNumber > 10) {
            throw std::out_of_range("Mask number has to be between 1 and 10 inclusive for maskMap");
        }
    } else {
        if (maskNumber < 1 || maskNumber > 12) {
            throw std::out_of_range("Mask number has to be between 1 and 12 inclusive for hmtMaskMap");
        }
    }

    cv::Mat result = image.clone();
    const std::vector<std::vector<FieldType> > mask = maskMapping.find(maskNumber)->second;

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
    for (int x = 0; x < image.rows; ++x) {
        for (int y = 0; y < image.cols; ++y) {
            bool shouldErode = false;
            for (int i = 0; i < mask.size() && !shouldErode; ++i) {
                for (int j = 0; j < mask[i].size() && !shouldErode; ++j) {
                    if (mask[i][j] == FieldType::INACTIVE) continue;

                    const int imgX = x + (i - markerX);
                    const int imgY = y + (j - markerY);
                    uchar pixelValue = 0;
                    if (imgX >= 0 && imgX < image.rows && imgY >= 0 && imgY < image.cols) {
                        pixelValue = image.at<uchar>(imgX, imgY);
                    }

                    if (const FieldType ft = mask[i][j];
                        ((ft == FieldType::BLACK_MARKER || ft == FieldType::BLACK) && pixelValue != 0) || (
                             ft == FieldType::WHITE_MARKER || ft == FieldType::WHITE) && pixelValue != 255) {
                        shouldErode = true;
                    }
                }
            }
            result.at<uchar>(x, y) = shouldErode ? 0 : 255;
        }
    }
    return result;
}

cv::Mat MorphologicalProcessor::opening(const cv::Mat &image, const int maskNumber,
                                        const std::unordered_map<int, std::vector<std::vector<FieldType> > > &
                                        maskMapping) {
    cv::Mat result = image.clone();
    result = erosion(result, maskNumber, maskMapping);
    result = dilation(result, maskNumber, maskMapping);
    return result;
}

cv::Mat MorphologicalProcessor::closing(const cv::Mat &image, const int maskNumber,
                                        const std::unordered_map<int, std::vector<std::vector<FieldType> > > &
                                        maskMapping) {
    cv::Mat result = image.clone();
    result = dilation(result, maskNumber, maskMapping);
    result = erosion(result, maskNumber, maskMapping);
    return result;
}

cv::Mat MorphologicalProcessor::hmt(const cv::Mat &image, const int maskNumber,
                                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping) {
    cv::Mat foregroundMatch = image.clone();
    cv::Mat backgroundMatch = image.clone();

    foregroundMatch = erosion(foregroundMatch, maskNumber, maskMapping);
    backgroundMatch = complement(backgroundMatch);

    backgroundMatch = erosion(backgroundMatch, maskNumber, hmtComplementMaskMap);

    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
#pragma omp parallel for collapse(2)
    for (int x = 0; x < foregroundMatch.rows; x++) {
        for (int y = 0; y < foregroundMatch.cols; y++) {
            if (foregroundMatch.at<uchar>(x, y) == UCHAR_MAX && backgroundMatch.at<uchar>(x, y) == UCHAR_MAX) {
                result.at<uchar>(x, y) = 255;
            }
        }
    }

    return result;
}

std::vector<cv::Vec3b> MorphologicalProcessor::createColorMap() {
    std::vector colorMap(256, cv::Vec3b(0, 0, 0));
    for (int i = 1; i <= 255; ++i) {
        const int hue = i * 179 / 255;
        cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255));
        cv::Mat rgb;
        cvtColor(hsv, rgb, cv::COLOR_HSV2BGR);
        colorMap[i] = rgb.at<cv::Vec3b>(0, 0);
    }
    return colorMap;
}

cv::Mat MorphologicalProcessor::applyColorMap(const cv::Mat &grayscaleMask, const std::vector<cv::Vec3b> &colorMap) {
    cv::Mat colorMask(grayscaleMask.size(), CV_8UC3);
#pragma omp parallel for collapse(2)
    for (int x = 0; x < grayscaleMask.rows; ++x) {
        for (int y = 0; y < grayscaleMask.cols; ++y) {
            const int grayValue = grayscaleMask.at<uchar>(x, y);
            colorMask.at<cv::Vec3b>(x, y) = colorMap[grayValue];
        }
    }
    return colorMask;
}
