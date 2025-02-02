//
// Created by gluckasz on 10/6/24.
//

#include "../include/SpatialDomainProcessor.h"


cv::Mat SpatialDomainProcessor::modifyBrightness(const cv::Mat &image, const int modVal) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            if (modVal < 0) {
                if (result.at<uchar>(x, y) >= 0 - modVal) {
                    result.at<uchar>(x, y) += modVal;
                } else {
                    result.at<uchar>(x, y) = 0;
                }
            } else {
                if (result.at<uchar>(x, y) <= UCHAR_MAX - modVal) {
                    result.at<uchar>(x, y) += modVal;
                } else {
                    result.at<uchar>(x, y) = UCHAR_MAX;
                }
            }
        }
    }
    return result;
}

cv::Mat SpatialDomainProcessor::modifyContrastLinear(const cv::Mat &image, const int modVal) {
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


cv::Mat SpatialDomainProcessor::modifyContrastGamma(const cv::Mat &image, const float modVal) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            const float normalizedPixelVal = static_cast<float>(result.at<uchar>(x, y)) / 255.0f;
            const float gammaCorrectedVal = pow(normalizedPixelVal, modVal);
            int gammaCorrectedPixel = static_cast<int>(round(gammaCorrectedVal * 255));

            gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
            result.at<uchar>(x, y) = gammaCorrectedPixel;
        }
    }
    return result;
}

cv::Mat SpatialDomainProcessor::negative(const cv::Mat &image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            result.at<uchar>(x, y) = 255 - result.at<uchar>(x, y);
        }
    }
    return result;
}

cv::Mat SpatialDomainProcessor::flipHorizontally(const cv::Mat &image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols / 2; y++) {
            const uchar temp = result.at<uchar>(x, y);
            result.at<uchar>(x, y) = result.at<uchar>(x, result.cols - y - 1);
            result.at<uchar>(x, result.cols - y - 1) = temp;
        }
    }
    return result;
}

cv::Mat SpatialDomainProcessor::flipVertically(const cv::Mat &image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows / 2; x++) {
        for (int y = 0; y < result.cols; y++) {
            const uchar temp = result.at<uchar>(x, y);
            result.at<uchar>(x, y) = result.at<uchar>(result.rows - x - 1, y);
            result.at<uchar>(result.rows - x - 1, y) = temp;
        }
    }
    return result;
}

cv::Mat SpatialDomainProcessor::flipDiagonally(const cv::Mat &image) {
    cv::Mat result = image.clone();
    result = flipHorizontally(result);
    return flipVertically(result);
}

cv::Mat SpatialDomainProcessor::resize(cv::Mat image, const float factor) {
    const int newWidth = static_cast<int>(static_cast<float>(image.cols) * factor);
    const int newHeight = static_cast<int>(static_cast<float>(image.rows) * factor);
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

cv::Mat SpatialDomainProcessor::midpointFilter(cv::Mat image, const int kernelSize) {
    cv::Mat newImage;
    image.copyTo(newImage);
    const int border = (kernelSize - 1) / 2;
    int leftFilterSize = -kernelSize / 2;
    const int rightFilterSize = kernelSize / 2;
    if (kernelSize % 2 == 0) {
        leftFilterSize += 1;
    }

    for (int x = border; x < image.rows - border; x++) {
        for (int y = border; y < image.cols - border; y++) {
            uchar max = image.at<uchar>(x - border, y - border);
            uchar min = image.at<uchar>(x - border, y - border);
            for (int i = leftFilterSize; i <= rightFilterSize; i++) {
                for (int j = leftFilterSize; j <= rightFilterSize; j++) {
                    const uchar currentPixel = image.at<uchar>(x + i, y + j);
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

cv::Mat SpatialDomainProcessor::arithmeticMeanFilter(cv::Mat image, const int kernelSize) {
    cv::Mat newImage;
    image.copyTo(newImage);
    const int border = (kernelSize - 1) / 2;
    int leftFilterSize = -kernelSize / 2;
    const int rightFilterSize = kernelSize / 2;
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
            newImage.at<uchar>(x, y) = static_cast<uchar>(sum / pow(kernelSize, 2));
        }
    }
    return newImage;
}


cv::Mat padImageGrayscale(const cv::Mat &image, const int padValue) {
    cv::Mat paddedImage = cv::Mat::zeros(image.rows + padValue * 2, image.cols + padValue * 2, CV_8UC1);
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            paddedImage.at<uchar>(x + padValue, y + padValue) = image.at<uchar>(x, y);
        }
    }
    return paddedImage;
}

template<typename TPixel>
cv::Mat SpatialDomainProcessor::laplacianFilter(const cv::Mat &image, const int laplaceMask) {
    static constexpr std::array<std::array<int, 3>, 3> laplacianMasks[] = {
        {{{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}}},
        {{{{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}}}},
        {{{{1, -2, 1}, {-2, 4, -2}, {1, -2, 1}}}}
    };

    if (laplaceMask < 0 || laplaceMask > 2) {
        throw std::invalid_argument("Invalid laplaceMask value");
    }

    const auto &mask = laplacianMasks[laplaceMask];

    cv::Mat paddedImage = padImage(image, 1);
    cv::Mat result = image.clone();

    for (int x = 1; x < paddedImage.rows - 1; x++) {
        for (int y = 1; y < paddedImage.cols - 1; y++) {
            typename AccTypeFor<TPixel>::type convolutionValue = {};

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    TPixel pixel = paddedImage.at<TPixel>(x + i, y + j);
                    int maskVal = mask[i + 1][j + 1];
                    convolutionValue += pixel * maskVal;
                }
            }

            TPixel resultPixel;
            if constexpr (std::is_same_v<TPixel, uchar>) {
                int val = std::min(std::abs(convolutionValue), 255);
                resultPixel = static_cast<uchar>(val);
            } else {
                for (int c = 0; c < 3; c++) {
                    int val = std::min(std::abs(convolutionValue[c]), 255);
                    resultPixel[c] = static_cast<uchar>(val);
                }
            }

            result.at<TPixel>(x - 1, y - 1) = resultPixel;
        }
    }

    return result;
}

