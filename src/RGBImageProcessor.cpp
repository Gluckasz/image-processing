//
// Created by gluckasz on 10/7/24.
//

#include "../include/RGBImageProcessor.h"


cv::Mat RGBImageProcessor::modifyBrightness(cv::Mat image, int modVal) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (modVal < 0) {
                    if (result.at<cv::Vec3b>(x, y)[z] >= 0 - modVal) {
                        result.at<cv::Vec3b>(x, y)[z] += modVal;
                    }
                    else {
                        result.at<cv::Vec3b>(x, y)[z] = 0;
                    }
                }
                else {
                    if (result.at<cv::Vec3b>(x, y)[z]<= UCHAR_MAX - modVal) {
                        result.at<cv::Vec3b>(x, y)[z]+= modVal;
                    }
                    else {
                        result.at<cv::Vec3b>(x, y)[z] = UCHAR_MAX;
                    }
                }
            }
        }
    }
    return result;
}

cv::Mat RGBImageProcessor::modifyContrastLinear(cv::Mat image, int modVal) {
    cv::Mat result = image.clone();
    uchar max[3] = {0, 0, 0};
    uchar min[3] = {255, 255, 255};
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (max[z] < image.at<cv::Vec3b>(x, y)[z]) {
                    max[z] = image.at<cv::Vec3b>(x, y)[z];
                }
                if (min[z] > image.at<cv::Vec3b>(x, y)[z]) {
                    min[z] = image.at<cv::Vec3b>(x, y)[z];
                }
            }
        }
    }
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                result.at<cv::Vec3b>(x, y)[z] = std::clamp(
                    (image.at<cv::Vec3b>(x, y)[z] - min[z])
                    * std::max(max[z] - min[z] + 2 * modVal, 0)
                    / (max[z] - min[z]) + min[z] - modVal,
                    0,
                    UCHAR_MAX
                    );
            }
        }
    }
    return result;
}

cv::Mat RGBImageProcessor::modifyContrastGamma(cv::Mat image, float modVal) {
    cv::Mat result = image.clone();
    for (int x = 0; x < result.rows; x++) {
        for (int y = 0; y < result.cols; y++) {
            for (int z = 0; z < 3; z++) {
                float normalizedPixelVal = static_cast<float>(image.at<cv::Vec3b>(x, y)[z]) / 255.0f;

                float gammaCorrectedVal = pow(normalizedPixelVal, modVal);
                int gammaCorrectedPixel = round(gammaCorrectedVal * 255);

                gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
                result.at<cv::Vec3b>(x, y)[z] = gammaCorrectedPixel;
            }
        }
    }
    return result;
}

cv::Mat RGBImageProcessor::negative(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                result.at<cv::Vec3b>(x, y)[z] = 255 - image.at<cv::Vec3b>(x, y)[z];
            }
        }
    }
    return result;
}

cv::Mat RGBImageProcessor::flipHorizontally(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<cv::Vec3b>(x, y)[z];
                result.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(x, image.cols - y - 1)[z];
                result.at<cv::Vec3b>(x, image.cols - y - 1)[z] = temp;
            }
        }
    }
    return result;
}

cv::Mat RGBImageProcessor::flipVertically(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<cv::Vec3b>(x, y)[z];
                result.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(image.rows - x - 1, y)[z];
                result.at<cv::Vec3b>(image.rows - x - 1, y)[z] = temp;
            }
        }
    }
    return result;
}
cv::Mat RGBImageProcessor::flipDiagonally(cv::Mat image) {
    cv::Mat result = image.clone();
    result = this->flipHorizontally(image);
    return this->flipVertically(result);
}

cv::Mat RGBImageProcessor::resize(cv::Mat image, float factor) {
    int newWidth = static_cast<int>(static_cast<float>(image.cols) * factor);
    int newHeight = static_cast<int>(static_cast<float>(image.rows) * factor);
    cv::Mat newImage = cv::Mat ::zeros(newHeight, newWidth, CV_8UC3);

    for (int x = 0; x < newHeight; x++) {
        for (int y = 0; y < newWidth; y++) {
            for (int z = 0; z < 3; z++) {
            newImage.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(
                static_cast<int>(static_cast<float>(x) / factor),
                static_cast<int>(static_cast<float>(y) / factor))[z];
            }
        }
    }
    return newImage;
}

