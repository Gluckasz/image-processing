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
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            float normalizedPixelVal = static_cast<float>(image.at<uchar>(x, y)) / 255.0f;
            float gammaCorrectedVal = pow(normalizedPixelVal, modVal);
            int gammaCorrectedPixel = round(gammaCorrectedVal * 255);

            gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
            image.at<uchar>(x, y) = gammaCorrectedPixel;
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::negative(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            image.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::flipHorizontally(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            uchar temp = image.at<uchar>(x, y);
            image.at<uchar>(x, y) = image.at<uchar>(x, image.cols - y - 1);
            image.at<uchar>(x, image.cols - y - 1) = temp;
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::flipVertically(cv::Mat image) {
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            uchar temp = image.at<uchar>(x, y);
            image.at<uchar>(x, y) = image.at<uchar>(image.rows - x - 1, y);
            image.at<uchar>(image.rows - x - 1, y)= temp;
        }
    }
    return image;
}

cv::Mat GrayscaleImageProcessor::flipDiagonally(cv::Mat image) {
    image = this->flipHorizontally(image);
    return this->flipVertically(image);
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

float computeMSEGrayscale(cv::Mat compareImage, cv::Mat secondImage) {
    int squareDistanceSum = 0;

    for (int y = 0; y < compareImage.cols; y++) {
        for (int x = 0; x < compareImage.rows; x++) {
            squareDistanceSum += pow(compareImage.at<uchar>(y, x) - secondImage.at<uchar>(y, x) , 2);
        }
    }

    return static_cast<float>(squareDistanceSum)
    / static_cast<float>(compareImage.cols)
    / static_cast<float>(compareImage.rows);
}

std::string GrayscaleImageProcessor::meanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    float mseBefore = computeMSEGrayscale(compareImage, originalImage);

    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float mseAfter= computeMSEGrayscale(compareImage, newImage);

    std::stringstream ss;
    ss << "Mean square error before denoising: " << mseBefore << "\n"
    << "Mean square error after denoising: " << mseAfter << "\n";

    return ss.str();
}

float computeMSEAndSetMaxGrayscale(cv::Mat compareImage, cv::Mat secondImage, uchar &max) {
    int squareDistanceSum = 0;

    for (int y = 0; y < compareImage.cols; y++) {
        for (int x = 0; x < compareImage.rows; x++) {
            squareDistanceSum += pow(compareImage.at<uchar>(y, x) - secondImage.at<uchar>(y, x) , 2);
            if (compareImage.at<uchar>(y, x) > max) {
                max = compareImage.at<uchar>(y, x);
            }
        }
    }
    return static_cast<float>(squareDistanceSum)
    / static_cast<float>(compareImage.cols)
    / static_cast<float>(compareImage.rows);
}

std::string GrayscaleImageProcessor::peakMeanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    uchar max = 0;

    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    float pmseBefore = computeMSEAndSetMaxGrayscale(compareImage, originalImage, max)
    / static_cast<float>(pow(max, 2));


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }
    float pmseAfter= computeMSEGrayscale(compareImage, newImage)
    / static_cast<float>(pow(max, 2));

    std::stringstream ss;
    ss << "Peak mean square error before denoising: " << pmseBefore << "\n"
    << "Peak mean square error after denoising: " << pmseAfter << "\n";

    return ss.str();
}

int computeSquareSumGrayscale(cv::Mat image) {
    int sum = 0;

    for (int y = 0; y < image.cols; y++) {
        for (int x = 0; x < image.rows; x++) {
            sum += pow(image.at<uchar>(y, x), 2);
        }
    }

    return sum;
}

std::string GrayscaleImageProcessor::signalToNoiseRatio(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }

    auto squareSum = static_cast<float>(computeSquareSumGrayscale(compareImage));

    float snrBefore = 10 * std::log10(
        squareSum
        / computeMSEGrayscale(compareImage, originalImage));


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float snrAfter = 10 * std::log10(
        squareSum
        / computeMSEGrayscale(compareImage, newImage));

    std::stringstream ss;
    ss << "Signal to noise ratio before denoising: " << snrBefore << "\n"
    << "Signal to noise ratio after denoising: " << snrAfter << "\n";

    return ss.str();
}

unsigned long long  computeMaxSquareSumGrayscale(uchar max, int rows, int cols) {
    unsigned long long  sum = 0;
    for (int y = 0; y < cols; y++) {
        for (int x = 0; x < rows; x++) {
            sum += pow(static_cast<int>(max), 2);
        }
    }
    return sum;
}

std::string GrayscaleImageProcessor::peakSignalToNoiseRatio(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    uchar max = 0;

    float mse = computeMSEAndSetMaxGrayscale(compareImage, originalImage, max);
    auto maxSquareSum = static_cast<float>(computeMaxSquareSumGrayscale(max, compareImage.rows, compareImage.cols));

    float psnrBefore = 10 * std::log10(
        maxSquareSum
        / mse);


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float psnrAfter = 10 * std::log10(
        maxSquareSum
        / computeMSEGrayscale(compareImage, newImage));

    std::stringstream ss;
    ss << "Peak signal to noise ratio before denoising: " << psnrBefore << "\n"
    << "Peak signal to noise ratio after denoising: " << psnrAfter << "\n";

    return ss.str();
}

uchar computeMaxGrayscale(cv::Mat image) {
    uchar max = 0;
    for (int y = 0; y < image.cols; y++) {
        for (int x = 0; x < image.rows; x++) {
            if (max < image.at<uchar>(y, x)) {
                max = image.at<uchar>(y, x);
            }
        }
    }

    return max;
}

std::string GrayscaleImageProcessor::maximumDifference(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }

    uchar compareImageMax = computeMaxGrayscale(compareImage);

    int maxDifferenceBefore = abs(compareImageMax - computeMaxGrayscale(originalImage));

    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    int maxDifferenceAfter = abs(compareImageMax - computeMaxGrayscale(newImage));

    std::stringstream ss;
    ss << "Maximum difference before denoising: " << maxDifferenceBefore << "\n"
    << "Maximum difference after denoising: " << maxDifferenceAfter << "\n";

    return ss.str();
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
        for (int x = widthFactor / 2; x < histogram[y / widthFactor]; x++) {
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
