//
// Created by gluckasz on 10/7/24.
//

#include "../include/RGBImageProcessor.h"

cv::Mat RGBImageProcessor::modifyBrightness(cv::Mat image, int modVal) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (modVal < 0) {
                    if (image.at<cv::Vec3b>(x, y)[z] >= 0 - modVal) {
                        image.at<cv::Vec3b>(x, y)[z] += modVal;
                    }
                    else {
                        image.at<cv::Vec3b>(x, y)[z] = 0;
                    }
                }
                else {
                    if (image.at<cv::Vec3b>(x, y)[z]<= UCHAR_MAX - modVal) {
                        image.at<cv::Vec3b>(x, y)[z]+= modVal;
                    }
                    else {
                        image.at<cv::Vec3b>(x, y)[z] = UCHAR_MAX;
                    }
                }
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::modifyContrastLinear(cv::Mat image, int modVal) {
    uchar max[3] = {0, 0, 0};
    uchar min[3] = {255, 255, 255};
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
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
                image.at<cv::Vec3b>(x, y)[z] = std::clamp(
                    (image.at<cv::Vec3b>(x, y)[z] - min[z])
                    * (max[z] - min[z] + 2 * modVal)
                    / (max[z] - min[z]) + min[z] - modVal,
                    0,
                    UCHAR_MAX
                    );
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::modifyContrastGamma(cv::Mat image, float modVal) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                float normalizedPixelVal = static_cast<float>(image.at<cv::Vec3b>(x, y)[z]) / 255.0f;

                float gammaCorrectedVal = pow(normalizedPixelVal, modVal);
                int gammaCorrectedPixel = round(gammaCorrectedVal * 255);

                gammaCorrectedPixel = std::clamp(gammaCorrectedPixel, 0, UCHAR_MAX);
                image.at<cv::Vec3b>(x, y)[z] = gammaCorrectedPixel;
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::negative(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                image.at<cv::Vec3b>(x, y)[z] = 255 - image.at<cv::Vec3b>(x, y)[z];
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::flipHorizontally(cv::Mat image) {
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols / 2; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<cv::Vec3b>(x, y)[z];
                image.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(x, image.cols - y - 1)[z];
                image.at<cv::Vec3b>(x, image.cols - y - 1)[z] = temp;
            }
        }
    }
    return image;
}

cv::Mat RGBImageProcessor::flipVertically(cv::Mat image) {
    for (int x = 0; x < image.rows / 2; x++) {
        for (int y = 0; y < image.cols; y++) {
            for (int z = 0; z < 3; z++) {
                uchar temp = image.at<cv::Vec3b>(x, y)[z];
                image.at<cv::Vec3b>(x, y)[z] = image.at<cv::Vec3b>(image.rows - x - 1, y)[z];
                image.at<cv::Vec3b>(image.rows - x - 1, y)[z] = temp;
            }
        }
    }
    return image;
}
cv::Mat RGBImageProcessor::flipDiagonally(cv::Mat image) {
    image = this->flipHorizontally(image);
    return this->flipVertically(image);
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
    cv::Mat  newImage;
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

float computeMSERGB(cv::Mat compareImage, cv::Mat secondImage) {
    int squareDistanceSum = 0;
    for (int y = 0; y < compareImage.cols; y++) {
        for (int x = 0; x < compareImage.rows; x++) {
            for (int z = 0; z < 3; z++) {
                squareDistanceSum += pow(compareImage.at<cv::Vec3b>(y, x)[z] - secondImage.at<cv::Vec3b>(y, x)[z] , 2);
            }
        }
    }
    return static_cast<float>(squareDistanceSum)
    / static_cast<float>(compareImage.cols)
    / static_cast<float>(compareImage.rows)
    / 3.0f;
}

std::string RGBImageProcessor::meanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {

    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    float mseBefore = computeMSERGB(compareImage, originalImage);

    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }
    float mseAfter= computeMSERGB(compareImage, newImage);

    std::stringstream ss;
    ss << "Mean square error before denoising: " << mseBefore << "\n"
    << "Mean square error after denoising: " << mseAfter << "\n";

    return ss.str();
}

float computeMSEAndSetMaxRGB(cv::Mat compareImage, cv::Mat secondImage, uchar* max) {
    int squareDistanceSum = 0;

    for (int y = 0; y < compareImage.cols; y++) {
        for (int x = 0; x < compareImage.rows; x++) {
            for (int z = 0; z < 3; z++) {
                squareDistanceSum += pow(compareImage.at<cv::Vec3b>(y, x)[z] - secondImage.at<cv::Vec3b>(y, x)[z] , 2);
                if (max[z] < compareImage.at<cv::Vec3b>(y, x)[z]) {
                    max[z] = compareImage.at<cv::Vec3b>(y, x)[z];
                }
            }
        }
    }

    return static_cast<float>(squareDistanceSum)
    / static_cast<float>(compareImage.cols)
    / static_cast<float>(compareImage.rows)
    / 3.0f;
}