cv::Mat RGBImageProcessor::midpointFilter(cv::Mat image, int kernelSize) {
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
            for (int z = 0; z < 3; z++) {
                uchar max = image.at<cv::Vec3b>(x - border, y - border)[z];
                uchar min = image.at<cv::Vec3b>(x - border, y - border)[z];
                for (int i = leftFilterSize; i <= rightFilterSize; i++) {
                    for (int j = leftFilterSize; j <= rightFilterSize; j++) {
                        uchar currentPixel = image.at<cv::Vec3b>(x + i, y + j)[z];
                        if (max < currentPixel) {
                            max = currentPixel;
                        }
                        if (min > currentPixel) {
                            min = currentPixel;
                        }
                    }
                }
                newImage.at<cv::Vec3b>(x, y)[z] = (max + min) / 2;
            }
        }
    }
    return newImage;
}

cv::Mat RGBImageProcessor::arithmeticMeanFilter(cv::Mat image, int kernelSize) {
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
            for (int z = 0; z < 3; z++) {
                int sum = 0;
                for (int i = leftFilterSize; i <= rightFilterSize; i++) {
                    for (int j = leftFilterSize; j <= rightFilterSize; j++) {
                        sum += image.at<cv::Vec3b>(x + j, y + i)[z];
                    }
                }
                newImage.at<cv::Vec3b>(x, y)[z] = sum / pow(kernelSize, 2);
            }
        }
    }
    return newImage;
}

double RGBImageProcessor::meanSquareError(cv::Mat originalImage, cv::Mat newImage) {
    int squareDistanceSum = 0;

    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            for (int z = 0; z < 3; z++) {
                squareDistanceSum += pow(originalImage.at<cv::Vec3b>(x, y)[z] - newImage.at<cv::Vec3b>(x, y)[z] , 2);
            }
        }
    }

    return static_cast<double>(squareDistanceSum)
    / static_cast<double>(originalImage.cols)
    / static_cast<double>(originalImage.rows)
    / 3;
}

double RGBImageProcessor::peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) {
    std::array<uchar, 3> max = {};
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            for (int z = 0; z < 3; z++) {
                if (originalImage.at<cv::Vec3b>(x, y)[z] > max[z]) {
                    max[z] = originalImage.at<cv::Vec3b>(x, y)[z];
                }
            }
        }
    }
    double maxSum = 0;
    for (uchar num : max) {
        maxSum += pow(num, 2);
    }
    return this->meanSquareError(originalImage, newImage)
    / maxSum
    * 3;
}

double RGBImageProcessor::signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
    unsigned long long squareSum = 0;
    double se = 0;

    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            for (int z = 0; z < 3; z++) {
                squareSum += pow(originalImage.at<cv::Vec3b>(x, y)[z], 2);
                se += pow(originalImage.at<cv::Vec3b>(x, y)[z] - newImage.at<cv::Vec3b>(x, y)[z], 2);
            }
        }
    }

    return 10 * std::log10(
        squareSum
        / se
        );
}

double RGBImageProcessor::peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) {
    std::array<uchar, 3> max = {};
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            for (int z = 0; z < 3; z++) {
                if (originalImage.at<cv::Vec3b>(x, y)[z] > max[z]) {
                    max[z] = originalImage.at<cv::Vec3b>(x, y)[z];
                }
            }
        }
    }

    unsigned long long  maxSquareSum = 0;\
    double se = 0;
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            for (int z = 0; z < 3; z++) {
                maxSquareSum += pow(static_cast<int>(max[z]), 2);
                se += pow(originalImage.at<cv::Vec3b>(x, y)[z] - newImage.at<cv::Vec3b>(x, y)[z], 2);
            }
        }
    }

    return 10 * std::log10(
        maxSquareSum
        / se
        );
}


double RGBImageProcessor::maximumDifference(cv::Mat originalImage, cv::Mat newImage) {
    uchar maxDifference = 0;
    for (int x = 0; x < originalImage.cols; x++) {
        for (int y = 0; y < originalImage.rows; y++) {
            for (int z = 0; z < 3; z++) {
                if (std::abs(originalImage.at<cv::Vec3b>(x, y)[z] - newImage.at<cv::Vec3b>(x, y)[z]) > maxDifference) {
                    maxDifference = std::abs(originalImage.at<cv::Vec3b>(x, y)[z] - newImage.at<cv::Vec3b>(x, y)[z]);
                }
            }
        }
    }
    return maxDifference;
}
std::array<uint, 256> RGBImageProcessor::computeHistogram(cv::Mat image, int histogramChannel, uint &histogramMaxVal) {
    std::array<uint, UCHAR_MAX + 1> intensityCountArray = {};
    histogramMaxVal = 0;

    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            uint intensity = image.at<cv::Vec3b>(x, y)[histogramChannel];
            intensityCountArray[intensity]++;
            if (intensityCountArray[intensity] > histogramMaxVal) {
                histogramMaxVal = intensityCountArray[intensity];
            }
        }
    }

    return intensityCountArray;
}


