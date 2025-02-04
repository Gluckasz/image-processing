//
// Created by gluckasz on 2/4/25.
//
#include <fstream>

#include "ImageComparer.h"
#include "ImageOperation.h"
#include "OutputManager.h"

class CompareImageStatsOperation final : public ImageOperation {
    cv::Mat &noNoiseImage_;
    cv::Mat &noiseImage_;

public:
    explicit CompareImageStatsOperation(cv::Mat &noNoiseImage, cv::Mat &noiseImage) : noNoiseImage_(noNoiseImage),
        noiseImage_(noiseImage) {
    }

    void apply(cv::Mat &newImage) const override {
        std::vector<cv::Mat> noiseImageChannels, noNoiseImageChannels, newImageChannels;
        split(noiseImage_, noiseImageChannels);
        split(noNoiseImage_, noNoiseImageChannels);
        split(newImage, newImageChannels);
        double mseBefore = 0, mseAfter = 0, pmseBefore = 0, pmseAfter = 0, snrBefore = 0, snrAfter = 0, psnrBefore = 0,
                psnrAfter = 0, mdBefore = 0, mdAfter = 0;
        for (int i = 0; i < newImageChannels.size(); i++) {
            mseBefore += ImageComparer::meanSquareError(noNoiseImageChannels[i], noiseImageChannels[i]);
            mseAfter += ImageComparer::meanSquareError(noNoiseImageChannels[i], newImageChannels[i]);
            pmseBefore += ImageComparer::peakMeanSquareError(noNoiseImageChannels[i], noiseImageChannels[i]);
            pmseAfter += ImageComparer::peakMeanSquareError(noNoiseImageChannels[i], newImageChannels[i]);
            snrBefore += ImageComparer::signalToNoiseRatio(noNoiseImageChannels[i], noiseImageChannels[i]);
            snrAfter += ImageComparer::signalToNoiseRatio(noNoiseImageChannels[i], newImageChannels[i]);
            psnrBefore += ImageComparer::peakSignalToNoiseRatio(noNoiseImageChannels[i], noiseImageChannels[i]);
            psnrAfter += ImageComparer::peakSignalToNoiseRatio(noNoiseImageChannels[i], newImageChannels[i]);
            mdBefore += ImageComparer::maximumDifference(noNoiseImageChannels[i], noiseImageChannels[i]);
            mdAfter += ImageComparer::maximumDifference(noNoiseImageChannels[i], noiseImageChannels[i]);
        }
        std::stringstream ss;
        ss << "Comparison image stats:\n";
        ss << "Mean square error before denoising: " << mseBefore / static_cast<double>(newImageChannels.size()) << "\n"
                << "Mean square error after denoising: " << mseAfter / static_cast<double>(newImageChannels.size()) <<
                "\n"
                << "Peak mean square error before denoising: " << pmseBefore / static_cast<double>(newImageChannels.
                    size()) << "\n"
                << "Peak mean square error after denoising: " << pmseAfter / static_cast<double>(newImageChannels.
                    size()) << "\n"
                << "Signal to noise ratio before denoising: " << snrBefore / static_cast<double>(newImageChannels.
                    size()) << "\n"
                << "Signal to noise ratio after denoising: " << snrAfter / static_cast<double>(newImageChannels.size())
                << "\n"
                << "Peak signal to noise ratio before denoising: " << psnrBefore / static_cast<double>(newImageChannels.
                    size()) << "\n"
                << "Peak signal to noise ratio before denoising: " << psnrAfter / static_cast<double>(newImageChannels.
                    size()) << "\n"
                << "Maximum difference before denoising: " << mdBefore / static_cast<double>(newImageChannels.size()) <<
                "\n"
                << "Maximum difference after denoising: " << mdAfter / static_cast<double>(newImageChannels.size()) <<
                "\n";
        const std::string path = OutputManager::constructPath("image", "comparison_stats", "txt");
        OutputManager::saveFile(path, ss);
    }
};
