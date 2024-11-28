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

std::array<uint, UCHAR_MAX + 1> GrayscaleImageProcessor::computeHistogram(cv::Mat image, int histogramChannel, uint& histogramMaxVal) {
    std::array<uint, UCHAR_MAX + 1> intensityCountArray = {};
    histogramMaxVal = 0;

    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            uint intensity = image.at<uchar>(x, y);
            intensityCountArray[intensity]++;
            if (intensityCountArray[intensity] > histogramMaxVal) {
                histogramMaxVal = intensityCountArray[intensity];
            }
        }
    }

    return intensityCountArray;
}



cv::Mat GrayscaleImageProcessor::histogram(cv::Mat image, int histogramChannel) {

    uint histogramHeight = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = computeHistogram(image, histogramChannel, histogramHeight);

    uint widthFactor = 8;
    uint histogramWidth = 256 * widthFactor;
    cv::Mat histogramImage = cv::Mat::zeros(histogramHeight, histogramWidth + widthFactor, CV_8UC1);
    for (int y = 0; y < histogramWidth; y++) {
        for (int x = 0; x < histogram[y / widthFactor]; x++) {
            histogramImage.at<uchar>(histogramHeight - x - 1, y) = 255;
        }
    }
    return histogramImage;
}

cv::Mat GrayscaleImageProcessor::histogramUniform(cv::Mat image, int gMax, int gMin) {
    uint histogramHeight = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = this->computeHistogram(image, 0, histogramHeight);

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
            image.at<uchar>(x, y) = lut[image.at<uchar>(x, y)];
        }
    }
    return image;
}

double GrayscaleImageProcessor::mean(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * i;
        pixels += imageHistogram[i];
    }

    return sum / pixels;
}

double GrayscaleImageProcessor::variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    double mean = this->mean(imageHistogram);
    for (int i = 0; i < imageHistogram.size(); i++) {
        sum += imageHistogram[i] * pow(i - mean, 2);
        pixels += imageHistogram[i];
    }

    return sum / pixels;
}

double GrayscaleImageProcessor::standardDeviation(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double variance = this->variance(imageHistogram);
    return sqrt(variance);
}

double GrayscaleImageProcessor::variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double variance = this->variance(imageHistogram);
    double mean = this->mean(imageHistogram);
    return variance / mean;
}

double GrayscaleImageProcessor::asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
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

double GrayscaleImageProcessor::flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
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

double GrayscaleImageProcessor::variation2(std::array<uint, UCHAR_MAX + 1> imageHistogram) {
    double sum = 0;
    double pixels = 0;
    for (unsigned int i : imageHistogram) {
        sum += pow(i, 2);
        pixels += i;
    }

    return sum / pow(pixels, 2);
}

double GrayscaleImageProcessor::entropy(std::array<uint, (127 * 2 + 1) + 1> imageHistogram) {
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
    for (int x = 1; x < paddedImage.rows - 1; x++) {
        for (int y = 1; y < paddedImage.cols - 1; y++) {
            int convolutionValue = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                   convolutionValue += paddedImage.at<uchar>(x + i, y + j) * mask[i + 1][j + 1];
                }
            }
            convolutionValue = std::clamp(convolutionValue, 0, 255);

            image.at<uchar>(x - 1, y - 1) = convolutionValue;
        }
    }

    return image;
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
                convolutionValue = std::clamp(convolutionValue, 0, 255);

                newImage.at<uchar>(x, y) = convolutionValue;
            }
        }
    return newImage;
}

cv::Mat GrayscaleImageProcessor::robertsOperator1(cv::Mat image) {
    for (int x = 0; x < image.rows - 1; x++) {
        for (int y = 0; y < image.cols - 1; y++) {
            image.at<uchar>(x, y) = std::clamp(static_cast<int>(sqrt(
                pow(image.at<uchar>(x, y) - image.at<uchar>(x + 1, y + 1), 2)
                + pow(image.at<uchar>(x, y + 1) - image.at<uchar>(x + 1, y), 2)
                )), 0, UCHAR_MAX);
        }
    }
    return image;
}