cv::Mat RGBImageProcessor::histogram(cv::Mat image, int histogramChannel) {
    uint histogramHeight = 0;
    std::array<uint, UCHAR_MAX + 1> intensityCountArray = computeHistogram(image, histogramChannel, histogramHeight);

    uint widthFactor = 8;
    int histogramWidth = (UCHAR_MAX + 1) * widthFactor;
    cv::Mat histogramImage = cv::Mat::zeros(histogramHeight, histogramWidth + widthFactor, CV_8UC1);
    for (int y = 0; y < histogramWidth; y++) {
        for (int x = 0; x < intensityCountArray[y / widthFactor]; x++) {
            histogramImage.at<uchar>(histogramHeight - x - 1, y) = 255;
        }
    }

    return histogramImage;
}

cv::Mat RGBImageProcessor::histogramUniform(cv::Mat image, int gMax, int gMin) {
    cv::Mat result = image.clone();
    for (int channel = 0; channel <= 2; channel++) {
        uint histogramHeight = 0;
        std::array<uint, UCHAR_MAX + 1> histogram = this->computeHistogram(image, channel, histogramHeight);

        int totalPixels = image.rows * image.cols;

        uint cdf[256] = {};
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; i++) {
            cdf[i] = cdf[i - 1] + histogram[i];
        }

        uchar lut[256] = {};
        for (int i = 0; i < 256; i++) {
            lut[i] = std::clamp(
                gMin + static_cast<int>(std::round((gMax - gMin) * (static_cast<double>(cdf[i]) / totalPixels))),
                0,
                UCHAR_MAX
                );
        }

        for (int x = 0; x < image.rows; x++) {
            for (int y = 0; y < image.cols; y++) {
                result.at<cv::Vec3b>(x, y)[channel] = lut[image.at<cv::Vec3b>(x, y)[channel]];
            }
        }
    }
    return result;
}

double RGBImageProcessor::mean(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * i;
        pixels += imageHistogram[i];
    }

    return sum / pixels;
}

double RGBImageProcessor::variance(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 2);
        pixels += imageHistogram[i];
    }

    return sum / pixels;
}

double RGBImageProcessor::standardDeviation(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double variance = this->variance(imageHistogram);
    return sqrt(variance);
}

double RGBImageProcessor::variation1(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double variance = this->variance(imageHistogram);
    double mean = this->mean(imageHistogram);
    return variance / mean;
}

double RGBImageProcessor::asymmetry(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 3);
        pixels += imageHistogram[i];
    }

    double standardDeviation = this->standardDeviation(imageHistogram);

    return sum / pixels / pow(standardDeviation, 3);
}

double RGBImageProcessor::flattening(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 4);
        pixels += imageHistogram[i];
    }

    double variance = this->variance(imageHistogram);

    return sum / pixels / pow(variance, 2)  - 3;
}

double RGBImageProcessor::variation2(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (unsigned int i : imageHistogram) {
        sum += pow(i, 2);
        pixels += i;
    }

    return sum / pow(pixels, 2);
}

double RGBImageProcessor::entropy(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (unsigned int i : imageHistogram) {
        pixels += i;
    }

    for (unsigned int i : imageHistogram) {
        if (i > 0) {
            sum += i * log2(i / pixels);
        }
    }

    return - sum / pixels;
}

cv::Mat padImage(const cv::Mat& image, int padValue) {
    cv::Mat paddedImage = cv::Mat::zeros(image.rows + padValue * 2, image.cols + padValue * 2, CV_8UC3);
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                paddedImage.at<cv::Vec3b>(x + padValue, y + padValue)[z] = image.at<cv::Vec3b>(x, y)[z];
            }
        }
    }
    return paddedImage;
}

cv::Mat RGBImageProcessor::laplacianFilter(cv::Mat image, int laplaceMask) {
    cv::Mat result = image.clone();
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

    cv::Mat paddedImage = padImage(image, 1);
    for (int x = 1; x < paddedImage.rows - 1; x++) {
        for (int y = 1; y < paddedImage.cols - 1; y++) {
            for (int z = 0; z < 3; z++) {
                int convolutionValue = 0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        convolutionValue += paddedImage.at<cv::Vec3b>(x + i, y + j)[z] * mask[i + 1][j + 1];
                    }
                }
                convolutionValue = std::min(std::abs(convolutionValue), 255);

                result.at<cv::Vec3b>(x - 1, y - 1)[z] = convolutionValue;
            }
        }
    }

    return result;
}

