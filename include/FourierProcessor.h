//
// Created by gluckasz on 1/29/25.
//

#ifndef FOURIERPROCESSOR_H
#define FOURIERPROCESSOR_H
#include <opencv2/opencv.hpp>


namespace FourierProcessor {
    /**
     * Visualize Fourier transform result.
     * @param fourierImage Fourier transform result
     * @param fourierVisPath Path to save visualization
     */
    void visualizeFourier(cv::Mat fourierImage, const std::string &fourierVisPath);

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
     * Apply low-pass filter in the frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowPassBandSize Filter cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftLowPass(const cv::Mat &fourierImage, int lowPassBandSize, const std::string &fourierVisPath);

    /**
     * Apply high-pass filter in the frequency domain.
     * @param fourierImage Fourier transformed image
     * @param highPassBandSize Filter cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftHighPass(cv::Mat fourierImage, int highPassBandSize, const std::string &fourierVisPath);

    /**
     * Apply band-pass filter in the frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowCut Lower cutoff frequency
     * @param highCut Higher cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftBandPass(const cv::Mat &fourierImage, int lowCut, int highCut, const std::string &fourierVisPath);

    /**
     * Apply band-cut filter in the frequency domain.
     * @param fourierImage Fourier transformed image
     * @param lowPass Lower cutoff frequency
     * @param highPass Higher cutoff frequency
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftBandCut(cv::Mat fourierImage, int lowPass, int highPass, const std::string &fourierVisPath);

    /**
     * Apply directional high-pass filter in the frequency domain.
     * @param fourierImage Fourier transformed image
     * @param mask Direction mask
     * @param fourierVisPath Path to save visualization
     * @return Filtered image
     */
    cv::Mat fftHighPassDirection(cv::Mat fourierImage, cv::Mat mask, const std::string &fourierVisPath);

    /**
     * Modify phase in frequency domain.
     * @param fourierImage Fourier transformed image
     * @param verticalShift Phase modification parameter k (vertical offset)
     * @param horizontalShift Phase modification parameter l (horizontal offset)
     * @param fourierVisPath Path to save visualization
     * @return Modified image
     */
    cv::Mat fftPhaseModifying(cv::Mat fourierImage, int verticalShift, int horizontalShift,
                              const std::string &fourierVisPath);

    /**
     * Compute inverse Fourier transform.
     * @param fourierImage Vector of Fourier transformed image components
     * @return Reconstructed spatial domain image
     */
    cv::Mat inverseFourierTransform(const cv::Mat &fourierImage);

    /**
    * Compute inverse Fast Fourier transform.
    * @param fourierImage Vector of Fourier transformed image components
    * @return Reconstructed spatial domain image
    */
    cv::Mat inverseFastFourierTransform(const cv::Mat &fourierImage);
}


#endif //FOURIERPROCESSOR_H
