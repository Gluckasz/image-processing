//
// Created by gluckasz on 10/6/24.
//

#include "../include/GrayscaleImageProcessor.h"


cv::Mat GrayscaleImageProcessor::modifyBrightness(cv::Mat image, int modVal) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            if (modVal < 0) {
                if (result.at<uchar>(x, y) >= 0 - modVal) {
                    result.at<uchar>(x, y) += modVal;
                }
                else {
                    result.at<uchar>(x, y) = 0;
                }
            }
            else {
                if (result.at<uchar>(x, y) <= UCHAR_MAX - modVal) {
                    result.at<uchar>(x, y) += modVal;
                }
                else {
                    result.at<uchar>(x, y) = UCHAR_MAX;
                }
            }
        }
    }
    return result;
}

cv::Mat GrayscaleImageProcessor::modifyContrastLinear(cv::Mat image, int modVal) {
    cv::Mat result = image.clone();
    int max = 0;
    int min = 255;
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            if (max < result.at<uchar>(x, y)) {
                max = static_cast<int>(result.at<uchar>(x, y));
            }
            if (min > result.at<uchar>(x, y)) {
                min = static_cast<int>(result.at<uchar>(x, y));
            }
        }
    }
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            result.at<uchar>(x, y) = static_cast<uchar>(std::clamp(
                (static_cast<int>(result.at<uchar>(x, y)) - min)
                * std::max(max - min + 2 * modVal, 0)
                / (max - min) + min - modVal,
                0,
                UCHAR_MAX
                ));
        }
    }
    return result;
}


cv::Mat GrayscaleImageProcessor::modifyContrastGamma(cv::Mat image, float modVal){
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            float normalizedPixelVal = static_cast<float>(result.at<uchar>(x, y)) / 255.0f;
            float gammaCorrectedVal = pow(normalizedPixelVal, modVal);
            int gammaCorrectedPixel = round(gammaCorrectedVal * 255);

            gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
            result.at<uchar>(x, y) = gammaCorrectedPixel;
        }
    }
    return result;
}

cv::Mat GrayscaleImageProcessor::negative(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            result.at<uchar>(x, y) = 255 - result.at<uchar>(x, y);
        }
    }
    return result;
}

cv::Mat GrayscaleImageProcessor::flipHorizontally(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols / 2; y++) {
            uchar temp = result.at<uchar>(x, y);
            result.at<uchar>(x, y) = result.at<uchar>(x, result.cols - y - 1);
            result.at<uchar>(x, result.cols - y - 1) = temp;
        }
    }
    return result;
}

cv::Mat GrayscaleImageProcessor::flipVertically(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows / 2; x++) {
        for (int y = 0; y < result.cols; y++) {
            uchar temp = result.at<uchar>(x, y);
            result.at<uchar>(x, y) = result.at<uchar>(result.rows - x - 1, y);
            result.at<uchar>(result.rows - x - 1, y)= temp;
        }
    }
    return result;
}

cv::Mat GrayscaleImageProcessor::flipDiagonally(cv::Mat image) {
    cv::Mat result = image.clone();
    result = this->flipHorizontally(result);
    return this->flipVertically(result);
}

cv::Mat GrayscaleImageProcessor::resize(cv::Mat image, float factor) {
    int newWidth = static_cast<int>(static_cast<float>(image.cols) * factor);
    int newHeight = static_cast<int>(static_cast<float>(image.rows) * factor);
    cv::Mat newImage = cv::Mat::zeros(newHeight, newWidth, CV_8UC1);

    for (int x = 0; x < newHeight; x++) {
        for (int y = 0; y < newWidth; y++) {
            newImage.at<uchar>(x, y) = image.at<uchar>(
                static_cast<int>(static_cast<float>(x) / factor),
                static_cast<int>(static_cast<float>(y) / factor));
        }
    }
    return newImage;
}