cv::Mat RGBImageProcessor::optimizedLaplacianFilter(cv::Mat image) {
    cv::Mat newImage = cv::Mat::zeros(image.size(), CV_8UC3);
    for (int x = 0; x < newImage.rows; x++) {
        for (int y = 0; y < newImage.cols; y++) {
            for (int z = 0; z < 3; z++) {
                int convolutionValue = 0;
                if (x - 1 >= 0) convolutionValue += image.at<cv::Vec3b>(x - 1, y)[z] * -1;

                if (y - 1 >= 0) convolutionValue += image.at<cv::Vec3b>(x, y - 1)[z] * -1;
                convolutionValue += image.at<cv::Vec3b>(x, y)[z] * 4;
                if (y + 1 < newImage.cols) convolutionValue += image.at<cv::Vec3b>(x, y + 1)[z] * -1;
                if (x + 1 < newImage.rows) convolutionValue += image.at<cv::Vec3b>(x + 1, y)[z] * -1;
                convolutionValue = std::min(std::abs(convolutionValue), 255);

                newImage.at<cv::Vec3b>(x, y)[z] = convolutionValue;
            }
        }
    }
    return newImage;
}

cv::Mat RGBImageProcessor::robertsOperator1(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows - 1; x++) {
        for (int y = 0; y < image.cols - 1; y++) {
            for (int z = 0; z < 3; z++) {
                result.at<cv::Vec3b>(x, y)[z] = std::clamp(static_cast<int>(sqrt(
                pow(image.at<cv::Vec3b>(x, y)[z] - image.at<cv::Vec3b>(x + 1, y + 1)[z], 2)
                + pow(image.at<cv::Vec3b>(x, y + 1)[z] - image.at<cv::Vec3b>(x + 1, y)[z], 2)
                )), 0, UCHAR_MAX);
            }
        }
    }
    return result;
}

cv::Mat RGBImageProcessor::regionGrowing(cv::Mat image, int criterion) {
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
                double regionMean = 0;
                for (int z = 0; z < 3; z++) {
                    regionMean += image.at<cv::Vec3b>(seedX, seedY)[z];
                }
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
                        if (newX >= 0 && newX < image.rows && newY >= 0 && newY < image.cols
                        && imageSegmentationMasks.at<uchar>(newX, newY) == 0
                        && ((std::abs(regionMean
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[0])
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[1])
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[2]))
                            <= regionMean * thresholdK && criterion == 0)
                        || (std::abs(regionMean
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[0])
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[1])
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[2]))
                            <= thresholdMean && criterion == 1)
                        || (std::abs(image.at<cv::Vec3b>(seedX, seedY)[0]
                            + image.at<cv::Vec3b>(seedX, seedY)[1]
                            + image.at<cv::Vec3b>(seedX, seedY)[2]
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[0])
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[1])
                            - static_cast<double>(image.at<cv::Vec3b>(newX, newY)[2]))
                            <= thresholdMean && criterion == 2))
                        ) {
                            queue.emplace(newX, newY);
                            imageSegmentationMasks.at<uchar>(newX, newY) = currentMask;
                            double regionSum = regionMean * regionCount;
                            regionCount++;
                            for (int z = 0; z < 3; z++) {
                                regionSum += image.at<cv::Vec3b>(newX, newY)[z];
                            }
                            regionMean = regionSum / regionCount;
                        }
                    }
                }
            }
        }
    }
    return imageSegmentationMasks;
}

