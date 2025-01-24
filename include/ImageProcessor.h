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
    /**
     * Compute complement (negative) of binary image.
     * @param image Input binary image
     * @return Complemented image
     */
    cv::Mat complement(cv::Mat image);

    /**
     * Compute union of two binary images.
     * @param image1 First input binary image
     * @param image2 Second input binary image
     * @return Union of input images
     */
    cv::Mat imagesUnion(cv::Mat image1, cv::Mat image2);

    /**
     * Visualize Fourier transform result.
     * @param fourierImage Fourier transform result
     * @param fourierVisPath Path to save visualization
     */
    void visualizeFourier(cv::Mat fourierImage, const std::string &fourierVisPath);

protected:
    /**
     * Check if two images are equal.
     * @param image1 First image to compare
     * @param image2 Second image to compare
     * @return true if images are equal, false otherwise
     */
    bool areEqual(cv::Mat image1, cv::Mat image2);

    /**
     * Perform 1D Fast Fourier Transform on image row.
     * @param row Image row to transform
     */
    void FFT1D(cv::Mat row);

    /**
     * Perform 1D Inverse Fast Fourier Transform on image row.
     * @param row Image row to transform
     */
    void iFFT1D(cv::Mat row);

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

    virtual std::array<uint, UCHAR_MAX + 1> computeHistogram(cv::Mat image, int histogramChannel, uint &histogramMaxVal)
    = 0;

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

    virtual cv::Mat inverseFastFourierTransform(std::vector<cv::Mat> fourierImages) = 0;

    enum class FieldType {
        WHITE,
        BLACK,
        WHITE_MARKER,
        BLACK_MARKER,
        INACTIVE
    };

    static const std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMap;
    static const std::unordered_map<int, std::vector<std::vector<FieldType> > > hmtMaskMap;
    static const std::unordered_map<int, std::vector<std::vector<FieldType> > > hmtComplementMaskMap;

    /**
     * Apply dilation morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Dilated image
     */
    cv::Mat dilation(cv::Mat image, int maskNumber,
                     std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMapping = maskMap);

    /**
     * Apply erosion morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Eroded image
     */
    cv::Mat erosion(cv::Mat image, int maskNumber,
                    std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMapping = maskMap);

    /**
     * Apply opening morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Opened image
     */
    cv::Mat opening(cv::Mat image, int maskNumber,
                    std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMapping = maskMap);

    /**
     * Apply closing morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Closed image
     */
    cv::Mat closing(cv::Mat image, int maskNumber,
                    std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMapping = maskMap);

    /**
     * Apply hit-or-miss transform.
     * @param image Input binary image
     * @param maskNumber HMT mask number
     * @param maskMapping Map of available HMT masks
     * @return Transformed image
     */
    cv::Mat hmt(cv::Mat image, int maskNumber,
                std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMapping = hmtMaskMap);

    /**
     * Custom morphological operation sequence.
     * @param image Input binary image
     * @return Processed image
     */
    cv::Mat taskM4(cv::Mat image);

    /**
     * Create color map for visualization.
     * @return Vector of BGR colors
     */
    std::vector<cv::Vec3b> createColorMap();

    /**
     * Apply color map to grayscale image.
     * @param grayscaleMask Input grayscale image
     * @param colorMap Color map to apply
     * @return Colored image
     */
    cv::Mat applyColorMap(const cv::Mat &grayscaleMask, const std::vector<cv::Vec3b> &colorMap);

    /**
     * Compute Fourier transform.
     * @param image Input image
     * @param fourierVisPath Path to save visualization
     * @return Fourier transformed image
     */
    cv::Mat fourierTransform(cv::Mat image, const std::string &fourierVisPath);

    /**
     * Compute Fast Fourier Transform.
     * @param image Input image
     * @param fourierVisPath Path to save visualization
     * @return FFT transformed image
     */
    cv::Mat fastFourierTransform(cv::Mat image, const std::string &fourierVisPath);

    /**
     * Apply low-pass filter in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowPassBandSize Filter cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftLowPass(cv::Mat fourierImage, int lowPassBandSize, const std::string &fourierVisPath);

    /**
     * Apply high-pass filter in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowPassBandSize Filter cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftHighPass(cv::Mat fourierImage, int lowPassBandSize, const std::string &fourierVisPath);

    /**
     * Apply band-pass filter in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowCut Lower cutoff frequency
     * @param highCut Higher cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftBandPass(cv::Mat fourierImage, int lowCut, int highCut, const std::string &fourierVisPath);

    /**
     * Apply band-cut filter in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowPass Lower cutoff frequency
     * @param highPass Higher cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftBandCut(cv::Mat fourierImage, int lowPass, int highPass, const std::string &fourierVisPath);

    /**
     * Apply directional high-pass filter in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param mask Direction mask
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftHighPassDirection(cv::Mat fourierImage, cv::Mat mask, const std::string &fourierVisPath);

    /**
     * Modify phase in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param k Phase modification parameter k (vertical offset)
     * @param l Phase modification parameter l (horizontal offset)
     * @param fourierVisPath Path to save visualization
     * @return Modified image
     */
    cv::Mat fftPhaseModifying(cv::Mat fourierImage, int k, int l, const std::string &fourierVisPath);
};

#endif //IMAGEPROCESSOR_H
