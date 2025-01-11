//
// Created by gluckasz on 10/6/24.
//

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <unordered_set>
#include <complex>

class ImageProcessor {
    cv::Mat complement(cv::Mat image);
    bool areEqual(cv::Mat image1, cv::Mat image2);
    cv::Mat imagesUnion(cv::Mat image1, cv::Mat image2);
    void visualizeFourier(cv::Mat fourierImage, const std::string& fourierVisPath);
public:
    virtual ~ImageProcessor() = default;

    virtual cv::Mat modifyBrightness(cv::Mat image, int modVal) = 0;
    virtual cv::Mat modifyContrastLinear(cv::Mat image, int modVal) = 0;
    virtual cv::Mat modifyContrastGamma(cv::Mat image, float modVal) = 0;
    virtual cv::Mat negative(cv::Mat image) = 0;
    virtual cv::Mat flipHorizontally(cv::Mat image) = 0;
    virtual cv::Mat flipVertically(cv::Mat image) = 0;
    virtual cv::Mat flipDiagonally(cv::Mat image) = 0;
    virtual cv::Mat resize(cv::Mat image, float factor) = 0;
    virtual cv::Mat midpointFilter(cv::Mat image, int kernelSize) = 0;
    virtual cv::Mat arithmeticMeanFilter(cv::Mat image, int kernelSize) = 0;
    virtual double meanSquareError(cv::Mat originalImage, cv::Mat newImage) = 0;
    virtual double peakMeanSquareError(cv::Mat originalImage, cv::Mat newImage) = 0;
    virtual double signalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) = 0;
    virtual double peakSignalToNoiseRatio(cv::Mat originalImage, cv::Mat newImage) = 0;
    virtual double maximumDifference(cv::Mat originalImage, cv::Mat newImage) = 0;
    virtual std::array<uint, UCHAR_MAX + 1> computeHistogram(cv::Mat image, int histogramChanngel, uint& histogramMaxVal) = 0;
    virtual cv::Mat histogram(cv::Mat image, int histogramChannel) = 0;
    virtual cv::Mat histogramUniform(cv::Mat image, int gMax, int gMin) = 0;
    virtual double mean(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double variance(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double standardDeviation(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double variation1(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double asymmetry(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double flattening(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double variation2(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual double entropy(std::array<uint, UCHAR_MAX + 1> imageHistogram) = 0;
    virtual cv::Mat laplacianFilter(cv::Mat image, int laplaceMask) = 0;
    virtual cv::Mat optimizedLaplacianFilter(cv::Mat image) = 0;
    virtual cv::Mat robertsOperator1(cv::Mat image) = 0;
    virtual cv::Mat regionGrowing(cv::Mat image, int criterion) = 0;
    virtual cv::Mat inverseFourierTransform(std::vector<cv::Mat> fourierImages) = 0;

    enum class FieldType {
        WHITE,
        BLACK,
        WHITE_MARKER,
        BLACK_MARKER,
        INACTIVE
    };

    static const std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMap;
    static const std::unordered_map<int, std::vector<std::vector<FieldType>>> hmtMaskMap;
    static const std::unordered_map<int, std::vector<std::vector<FieldType>>> hmtComplementMaskMap;

    cv::Mat dilation(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping = maskMap);
    cv::Mat erosion(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping = maskMap);
    cv::Mat opening(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping = maskMap);
    cv::Mat closing(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping = maskMap);
    cv::Mat hmt(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping = hmtMaskMap);
    cv::Mat taskM4(cv::Mat image);

    std::vector<cv::Vec3b> createColorMap();
    cv::Mat applyColorMap(const cv::Mat& grayscaleMask, const std::vector<cv::Vec3b>& colorMap);

    cv::Mat fourierTransform(cv::Mat image, const std::string& fourierVisPath);
    cv::Mat fastFourierTransform(cv::Mat image, const std::string& fourierVisPath);
};

#endif //IMAGEPROCESSOR_H