cv::Mat GrayscaleImageProcessor::midpointFilter(cv::Mat image, int kernelSize) {
    cv::Mat newImage;
    image.copyTo(newImage);
    int border = (kernelSize - 1) / 2;
    int leftFilterSize = -(kernelSize / 2);
    int rightFilterSize = (kernelSize / 2);
    if (kernelSize % 2 == 0) {
        leftFilterSize += 1;
    }

    for (int x = border; x < image.rows - border; x++) {
        for (int y = border; y < image.cols - border; y++) {
            uchar max = image.at<uchar>(x - border, y - border);
            uchar min = image.at<uchar>(x - border, y - border);
            for (int i = leftFilterSize; i <= rightFilterSize; i++) {
                for (int j = leftFilterSize; j <= rightFilterSize; j++) {
                    uchar currentPixel = image.at<uchar>(x + i, y + j);
                    if (max < currentPixel) {
                        max = currentPixel;
                    }
                    if (min > currentPixel) {
                        min = currentPixel;
                    }
                }
            }
            newImage.at<uchar>(x, y) = (max + min) / 2;
        }
    }
    return newImage;
}

cv::Mat GrayscaleImageProcessor::arithmeticMeanFilter(cv::Mat image, int kernelSize) {
    cv::Mat newImage;
    image.copyTo(newImage);
    int border = (kernelSize - 1) / 2;
    int leftFilterSize = -(kernelSize / 2);
    int rightFilterSize = (kernelSize / 2);
    if (kernelSize % 2 == 0) {
        leftFilterSize += 1;
    }

    for (int x = border; x < image.rows - border; x++) {
        for (int y = border; y < image.cols - border; y++) {
            int sum = 0;
            for (int i = leftFilterSize; i <= rightFilterSize; i++) {
                for (int j = leftFilterSize; j <= rightFilterSize; j++) {
                    sum += image.at<uchar>(x + j, y + i);
                }
            }
            newImage.at<uchar>(x, y) = sum / pow(kernelSize, 2);
        }
    }
    return newImage;
}

double GrayscaleImageProcessor::meanSquareError(cv::Mat originalImage, cv::Mat newImage) {
    int squareDistanceSum = 0;

    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            squareDistanceSum += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y) , 2);
        }
    }

    return static_cast<double>(squareDistanceSum)
    / static_cast<double>(originalImage.cols)
    / static_cast<double>(originalImage.rows);
}

double GrayscaleImageProcessor::peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) {
    uchar max = 0;

    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            if (originalImage.at<uchar>(x, y) > max) {
                max = originalImage.at<uchar>(x, y);
            }
        }
    }
    return this->meanSquareError(originalImage, newImage)
    / pow(max, 2);
}

double GrayscaleImageProcessor::signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
    double squareSum = 0;
    double se = 0;

    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            squareSum += pow(originalImage.at<uchar>(x, y), 2);
            se += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y) , 2);
        }
    }

    return 10 * std::log10(
        squareSum
        / se
        );
}

double GrayscaleImageProcessor::peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
    uchar max = 0;
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            if (originalImage.at<uchar>(x, y) > max) {
                max = originalImage.at<uchar>(x, y);
            }
        }
    }

    unsigned long long  maxSquareSum = 0;
    double se = 0;
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            maxSquareSum += pow(static_cast<int>(max), 2);
            se += pow(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y) , 2);
        }
    }

    return 10 * std::log10(
        maxSquareSum
        / se);
}


double GrayscaleImageProcessor::maximumDifference(cv::Mat originalImage, cv::Mat newImage) {
    uchar maxDifference = 0;
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            if (std::abs(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y)) > maxDifference) {
                maxDifference = std::abs(originalImage.at<uchar>(x, y) - newImage.at<uchar>(x, y));
            }
        }
    }
    return maxDifference;
}

cv::Mat padImageGrayscale(const cv::Mat& image, int padValue) {
    cv::Mat paddedImage = cv::Mat::zeros(image.rows + padValue * 2, image.cols + padValue * 2, CV_8UC1);
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            paddedImage.at<uchar>(x + padValue, y + padValue) = image.at<uchar>(x, y);
        }
    }
    return paddedImage;
}

cv::Mat GrayscaleImageProcessor::laplacianFilter(cv::Mat image, int laplaceMask) {
    std::array<std::array<int, 3>, 3> mask{};
    switch (laplaceMask) {
        case 0:
            mask = {{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}};
            break;
        case 1:
            mask = {{{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}}};
            break;
        case 2:
            mask = {{{1, -2, 1}, {-2, 4, -2}, {1, -2, 1}}};
            break;
    }

    cv::Mat paddedImage = padImageGrayscale(image, 1);
    cv::Mat result = image.clone();
    for (int x = 1; x < paddedImage.rows - 1; x++) {
        for (int y = 1; y < paddedImage.cols - 1; y++) {
            int convolutionValue = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                   convolutionValue += paddedImage.at<uchar>(x + i, y + j) * mask[i + 1][j + 1];
                }
            }
            convolutionValue = std::min(std::abs(convolutionValue), 255);

            result.at<uchar>(x - 1, y - 1) = convolutionValue;
        }
    }

    return result;
}

