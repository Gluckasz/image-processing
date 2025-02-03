//
// Created by gluckasz on 10/9/24.
//

#include "../include/InputProcessor.h"

InputProcessor::InputProcessor(int argc, char **argv) {
    this->argc = argc;
    this->argv = argv;
}

void InputProcessor::printCommands() {
    std::cout
    << "Available commands:\n"
    << commandToStringMap.find(CommandType::HELP)->second
    << " - print list of all the available commands with detailed description of their arguments.\n\n"
    << commandToStringMap.find(CommandType::OUTPUT)->second
    << "[-fileName=value] - provide output file name.\n"
    << "\t -fileName - output file name (default is output.bmp).\n\n"
    << commandToStringMap.find(CommandType::GRAYSCALE)->second
    << " - read image in grayscale.\n\n"
    << commandToStringMap.find(CommandType::BRIGHTNESS)->second
    << "[-val=value] - modify brightness of an image.\n"
    << "\t -val - integral value to add to each pixel (can be negative).\n\n"
    << commandToStringMap.find(CommandType::CONTRAST_LINEAR)->second
    << "[-val=value] - modify contrast of an image using linear contrast stretching.\n"
    << "\t -val - new value added and subtracted from upper and lower boundary.\n\n"
    << commandToStringMap.find(CommandType::CONTRAST_GAMMA)->second
    << "[-val=value] - modify contrast of an image using gamma correction.\n"
    << "\t -val - floating-point value of gamma variable.\n\n"
    << commandToStringMap.find(CommandType::NEGATIVE)->second
    << " - take negative of an image.\n\n"
    << commandToStringMap.find(CommandType::HORIZONTAL_FLIP)->second
    << " - flip the image horizontally.\n\n"
    << commandToStringMap.find(CommandType::VERTICAL_FLIP)->second
    << " - flip the image vertically.\n\n"
    << commandToStringMap.find(CommandType::DIAGONAL_FLIP)->second
    << " - flip the image diagonally.\n\n"
    << commandToStringMap.find(CommandType::SHRINK)->second
    << "[-val=value] - shrink an image using nearest Neighbor Method.\n"
    << "\t -val - floating-point scale factor of new image (has to be between 0 and 1 non-inclusive).\n\n"
    << commandToStringMap.find(CommandType::ENLARGE)->second
    << "[-val=value] - enlarge an image using nearest Neighbor Method.\n"
    << "\t -val - floating-point scale factor of new image (has to be greater than 1).\n\n"
    << commandToStringMap.find(CommandType::MIDPOINT_FILTER)->second
    << "[-val=value] - apply midpoint filter.\n"
    << "\t -val - integer kernel size value.\n\n"
    << commandToStringMap.find(CommandType::ARITHMETIC_MEAN_FILTER)->second
    << " - apply arithmetic mean filter.\n"
    << "\t -val - integer kernel size value.\n\n"
    << commandToStringMap.find(CommandType::NO_NOISE_IMAGE)->second
    << "[-fileName=value] - provide image with no noise to compare with noisy and denoised image.\n"
    << "\t -fileName - file name of the image with no noise.\n\n"
    << commandToStringMap.find(CommandType::MSE)->second
    << "- compute mean square error.\n\n"
    << commandToStringMap.find(CommandType::PMSE)->second
    << " - compute peak mean square error.\n\n"
    << commandToStringMap.find(CommandType::SNR)->second
    << " - compute signal to noise ratio.\n\n"
    << commandToStringMap.find(CommandType::PSNR)->second
    << " - compute peak signal to noise ratio.\n\n"
    << commandToStringMap.find(CommandType::MAXIMUM_DIFFERENCE)->second
    << " - compute maximum difference.\n\n"
    << commandToStringMap.find(CommandType::HISTOGRAM)->second
    << " - save histogram of an image to output_file_name_histogram.bmp\n"
    << "\t -val - channel of an image for which to compute and save histogram. "
    << "For grayscale images channel does not matter.\n\n"
    << commandToStringMap.find(CommandType::HISTOGRAM_UNIFORM)->second
    << " - improve the quality of an image based on its histogram.\n"
    << "\t -gMax - output image maximum intensity.\n"
    << "\t -gMin - output image minimum intensity.\n\n"
    << commandToStringMap.find(CommandType::MEAN)->second
    << " - compute mean.\n\n"
    << commandToStringMap.find(CommandType::VARIANCE)->second
    << " - compute variance.\n\n"
    << commandToStringMap.find(CommandType::STANDARD_DEVIATION)->second
    << " - compute standard deviation.\n\n"
    << commandToStringMap.find(CommandType::VARIATION_1)->second
    << " - compute variation coefficient I.\n\n"
    << commandToStringMap.find(CommandType::ASYMMETRY)->second
    << " - compute asymmetry coefficient.\n\n"
    << commandToStringMap.find(CommandType::FLATTENING)->second
    << " - compute flattening coefficient.\n\n"
    << commandToStringMap.find(CommandType::VARIATION_2)->second
    << " - compute variation coefficient II.\n\n"
    << commandToStringMap.find(CommandType::ENTROPY)->second
    << " - compute information source entropy.\n\n"
    << commandToStringMap.find(CommandType::LAPLACE)->second
    << " - apply laplacian filter.\n"
    << "\t -val - number of mask to choose (between 0 and 2).\n\n"
    << commandToStringMap.find(CommandType::OPTIMIZED_LAPLACE)->second
    << " - apply optimized laplacian filter.\n\n"
    << commandToStringMap.find(CommandType::ROBERTS_OPERATOR)->second
    << " - apply roberts operator I.\n\n"
    << commandToStringMap.find(CommandType::DILATION)->second
    << " - apply dilation.\n"
    << "\t -mask - number of mask to choose (between 1 and 10).\n\n"
    << commandToStringMap.find(CommandType::EROSION)->second
    << " - apply erosion.\n"
    << "\t -mask - number of mask to choose (between 1 and 10).\n\n"
    << commandToStringMap.find(CommandType::OPENING)->second
    << " - apply opening.\n"
    << "\t -mask - number of mask to choose (between 1 and 10).\n\n"
    << commandToStringMap.find(CommandType::CLOSING)->second
    << " - apply closing.\n"
    << "\t -mask - number of mask to choose (between 1 and 10).\n\n"
    << commandToStringMap.find(CommandType::HMT)->second
    << " - apply HMT.\n"
    << "\t -mask - number of mask to choose (between 1 and 10).\n\n"
    << commandToStringMap.find(CommandType::TASK_M4)->second
    << " - complete task M4.\n\n"
    << commandToStringMap.find(CommandType::REGION_GROWING)->second
    << " - make image segmentation using region growing method.\n"
    << "\t -criterion - criterion to choose (0- adaptive mean distance, 1- fixed mean distance, 2- absolute distance)\n\n"
    << commandToStringMap.find(CommandType::FOURIER_TRANSFORM)->second
    << " - do the fourier transform and then inverse fourier transform.\n\n"
    << commandToStringMap.find(CommandType::FAST_FOURIER_TRANSFORM)->second
    << " - do the fast fourier transform and then inverse fast fourier transform.\n\n"
    << commandToStringMap.find(CommandType::FFT_LOW_PASS)->second
    << " - do the low pass filter after fast fourier transform and then inverse fast fourier transform.\n"
    << "\t -bandValue - radius of the circle which will be preserved.\n\n"
    << commandToStringMap.find(CommandType::FFT_HIGH_PASS)->second
    << " - do the high pass filter after fast fourier transform and then inverse fast fourier transform.\n"
    << "\t -bandValue - radius of the circle which will not be preserved.\n\n"
    << commandToStringMap.find(CommandType::FFT_BAND_PASS)->second
    << " - do the band pass filter after fast fourier transform and then inverse fast fourier transform.\n"
    << "\t -lowCut - radius of the lower bound circle, which will be cut (with high pass filter)\n"
    << "\t -highCut - radius of the upper bound circle up to which the fourier image will be preserved (with low pass filter).\n\n"
    << commandToStringMap.find(CommandType::FFT_BAND_CUT)->second
    << " - do the band cut filter after fast fourier transform and then inverse fast fourier transform.\n"
    << "\t -lowPass - radius of the larger circle. Between its edge and the smaller circle the area will be cut.\n"
    << "\t -highPass - radius of the smaller circle. Between its edge and the larger circle the area will be cut\n\n"
    << commandToStringMap.find(CommandType::FFT_HIGH_PASS_DIRECTION)->second
    << " - do the high-pass filter with detection of edge direction after fast fourier transform and then inverse fast fourier transform.\n"
    << "\t -mask - number of mask to choose (1 or 2)\n\n"
    << commandToStringMap.find(CommandType::FFT_PHASE_MODIFYING)->second
    << " - do the phase modifying filter after fast fourier transform and then inverse fast fourier transform.\n"
    << "\t -k - k coefficient in task F6.\n"
    << "\t -l - l coefficient in task F6.\n\n";
}

