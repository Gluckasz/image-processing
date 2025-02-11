//
// Created by gluckasz on 10/6/24.
//

#include "../../include/image-processing-lib/SpatialDomainProcessor.h"

namespace {
    template<typename TPixel>
    double getIntensity(const TPixel &pixel);

    template<>
    double getIntensity<uchar>(const uchar &pixel) {
        return pixel;
    }

    template<>
    double getIntensity<cv::Vec3b>(const cv::Vec3b &pixel) {
        return pixel[0] + pixel[1] + pixel[2];
    }

    cv::Mat padImage(const cv::Mat &image) {
        cv::Mat padded;
        constexpr int borderSize = 1;
        copyMakeBorder(image, padded, borderSize, borderSize, borderSize, borderSize, cv::BORDER_CONSTANT);
        return padded;
    }
}

namespace SpatialDomainProcessor {
    cv::Mat modifyBrightness(const cv::Mat &image, const int modVal) {
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

    cv::Mat modifyContrastLinear(const cv::Mat &image, const int modVal) {
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


    cv::Mat modifyContrastGamma(const cv::Mat &image, const float modVal) {
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

    cv::Mat negative(const cv::Mat &image) {
        cv::Mat result = image.clone();
        for (int x = 0; x < result.rows; x++) {
            for (int y = 0; y < result.cols; y++) {
                result.at<uchar>(x, y) = 255 - result.at<uchar>(x, y);
            }
        }
        return result;
    }

    cv::Mat flipHorizontally(const cv::Mat &image) {
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

    cv::Mat flipVertically(const cv::Mat &image) {
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

    cv::Mat flipDiagonally(const cv::Mat &image) {
        cv::Mat result = image.clone();
        result = flipHorizontally(result);
        return flipVertically(result);
    }

    cv::Mat resize(cv::Mat image, const float factor) {
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

    cv::Mat midpointFilter(cv::Mat image, const int kernelSize) {
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

    cv::Mat arithmeticMeanFilter(cv::Mat image, const int kernelSize) {
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

    cv::Mat laplacianFilter(const cv::Mat &image, const int laplaceMask) {
        static constexpr std::array<std::array<std::array<int, 3>, 3>, 3> laplacianMasks = {{
            {{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}},
            {{{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}}},
            {{{1, -2, 1}, {-2, 4, -2}, {1, -2, 1}}}
        }};

        if (laplaceMask < 0 || laplaceMask > 2) {
            throw std::invalid_argument("Invalid laplaceMask value");
        }

        const auto &mask = laplacianMasks[laplaceMask];

        cv::Mat paddedImage = padImage(image);
        cv::Mat result = image.clone();

        for (int x = 1; x < paddedImage.rows - 1; x++) {
            for (int y = 1; y < paddedImage.cols - 1; y++) {
                int convolutionValue = 0;

                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int pixel = paddedImage.at<uchar>(x + i, y + j);
                        int maskVal = mask[i + 1][j + 1];
                        convolutionValue += pixel * maskVal;
                    }
                }

                int val = std::min(std::abs(convolutionValue), 255);
                result.at<uchar>(x - 1, y - 1) = static_cast<uchar>(val);
            }
        }

        return result;
    }

    cv::Mat optimizedLaplacianFilter(cv::Mat image) {
        cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());

        for (int x = 0; x < image.rows; x++) {
            for (int y = 0; y < image.cols; y++) {
               int convolutionValue = 0;

                if (x - 1 >= 0)
                    convolutionValue += image.at<uchar>(x - 1, y) * -1;
                if (y - 1 >= 0)
                    convolutionValue += image.at<uchar>(x, y - 1) * -1;
                convolutionValue += image.at<uchar>(x, y) * 4;
                if (y + 1 < image.cols)
                    convolutionValue += image.at<uchar>(x, y + 1) * -1;
                if (x + 1 < image.rows)
                    convolutionValue += image.at<uchar>(x + 1, y) * -1;

                int val = std::min(std::abs(convolutionValue), 255);
                newImage.at<uchar>(x, y) = static_cast<uchar>(val);
            }
        }

        return newImage;
    }

    cv::Mat robertsOperator1(cv::Mat image) {
        cv::Mat result = image.clone();

        for (int x = 0; x < image.rows - 1; x++) {
            for (int y = 0; y < image.cols - 1; y++) {
                int current = image.at<uchar>(x, y);
                int diag = image.at<uchar>(x + 1, y + 1);
                int right = image.at<uchar>(x, y + 1);
                int below = image.at<uchar>(x + 1, y);

                int diff1 = current - diag;
                int diff2 = right - below;

                int val = static_cast<int>(std::round(std::sqrt(std::pow(diff1, 2) + std::pow(diff2, 2))));
                int magnitude = std::clamp(val, 0, 255);
                result.at<uchar>(x, y) = magnitude;
            }
        }

        return result;
    }

    template<typename TPixel>
    cv::Mat regionGrowing(cv::Mat image, const int criterion) {
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
                                        conditionMet =
                                                std::abs(regionMean - newPixelIntensity) <= regionMean * thresholdK;
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

    std::vector<cv::Vec3b> createColorMap() {
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

    cv::Mat applyColorMap(const cv::Mat &grayscaleMask,
                                                  const std::vector<cv::Vec3b> &colorMap) {
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
}

template cv::Mat SpatialDomainProcessor::regionGrowing<uchar>(cv::Mat, int);
template cv::Mat SpatialDomainProcessor::regionGrowing<cv::Vec3b>(cv::Mat, int);
