//
// Created by gluckasz on 1/29/25.
//

#include "../../include/image-processing-lib/MorphologicalProcessor.h"


namespace MorphologicalProcessor {
    cv::Mat complement(cv::Mat image) {
        cv::Mat result = image.clone();
#pragma omp parallel for collapse(2)
        for (int x = 0; x < image.rows; x++) {
            for (int y = 0; y < image.cols; y++) {
                result.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
            }
        }

        return result;
    }

    bool areEqual(cv::Mat image1, cv::Mat image2) {
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

    cv::Mat imagesUnion(cv::Mat image1, cv::Mat image2) {
        cv::Mat result = image1.clone();
#pragma omp parallel for collapse(2)
        for (int x = 0; x < image1.rows; x++) {
            for (int y = 0; y < image1.cols; y++) {
                result.at<uchar>(x, y) = std::max(image1.at<uchar>(x, y), image2.at<uchar>(x, y));
            }
        }

        return result;
    }

    cv::Mat dilation(cv::Mat image, const int maskNumber,
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

    cv::Mat erosion(cv::Mat image, const int maskNumber,
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

    cv::Mat opening(const cv::Mat &image, const int maskNumber,
                                            const std::unordered_map<int, std::vector<std::vector<FieldType> > > &
                                            maskMapping) {
        cv::Mat result = image.clone();
        result = erosion(result, maskNumber, maskMapping);
        result = dilation(result, maskNumber, maskMapping);
        return result;
    }

    cv::Mat closing(const cv::Mat &image, const int maskNumber,
                                            const std::unordered_map<int, std::vector<std::vector<FieldType> > > &
                                            maskMapping) {
        cv::Mat result = image.clone();
        result = dilation(result, maskNumber, maskMapping);
        result = erosion(result, maskNumber, maskMapping);
        return result;
    }

    cv::Mat hmt(const cv::Mat &image, const int maskNumber) {
        cv::Mat foregroundMatch = image.clone();
        cv::Mat backgroundMatch = image.clone();

        foregroundMatch = erosion(foregroundMatch, maskNumber, hmtMaskMap);
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
}