template<typename TPixel>
cv::Mat SpatialDomainProcessor::optimizedLaplacianFilter(cv::Mat image) {
    cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());

    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            typename AccTypeFor<TPixel>::type convolutionValue = {};

            if (x - 1 >= 0)
                convolutionValue += image.at<TPixel>(x - 1, y) * -1;
            if (y - 1 >= 0)
                convolutionValue += image.at<TPixel>(x, y - 1) * -1;
            convolutionValue += image.at<TPixel>(x, y) * 4;
            if (y + 1 < image.cols)
                convolutionValue += image.at<TPixel>(x, y + 1) * -1;
            if (x + 1 < image.rows)
                convolutionValue += image.at<TPixel>(x + 1, y) * -1;

            TPixel resultPixel;
            if constexpr (std::is_same_v<TPixel, uchar>) {
                int val = std::min(std::abs(convolutionValue), 255);
                resultPixel = static_cast<uchar>(val);
            } else {
                for (int c = 0; c < 3; c++) {
                    int val = std::min(std::abs(convolutionValue[c]), 255);
                    resultPixel[c] = static_cast<uchar>(val);
                }
            }

            newImage.at<TPixel>(x, y) = resultPixel;
        }
    }

    return newImage;
}

template<typename TPixel>
cv::Mat SpatialDomainProcessor::robertsOperator1(cv::Mat image) {
    cv::Mat result = image.clone();

    for (int x = 0; x < image.rows - 1; x++) {
        for (int y = 0; y < image.cols - 1; y++) {
            TPixel current = image.at<TPixel>(x, y);
            TPixel diag = image.at<TPixel>(x + 1, y + 1);
            TPixel right = image.at<TPixel>(x, y + 1);
            TPixel below = image.at<TPixel>(x + 1, y);

            TPixel diff1 = current - diag;
            TPixel diff2 = right - below;

            TPixel magnitude;
            if constexpr (std::is_same_v<TPixel, uchar>) {
                int val = static_cast<int>(std::sqrt(std::pow(diff1, 2) + std::pow(diff2, 2)));
                magnitude = std::clamp(val, 0, 255);
            } else {
                for (int c = 0; c < 3; c++) {
                    int val = static_cast<int>(std::sqrt(std::pow(diff1[c], 2) + std::pow(diff2[c], 2)));
                    magnitude[c] = std::clamp(val, 0, 255);
                }
            }

            result.at<TPixel>(x, y) = magnitude;
        }
    }

    return result;
}

template<typename TPixel>
cv::Mat SpatialDomainProcessor::regionGrowing(cv::Mat image, const int criterion) {
    cv::Mat imageSegmentationMasks = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    const int seedXGridSpacing = image.rows / 10;
    const int seedYGridSpacing = image.cols / 10;
    int currentMask = 0;
    constexpr std::array<std::array<int, 2>, 4> dirs = {{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

    for (int seedX = seedXGridSpacing; seedX < image.rows - seedXGridSpacing; seedX += seedXGridSpacing) {
        for (int seedY = seedYGridSpacing; seedY < image.cols - seedYGridSpacing; seedY += seedYGridSpacing) {
            if (imageSegmentationMasks.at<uchar>(seedX, seedY) == 0) {
                currentMask++;
                imageSegmentationMasks.at<uchar>(seedX, seedY) = currentMask;
                TPixel seedPixel = image.at<TPixel>(seedX, seedY);
                double regionSum = getIntensity(seedPixel);
                int regionCount = 1;
                double regionMean = regionSum / regionCount;
                std::queue<std::pair<int, int> > queue;
                queue.emplace(seedX, seedY);

                while (!queue.empty()) {
                    auto [fst, snd] = queue.front();
                    queue.pop();
                    const int currentX = fst;
                    const int currentY = snd;

                    for (auto &dir: dirs) {
                        int newX = currentX + dir[0];
                        if (int newY = currentY + dir[1];
                            newX >= 0 && newX < image.rows && newY >= 0 && newY < image.cols &&
                            imageSegmentationMasks.at<uchar>(newX, newY) == 0) {
                            constexpr double thresholdMean = 20;
                            constexpr double thresholdK = 0.1;

                            TPixel newPixel = image.at<TPixel>(newX, newY);
                            const double newPixelIntensity = getIntensity(newPixel);
                            bool conditionMet = false;

                            switch (criterion) {
                                case 0:
                                    conditionMet = std::abs(regionMean - newPixelIntensity) <= regionMean * thresholdK;
                                    break;
                                case 1:
                                    conditionMet = std::abs(regionMean - newPixelIntensity) <= thresholdMean;
                                    break;
                                case 2: {
                                    constexpr double thresholdAbs = 20;
                                    const double seedIntensity = getIntensity(seedPixel);
                                    conditionMet = std::abs(seedIntensity - newPixelIntensity) <= thresholdAbs;
                                    break;
                                }
                                default:
                                    break;
                            }

                            if (conditionMet) {
                                imageSegmentationMasks.at<uchar>(newX, newY) = currentMask;
                                queue.emplace(newX, newY);
                                regionSum += newPixelIntensity;
                                regionCount++;
                                regionMean = regionSum / regionCount;
                            }
                        }
                    }
                }
            }
        }
    }

    return imageSegmentationMasks;
}