cv::Mat RGBImageProcessor::inverseFourierTransform(std::vector<cv::Mat> fourierImages) {
    int M = fourierImages[0].rows;
    int N = fourierImages[0].cols;
    cv::Mat result = cv::Mat::zeros(M, N, CV_8UC3);

    std::vector<std::complex<double>> rowExps(M * M), colExps(N * N);
#pragma omp parallel for
    for (int u = 0; u < M; u++) {
        for (int x = 0; x < M; x++) {
            rowExps[u * M + x] = std::exp(std::complex<double>(0, 2.0 * std::numbers::pi * (static_cast<double>(u * x) / M)));
        }
    }

#pragma omp parallel for
    for (int v = 0; v < N; v++) {
        for (int y = 0; y < N; y++) {
            colExps[v * N + y] = std::exp(std::complex<double>(0, 2.0 * std::numbers::pi * (static_cast<double>(v * y) / N)));
        }
    }

    for (int channel = 0; channel < fourierImages.size(); channel++) {
        cv::Mat shiftedInput = fourierImages[channel].clone();

        for (int u = 0; u < M / 2; u++) {
            for (int v = 0; v < N / 2; v++) {
                std::swap(shiftedInput.at<cv::Vec2d>(u, v), shiftedInput.at<cv::Vec2d>(u + M/2, v + N/2));
                std::swap(shiftedInput.at<cv::Vec2d>(u + M/2, v), shiftedInput.at<cv::Vec2d>(u, v + N/2));
            }
        }

#pragma omp parallel for collapse(2)
        for (int x = 0; x < M; x++) {
            for (int y = 0; y < N; y++) {
                std::complex<double> sum(0, 0);
                for (int u = 0; u < M; u++) {
                    const auto factorX = rowExps[u * M + x];
                    for (int v = 0; v < N; v++) {
                        cv::Vec2d value = shiftedInput.at<cv::Vec2d>(u, v);
                        std::complex<double> input(value[0], value[1]);
                        sum += input * factorX * colExps[v * N + y];
                    }
                }
                double pixelValue = std::abs(sum) / (M * N);
                result.at<cv::Vec3b>(x, y)[channel] = static_cast<uchar>(
                    std::min(255.0, std::max(0.0, std::round(pixelValue)))
                );
            }
            std::cout << "Done: " << x << " u iterations." << "\n";
        }
    }

    return result;
}

cv::Mat RGBImageProcessor::inverseFastFourierTransform(std::vector<cv::Mat> fourierImages) {
    int M = fourierImages[0].rows;
    int N = fourierImages[0].cols;

    cv::Mat result = cv::Mat::zeros(M, N, CV_8UC3);

    for (int i = 0; i < 3; i++) {
        cv::Mat shiftedInput = fourierImages[i].clone();

        for (int u = 0; u < M / 2; u++) {
            for (int v = 0; v < N / 2; v++) {
                std::swap(shiftedInput.at<cv::Vec2d>(u, v), shiftedInput.at<cv::Vec2d>(u + M/2, v + N/2));
                std::swap(shiftedInput.at<cv::Vec2d>(u + M/2, v), shiftedInput.at<cv::Vec2d>(u, v + N/2));
            }
        }

#pragma omp parallel
        {
#pragma omp for
            for (int x = 0; x < M; x++) {
                cv::Mat row = cv::Mat::zeros(1, N, CV_64FC2);
                for (int y = 0; y < N; y++) {
                    row.at<cv::Vec2d>(0, y)[0] = shiftedInput.at<cv::Vec2d>(x, y)[0];
                    row.at<cv::Vec2d>(0, y)[1] = shiftedInput.at<cv::Vec2d>(x, y)[1];
                }
                ifft1D(row);
                for (int y = 0; y < N; y++) {
                    shiftedInput.at<cv::Vec2d>(x, y)[0] = row.at<cv::Vec2d>(0, y)[0];
                    shiftedInput.at<cv::Vec2d>(x, y)[1] = row.at<cv::Vec2d>(0, y)[1];
                }
            }
        }

#pragma omp parallel
{
#pragma omp for
    for (int y = 0; y < N; y++) {
        cv::Mat col = cv::Mat::zeros(1, M, CV_64FC2);
        for (int x = 0; x < M; x++) {
            col.at<cv::Vec2d>(0, x)[0] = shiftedInput.at<cv::Vec2d>(x, y)[0];
            col.at<cv::Vec2d>(0, x)[1] = shiftedInput.at<cv::Vec2d>(x, y)[1];
        }
        ifft1D(col);
        for (int x = 0; x < M; x++) {
            shiftedInput.at<cv::Vec2d>(x, y)[0] = col.at<cv::Vec2d>(0, x)[0];
            shiftedInput.at<cv::Vec2d>(x, y)[1] = col.at<cv::Vec2d>(0, x)[1];
        }
    }
}



#pragma omp parallel for collapse(2)
        for (int x = 0; x < M; x++) {
            for (int y = 0; y < N; y++) {
                std::complex<double> complexPixelValue(
                    shiftedInput.at<cv::Vec2d>(x, y)[0],
                    shiftedInput.at<cv::Vec2d>(x, y)[1]
                    );
                double pixelValue = std::abs(complexPixelValue) / M / N;
                result.at<cv::Vec3b>(x, y)[i] = static_cast<uchar>(
                    std::clamp(std::round(pixelValue), 0.0, 255.0)
                    );
            }
        }
    }

    return result;
}

