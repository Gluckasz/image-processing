//
// Created by gluckasz on 2/4/25.
//

#include "../../include/ImageOperation.h"
#include "../../include/input-processing-lib/OutputManager.h"
#include "image-processing-lib/FourierProcessor.h"

class BandCutOperation final : public ImageOperation {
    int low_;
    int high_;

public:
    explicit BandCutOperation(const int low, const int high) : low_(low), high_(high) {
    }

    void apply(cv::Mat &image) const override {
        cv::Mat fourierImage = FourierProcessor::fastFourierTransform(image);
        fourierImage = FourierProcessor::fftBandCut(fourierImage, low_, high_);
        const std::string path = OutputManager::constructPath("image_fourier", "magnitude_spectrum", "bmp");
        FourierProcessor::visualizeFourier(image, path);
        image = FourierProcessor::inverseFastFourierTransform(fourierImage);
    }
};