cv::Mat InputProcessor::applyFastFourier(const cv::Mat& image, const std::string &fourierVisPath) const {
    cv::Mat result = image.clone();
    std::vector<cv::Mat> channels, fourierImages;
    cv::split(image, channels);
    for (int i = 0; i < channels.size(); i++) {
        cv::Mat fourierImage = FourierProcessor::fastFourierTransform(channels[i],
        fourierVisPath + std::to_string(i) + ".bmp");

        if (lowPassBandSize.has_value()) {
            fourierImage = FourierProcessor::fftLowPass(
                fourierImage,
                lowPassBandSize.value(),
                fourierVisPath + std::to_string(i) + "_after_low_pass.bmp"
                );
        }
        if (highPassBandSize.has_value()) {
            fourierImage = FourierProcessor::fftHighPass(
                fourierImage,
                highPassBandSize.value(),
                fourierVisPath + std::to_string(i) + "_after_high_pass.bmp"
                );
        }

        if (isBandPass) {
            fourierImage = FourierProcessor::fftBandPass(
                fourierImage,
                lowCut.value(),
                highCut.value(),
                fourierVisPath + std::to_string(i) + "_after_band_pass.bmp"
                );
        }

        if (isBandCut) {
            fourierImage = FourierProcessor::fftBandCut(
                fourierImage,
                lowPass.value(),
                highPass.value(),
                fourierVisPath + std::to_string(i) + "_after_band_cut.bmp"
                );
        }

        if (highPassDirectionMask.has_value()) {
            cv::Mat mask;
            if (highPassDirectionMask.value() == 1) {
                mask = cv::imread("/home/gluckasz/Downloads/Images/F5mask1.png", cv::IMREAD_GRAYSCALE);
            } else {
                mask = cv::imread("/home/gluckasz/Downloads/Images/F5mask2.png", cv::IMREAD_GRAYSCALE);
            }
            fourierImage = FourierProcessor::fftHighPassDirection(
                fourierImage,
                mask,
                fourierVisPath + std::to_string(i) + "_after_high_pass_direction.bmp"
                );
        }

        if (taskF6k.has_value() && taskF6l.has_value()) {
            fourierImage = FourierProcessor::fftPhaseModifying(
                fourierImage,
                taskF6k.value(),
                taskF6l.value(),
                fourierVisPath + std::to_string(i) + "_after_phase_modifying.bmp"
                );
        }
        fourierImages.push_back(FourierProcessor::inverseFastFourierTransform(fourierImage));
    }

    cv::merge(fourierImages, result);
    return result;
}