cv::Mat GrayscaleImageProcessor::optimizedLaplacianFilter(cv::Mat image) {
    cv::Mat newImage = cv::Mat::zeros(image.size(), CV_8UC1);
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
                int convolutionValue = 0;
                if (x - 1 >= 0) convolutionValue += image.at<uchar>(x - 1, y) * -1;
                if (y - 1 >= 0) convolutionValue += image.at<uchar>(x, y - 1) * -1;
                convolutionValue += image.at<uchar>(x, y) * 4;
                if (y + 1 < image.cols) convolutionValue += image.at<uchar>(x, y + 1) * -1;
                if (x + 1 < image.rows) convolutionValue += image.at<uchar>(x + 1, y) * -1;
                convolutionValue = std::min(std::abs(convolutionValue), 255);

                newImage.at<uchar>(x, y) = convolutionValue;
            }
        }
    return newImage;
}

cv::Mat GrayscaleImageProcessor::robertsOperator1(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows - 1; x++) {
        for (int y = 0; y < image.cols - 1; y++) {
            result.at<uchar>(x, y) = std::min(std::abs(static_cast<int>(sqrt(
                pow(image.at<uchar>(x, y) - image.at<uchar>(x + 1, y + 1), 2)
                + pow(image.at<uchar>(x, y + 1) - image.at<uchar>(x + 1, y), 2)
                ))), 255);
        }
    }
    return result;
}

cv::Mat GrayscaleImageProcessor::regionGrowing(cv::Mat image, int criterion) {
    cv::Mat imageSegmentationMasks = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    int seedXGridSpacing = image.rows / 10;
    int seedYGridSpacing = image.cols / 10;
    int currentMask = 0;
    const double thresholdK = 0.1;
    const double thresholdMean = 20;
    const double thresholdAbs = 20;
    std::array<std::array<int, 2>, 4> dirs = {
        {
            {1, 0},
            {-1, 0},
            {0, 1},
            {0, -1}
        }
    };
    for (int seedX = seedXGridSpacing; seedX < image.rows - seedXGridSpacing; seedX += seedXGridSpacing) {
        for (int seedY = seedYGridSpacing; seedY < image.cols - seedYGridSpacing; seedY+= seedYGridSpacing) {
            if (imageSegmentationMasks.at<uchar>(seedX, seedY) == 0) {
                currentMask++;
                imageSegmentationMasks.at<uchar>(seedX, seedY) = currentMask;
                double regionMean = image.at<uchar>(seedX, seedY);
                int regionCount = 1;
                std::queue<std::pair<int, int>> queue;
                queue.emplace(seedX, seedY);
                while (!queue.empty()) {
                    std::pair<int, int> currentPixel = queue.front();
                    int currentX = currentPixel.first;
                    int currentY = currentPixel.second;
                    queue.pop();
                    for(auto& dir: dirs) {
                        int newX = currentX + dir[0];
                        int newY = currentY + dir[1];
                        if (
                            newX >= 0 && newX < image.rows && newY >= 0 && newY < image.cols
                        && imageSegmentationMasks.at<uchar>(newX, newY) == 0
                        && ((std::abs(regionMean - static_cast<double>(image.at<uchar>(newX, newY)))
                            <= regionMean * thresholdK && criterion == 0)
                        || (std::abs(regionMean - static_cast<double>(image.at<uchar>(newX, newY)))
                            <= thresholdMean && criterion == 1)
                        || (std::abs(image.at<uchar>(seedX, seedY) - static_cast<double>(image.at<uchar>(newX, newY)))
                            <= thresholdAbs && criterion == 2))
                            ) {
                            queue.emplace(newX, newY);
                            imageSegmentationMasks.at<uchar>(newX, newY) = currentMask;
                            double regionSum = regionMean * regionCount;
                            regionCount++;
                            regionSum += image.at<uchar>(newX, newY);
                            regionMean = regionSum / regionCount;
                        }
                    }
                }
            }
        }
    }
    return imageSegmentationMasks;
}