std::string RGBImageProcessor::peakMeanSquareError(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }

    uchar max[3] = {0, 0, 0};
    float pmseBefore = computeMSEAndSetMaxRGB(compareImage, originalImage, max);
    float maxSum = 0;
    for (unsigned char i : max) {
        maxSum += pow(i, 2);
    }
    pmseBefore /= maxSum;


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float pmseAfter= computeMSERGB(compareImage, newImage)
    / maxSum;

    std::stringstream ss;
    ss << "Peak mean square error before denoising: " << pmseBefore << "\n"
    << "Peak mean square error after denoising: " << pmseAfter << "\n";

    return ss.str();
}

unsigned long long  computeSquareSumRGB(cv::Mat image) {
    unsigned long long  sum = 0;

    for (int y = 0; y < image.cols; y++) {
        for (int x = 0; x < image.rows; x++) {
            for (int z = 0; z < 3; z++) {
                sum += pow(image.at<cv::Vec3b>(y, x)[z], 2);
            }
        }
    }

    return sum;
}

std::string RGBImageProcessor::signalToNoiseRatio(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }

    auto squareSum = static_cast<float>(computeSquareSumRGB(compareImage));

    float snrBefore = 10 * std::log10(
        squareSum
        / computeMSERGB(compareImage, originalImage));


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float snrAfter = 10 * std::log10(
        squareSum
        / computeMSERGB(compareImage, newImage));

    std::stringstream ss;
    ss << "Signal to noise ratio before denoising: " << snrBefore << "\n"
    << "Signal to noise ratio after denoising: " << snrAfter << "\n";

    return ss.str();
}

unsigned long long computeMaxSquareSumRGB(uchar *max, int rows, int cols) {
    unsigned long long  sum = 0;
    for (int y = 0; y < cols; y++) {
        for (int x = 0; x < rows; x++) {
            for (int z = 0; z < 3; z++) {
                sum += pow(static_cast<int>(max[z]), 2);
            }
        }
    }

    return sum;
}

std::string RGBImageProcessor::peakSignalToNoiseRatio(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }
    uchar max[3] = {0, 0, 0};

    float mse = computeMSEAndSetMaxRGB(compareImage, originalImage, max);
    auto maxSquareSum = static_cast<float>(computeMaxSquareSumRGB(max, compareImage.rows, compareImage.cols));

    float psnrBefore = 10 * std::log10(
        maxSquareSum
        / mse);


    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    float psnrAfter = 10 * std::log10(
        maxSquareSum
        / computeMSERGB(compareImage, newImage));

    std::stringstream ss;
    ss << "Peak signal to noise ratio before denoising: " << psnrBefore << "\n"
    << "Peak signal to noise ratio after denoising: " << psnrAfter << "\n";

    return ss.str();
}

uchar computeMaxRGB(cv::Mat image) {
    uchar max[3] = {0, 0, 0};
    for (int y = 0; y < image.cols; y++) {
        for (int x = 0; x < image.rows; x++) {
            for (int z = 0; z < 3; z++) {
                if (max[z] < image.at<cv::Vec3b>(y, x)[z]) {
                    max[z] = image.at<cv::Vec3b>(y, x)[z];
                }
            }
        }
    }

    return *max;
}

std::string RGBImageProcessor::maximumDifference(cv::Mat compareImage, cv::Mat originalImage, cv::Mat newImage) {
    if (compareImage.rows != originalImage.rows) {
        originalImage = this->resize(originalImage, static_cast<float>(compareImage.rows) / static_cast<float>(originalImage.rows));
    }

    uchar compareImageMax = computeMaxRGB(compareImage);

    int maxDifferenceBefore = abs(compareImageMax - computeMaxRGB(originalImage));

    if (compareImage.rows != newImage.rows) {
        newImage = this->resize(newImage, static_cast<float>(compareImage.rows) / static_cast<float>(newImage.rows));
    }

    int maxDifferenceAfter = abs(compareImageMax - computeMaxRGB(originalImage));

    std::stringstream ss;
    ss << "Maximum difference before denoising: " << maxDifferenceBefore << "\n"
    << "Maximum difference after denoising: " << maxDifferenceAfter << "\n";

    return ss.str();
}

cv::Mat RGBImageProcessor::histogram(cv::Mat image, int histogramChannel) {
    uint intensityCountArray[UCHAR_MAX + 1] = {0};
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            intensityCountArray[image.at<cv::Vec3b>(x, y)[histogramChannel]] += 1;
        }
    }

    // Find max in intensityCountArray to determine histogram image height
    int histogramHeight = 0;
    for (int i : intensityCountArray) {
        histogramHeight = i > histogramHeight ? i : histogramHeight;
    }

    uint widthFactor = 8;
    int histogramWidth = (UCHAR_MAX + 1) * widthFactor;
    cv::Mat histogramImage = cv::Mat::zeros(histogramHeight, histogramWidth + widthFactor, CV_8UC1);
    for (int y = 0; y < histogramWidth; y++) {
        for (int x = widthFactor / 2; x < intensityCountArray[y / widthFactor]; x++) {
            histogramImage.at<uchar>(histogramHeight - x - 1, y) = 255;
        }
    }

    return histogramImage;
}

cv::Mat RGBImageProcessor::histogramUniform(cv::Mat image, int gMax, int gMin) {
    return image;
}