void InputProcessor::applyImageTransformations(cv::Mat &image) {
    cv::Mat result = image.clone();
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    for (int i = 0; i < channels.size(); i++) {
        // Apply brightness modification if set
        if (brightnessModVal.has_value()) {
            channels[i] = SpatialDomainProcessor::modifyBrightness(channels[i], brightnessModVal.value());
        }

        // Apply linear contrast modification if set
        if (contrastLinearModVal.has_value()) {
            channels[i] = SpatialDomainProcessor::modifyContrastLinear(channels[i], contrastLinearModVal.value());
        }

        // Apply gamma contrast modification if set
        if (contrastGammaModVal.has_value()) {
            channels[i] = SpatialDomainProcessor::modifyContrastGamma(channels[i], contrastGammaModVal.value());
        }

        // Apply negative transformation
        if (isNegative) {
            channels[i] = SpatialDomainProcessor::negative(channels[i]);
        }

        // Apply horizontal flip if requested
        if (isHorizontalFlip) {
            channels[i] = SpatialDomainProcessor::flipHorizontally(channels[i]);
        }

        // Apply vertical flip if requested
        if (isVerticalFlip) {
            channels[i] = SpatialDomainProcessor::flipVertically(channels[i]);
        }

        // Apply diagonal flip if requested
        if (isDiagonalFlip) {
            channels[i] = SpatialDomainProcessor::flipDiagonally(channels[i]);
        }

        // Apply a shrink transformation if set
        if (shrinkModVal.has_value()) {
            channels[i] = SpatialDomainProcessor::resize(channels[i], shrinkModVal.value());
        }

        // Apply an enlarge transformation if set
        if (enlargeModVal.has_value()) {
            channels[i] = SpatialDomainProcessor::resize(channels[i], enlargeModVal.value());
        }

        // Apply midpoint filter if kernel size is set
        if (midpointKernelSize.has_value()) {
            channels[i] = SpatialDomainProcessor::midpointFilter(channels[i], midpointKernelSize.value());
        }

        // Apply arithmetic mean filter if kernel size is set
        if (arithmeticMeanKernelSize.has_value()) {
            channels[i] = SpatialDomainProcessor::arithmeticMeanFilter(channels[i], arithmeticMeanKernelSize.value());
        }

        if (histogramUniformGMax.has_value() && histogramUniformGMin.has_value()) {
            std::array<uint, UCHAR_MAX + 1> imageHistogram = HistogramProcessor::computeHistogram(channels[i]);
            channels[i] = HistogramProcessor::histogramUniform(channels[i], imageHistogram, histogramUniformGMax.value(), histogramUniformGMin.value());
        }

        if(isFourierTransform) {
            channels[i] = FourierProcessor::inverseFourierTransform(FourierProcessor::fourierTransform(channels[i],
            OUTPUT_DIR_NAME + "/" + outputFileName.substr(0, outputFileName.length() - 4)
            + "_slow_fourier_visualization_channel" + std::to_string(i) + ".bmp"));
        }
    }

    cv::merge(channels, image);

    if (laplaceMask.has_value()) {
        auto start = std::chrono::high_resolution_clock::now();
        if (channels.size() == 1) {
            image = SpatialDomainProcessor::laplacianFilter<uchar>(image, laplaceMask.value());
        } else {
            image = SpatialDomainProcessor::laplacianFilter<cv::Vec3b>(image, laplaceMask.value());
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::microseconds>(stop - start);

        std::ofstream statsFile;
        statsFile.open(
            OUTPUT_DIR_NAME + "/" + outputFileName.substr(0, outputFileName.length() - 4) + "_laplacian_time" + ".txt");
        statsFile << duration;
        statsFile.close();
    }

    if (isOptimizedLaplacian) {
        auto start = std::chrono::high_resolution_clock::now();
        if (channels.size() == 1) {
            image = SpatialDomainProcessor::optimizedLaplacianFilter<uchar>(image);
        } else {
            image = SpatialDomainProcessor::optimizedLaplacianFilter<cv::Vec3b>(image);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::microseconds>(stop - start);

        std::ofstream statsFile;
        statsFile.open(
            OUTPUT_DIR_NAME + "/" + outputFileName.substr(0, outputFileName.length() - 4) + "_optimized_laplacian_time" + ".txt");
        statsFile << duration;
        statsFile.close();
    }

    if (isRobertsOperator) {
        if (channels.size() == 1) {
            image = SpatialDomainProcessor::robertsOperator1<uchar>(image);
        } else {
            image = SpatialDomainProcessor::robertsOperator1<cv::Vec3b>(image);
        }
    }
    if(dilationMask.has_value()) {
        image = MorphologicalProcessor::dilation(image, dilationMask.value());
    }

    if(erosionMask.has_value()) {
        image = MorphologicalProcessor::erosion(image, erosionMask.value());
    }

    if(openingMask.has_value()) {
        image = MorphologicalProcessor::opening(image, openingMask.value());
    }

    if(closingMask.has_value()) {
        image = MorphologicalProcessor::closing(image, closingMask.value());
    }

    if(hmtMask.has_value()) {
        image = MorphologicalProcessor::hmt(image, hmtMask.value());
    }

    if (!std::filesystem::is_directory(OUTPUT_DIR_NAME) || !std::filesystem::exists(OUTPUT_DIR_NAME)) {
        std::filesystem::create_directory(OUTPUT_DIR_NAME);
    }

    if(isFastFourierTransform) {
        image = applyFastFourier(
            image, OUTPUT_DIR_NAME + "/"
            + outputFileName.substr(0, outputFileName.length() - 4)
            + "_fast_fourier_visualization_channel"
            );
    }
}

void InputProcessor::calculateAndSaveComparisonImageStatistics(
    const cv::Mat &compareImage,
    const cv::Mat &originalImage,
    const cv::Mat &newImage
) {
    std::stringstream ss;
    ss << "Comparison image stats:\n";
    std::vector<cv::Mat> compareImageChannels, originalImageChannels, newImageChannels;
    cv::split(newImage, compareImageChannels);
    cv::split(newImage, originalImageChannels);
    cv::split(newImage, newImageChannels);

    for (int i = 0; i < compareImageChannels.size(); i++) {
        ss << "Comparison image stats channel " << i << ":\n";
        if (isMeanSquareError)
            ss << "Mean square error before denoising: " << ImageComparer::meanSquareError(compareImageChannels[i], originalImageChannels[i]) << "\n"
            << "Mean square error after denoising: " << ImageComparer::meanSquareError(originalImageChannels[i], newImageChannels[i]) << "\n";
        if (isPeakMeanSquareError)
            ss << "Peak mean square error before denoising: " << ImageComparer::peakMeanSquareError(compareImageChannels[i], originalImageChannels[i]) << "\n"
        << "Peak mean square error after denoising: " << ImageComparer::peakMeanSquareError(originalImageChannels[i], newImageChannels[i]) << "\n";
        if (isSignalToNoise)
            ss << "Signal to noise ratio before denoising: " << ImageComparer::signalToNoiseRatio(compareImageChannels[i], originalImageChannels[i]) << "\n"
        << "Signal to noise ratio after denoising: " << ImageComparer::signalToNoiseRatio(originalImageChannels[i], newImageChannels[i]) << "\n";
        if (isPeakSignalToNoise)
            ss << "Signal to noise ratio before denoising: " << ImageComparer::peakSignalToNoiseRatio(compareImageChannels[i], originalImageChannels[i]) << "\n"
        << "Signal to noise ratio after denoising: " << ImageComparer::peakSignalToNoiseRatio(originalImageChannels[i], newImageChannels[i]) << "\n";
        if (isMaximumDifference)
            ss << "Signal to noise ratio before denoising: " << ImageComparer::maximumDifference(compareImageChannels[i], originalImageChannels[i]) << "\n"
        << "Signal to noise ratio after denoising: " << ImageComparer::maximumDifference(originalImageChannels[i], newImageChannels[i]) << "\n";
    }
    std::ofstream statsFile;
    statsFile.open(
        OUTPUT_DIR_NAME + "/" + outputFileName.substr(0, outputFileName.length() - 4) + "_comparison_stats" + ".txt");
    statsFile << ss.rdbuf();
    statsFile.close();
}

void InputProcessor::calculateAndSaveImageStats(const cv::Mat &newImage) {
    std::stringstream ss;
    ss << "Comparison image stats:\n";
    std::vector<cv::Mat> channels;
    cv::split(newImage, channels);

    std::array<std::array<uint, UCHAR_MAX + 1>, 3> imageHistograms{};
    for (int i = 0; i < channels.size(); i++) {
        imageHistograms[i] = HistogramProcessor::computeHistogram(newImage);
        if (isMean) {
            double mean = HistogramProcessor::mean(imageHistograms[i]);
            ss << "Mean channel " << i << ": " << mean << "\n";
        }
        if (isVariance) {
            double variance = HistogramProcessor::variance(imageHistograms[i]);
            ss << "Variance channel " << i << ": " << variance << "\n";
        }
        if (isStandardDeviation) {
            double standardDeviation = HistogramProcessor::standardDeviation(imageHistograms[i]);
            ss << "Standard channel " << i << ": " << standardDeviation << "\n";
        }

        if (isStandardDeviation) {
            double variation1 = HistogramProcessor::variation1(imageHistograms[i]);
            ss << "Variation channel " << i << ": " << variation1 << "\n";
        }

        if (isAsymmetry) {
                double asymmetry = HistogramProcessor::asymmetry(imageHistograms[i]);
                ss << "Asymmetry channel " << i << ": " << asymmetry << "\n";
        }

        if (isFlattening) {
                double flattening = HistogramProcessor::flattening(imageHistograms[i]);
                ss << "Flattening channel " << i << ": "<< flattening << "\n";
        }

        if (isVariation2) {
                double variation2 = HistogramProcessor::variation2(imageHistograms[i]);
                ss << "Variation coefficient II channel " << i << ": " << variation2 << "\n";
        }

        if (isEntropy) {
                double entropy = HistogramProcessor::entropy(imageHistograms[i]);
                ss << "Information source entropy channel " << i << ": " << entropy << "\n";
        }
    }
    std::ofstream statsFile;
    statsFile.open(
        OUTPUT_DIR_NAME + "/" + outputFileName.substr(0, outputFileName.length() - 4) + "_stats" + ".txt");
    statsFile << ss.rdbuf();
    statsFile.close();
}


void InputProcessor::processImage() {
    cv::Mat originalImage = imread(argv[INPUT_IMAGE_POS], imreadMode);
    if (!originalImage.data) {
        std::cout << "No image data \n";
        return;
    }

    cv::Mat newImage;
    originalImage.copyTo(newImage);
    applyImageTransformations(newImage);
    if (histogramChannel) {
        std::string histogramFileName = outputFileName.substr(0, outputFileName.length() - 4) +
                                        "_histogram.png";
        saveImage(HistogramProcessor::histogramVisualization(HistogramProcessor::computeHistogram(newImage), 8), histogramFileName);
    }
    saveImage(newImage, this->outputFileName);

    if (isNoNoise) {
        cv::Mat compareImage = imread(noNoiseImage, imreadMode);
        calculateAndSaveComparisonImageStatistics(compareImage, originalImage, newImage);
        calculateAndSaveImageStats(newImage);
    }

    if (regionGrowing.has_value()) {
        if (imreadMode == cv::IMREAD_COLOR) {
            cv::Mat segmentedImage = SpatialDomainProcessor::regionGrowing<cv::Vec3b>(newImage, regionGrowing.value());
            saveImage(segmentedImage, outputFileName.substr(0, outputFileName.length() - 4) +
                                        "_segmentation.bmp");
            cv::Mat segmentedColorImage = SpatialDomainProcessor::applyColorMap(segmentedImage, SpatialDomainProcessor::createColorMap());
            saveImage(segmentedColorImage, outputFileName.substr(0, outputFileName.length() - 4) +
                                            "_segmentation_colored.bmp");
        } else {
            cv::Mat segmentedImage = SpatialDomainProcessor::regionGrowing<uchar>(newImage, regionGrowing.value());
            saveImage(segmentedImage, outputFileName.substr(0, outputFileName.length() - 4) +
                                        "_segmentation.bmp");
            cv::Mat segmentedColorImage = SpatialDomainProcessor::applyColorMap(segmentedImage, SpatialDomainProcessor::createColorMap());
            saveImage(segmentedColorImage, outputFileName.substr(0, outputFileName.length() - 4) +
                                            "_segmentation_colored.bmp");
        }
    }
}


void InputProcessor::saveImage(cv::Mat image, std::string outputFileName) const {
    if (!std::filesystem::is_directory(OUTPUT_DIR_NAME) || !
        std::filesystem::exists(OUTPUT_DIR_NAME)) {
        std::filesystem::create_directory(OUTPUT_DIR_NAME);
    }
    imwrite(OUTPUT_DIR_NAME + "/" + outputFileName, image);
}
