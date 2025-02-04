//
// Created by gluckasz on 2/4/25.
//
#include "../../include/ImageOperation.h"
#include "../../include/input-processing-lib/OutputManager.h"
#include "image-processing-lib/FourierProcessor.h"

class PhaseShiftOperation final : public ImageOperation {
    int vertical_;
    int horizontal_;

public:
    explicit PhaseShiftOperation(const int vertical, const int horizontal) : vertical_(vertical), horizontal_(horizontal) {
    }

    void apply(cv::Mat &image) const override {
        cv::Mat fourierImage = FourierProcessor::fastFourierTransform(image);
        fourierImage = FourierProcessor::fftPhaseModifying(fourierImage, vertical_, horizontal_);
        const std::string path = OutputManager::constructPath("image_fourier", "magnitude_spectrum", "bmp");
        FourierProcessor::visualizeFourier(image, path);
        image = FourierProcessor::inverseFastFourierTransform(fourierImage);
    }
};

