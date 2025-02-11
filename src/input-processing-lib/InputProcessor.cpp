//
// Created by gluckasz on 10/9/24.
//

#include "../../include/input-processing-lib/InputProcessor.h"


#include "operations/channel-operations/ArithmeticMeanFilterOperation.h"
#include "operations/channel-operations/BandCutOperation.h"
#include "operations/channel-operations/BandPassOperation.h"
#include "operations/channel-operations/BrightnessOperation.h"
#include "operations/channel-operations/ContrastGammaOperation.h"
#include "operations/channel-operations/ContrastLinearOperation.h"
#include "operations/channel-operations/FastFourierOperation.h"
#include "operations/channel-operations/FlipDiagonallyOperation.h"
#include "operations/channel-operations/FlipHorizontallyOperation.h"
#include "operations/channel-operations/FlipVerticallyOperation.h"
#include "operations/channel-operations/FourierOperation.h"
#include "operations/channel-operations/HighPassOperation.h"
#include "operations/channel-operations/HistogramEqualizationOperation.h"
#include "operations/channel-operations/LowPassOperation.h"
#include "operations/channel-operations/MidpointFilterOperation.h"
#include "operations/channel-operations/NegativeOperation.h"
#include "operations/channel-operations/PhaseShiftOperation.h"
#include "operations/channel-operations/ResizeOperation.h"
#include "input-processing-lib/CommandMapping.h"
#include "operations/whole-image-operations/ClosingOperation.h"
#include "operations/whole-image-operations/CompareImageStatsOperation.h"
#include "operations/whole-image-operations/DilationOperation.h"
#include "operations/whole-image-operations/HistogramStatsOperation.h"
#include "operations/whole-image-operations/HistogramVisualizationOperation.h"
#include "operations/whole-image-operations/HMTOperation.h"
#include "../../include/operations/channel-operations/LaplacianFilterOperation.h"
#include "operations/whole-image-operations/OpeningOperation.h"
#include "../../include/operations/channel-operations/OptimizedLaplacianFilterOperation.h"
#include "operations/whole-image-operations/RegionGrowingOperation.h"
#include "../../include/operations/channel-operations/RobertsOperation.h"


void InputProcessor::setupChannelProcessingPipeline() {
    if (options_.arithmeticMeanKernelSize.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<ArithmeticMeanFilterOperation>(options_.arithmeticMeanKernelSize.value()));
    }
    if (options_.lowPass.has_value() && options_.highPass.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<BandCutOperation>(options_.lowPass.value(), options_.highPass.value()));
    }
    if (options_.lowCut.has_value() && options_.highCut.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<BandPassOperation>(options_.lowCut.value(), options_.highCut.value()));
    }
    if (options_.brightnessModVal.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<BrightnessOperation>(options_.brightnessModVal.value()));
    }
    if (options_.contrastGammaModVal.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<ContrastGammaOperation>(options_.contrastGammaModVal.value()));
    }
    if (options_.contrastLinearModVal.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<ContrastLinearOperation>(options_.contrastLinearModVal.value()));
    }
    if (options_.isFastFourierTransform) {
        channelOperations_.emplace_back(
            std::make_unique<FastFourierOperation>());
    }
    if (options_.isDiagonalFlip) {
        channelOperations_.emplace_back(
            std::make_unique<FlipDiagonallyOperation>());
    }
    if (options_.isHorizontalFlip) {
        channelOperations_.emplace_back(
            std::make_unique<FlipHorizontallyOperation>());
    }
    if (options_.isVerticalFlip) {
        channelOperations_.emplace_back(
            std::make_unique<FlipVerticallyOperation>());
    }
    if (options_.isFourierTransform) {
        channelOperations_.emplace_back(
            std::make_unique<FourierOperation>());
    }
    if (options_.highPassBandSize.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<HighPassOperation>(options_.highPassBandSize.value()));
    }
    if (options_.histogramUniformGMin.has_value() && options_.histogramUniformGMax.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<HistogramEqualizationOperation>(options_.histogramUniformGMin.value(), options_.histogramUniformGMax.value()));
    }
    if (options_.lowPassBandSize.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<LowPassOperation>(options_.lowPassBandSize.value()));
    }
    if (options_.midpointKernelSize.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<MidpointFilterOperation>(options_.midpointKernelSize.value()));
    }
    if (options_.isNegative) {
        channelOperations_.emplace_back(
            std::make_unique<NegativeOperation>());
    }
    if (options_.taskF6k.has_value() && options_.taskF6l.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<PhaseShiftOperation>(options_.taskF6k.value(), options_.taskF6l.value()));
    }
    if (options_.resizeModVal.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<ResizeOperation>(options_.resizeModVal.value()));
    }
    if (options_.laplaceMask.has_value()) {
        channelOperations_.emplace_back(
            std::make_unique<LaplacianFilterOperation>(options_.laplaceMask.value()));
    }
    if (options_.isOptimizedLaplacian) {
        channelOperations_.emplace_back(
            std::make_unique<OptimizedLaplacianFilterOperation>());
    }
    if (options_.isRobertsOperator) {
        channelOperations_.emplace_back(
            std::make_unique<RobertsOperation>());
    }
}

void InputProcessor::setupImageProcessingPipeline() {
    if (options_.closingMask.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<ClosingOperation>(options_.closingMask.value()));
    }
    if (options_.dilationMask.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<DilationOperation>(options_.dilationMask.value()));
    }
    if (options_.erosionMask.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<DilationOperation>(options_.erosionMask.value()));
    }
    if (options_.histogramChannel.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<HistogramVisualizationOperation>(options_.histogramChannel.value()));
    }
    if (options_.hmtMask.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<HMTOperation>(options_.hmtMask.value()));
    }
    if (options_.openingMask.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<OpeningOperation>(options_.openingMask.value()));
    }
    if (options_.regionGrowing.has_value()) {
        imageOperations_.emplace_back(
            std::make_unique<RegionGrowingOperation>(options_.regionGrowing.value()));
    }
}

void InputProcessor::setupStatsPipeline() {
    if (options_.isHistogramStats) {
        statsOperations_.emplace_back(
            std::make_unique<HistogramStatsOperation>());
    }
    if (options_.isCompareImages && options_.originalImage) {
        cv::Mat originalImage = imread(inputImagePath_, options_.imreadMode);
        statsOperations_.emplace_back(
            std::make_unique<CompareImageStatsOperation>(originalImage));
    }
}

void InputProcessor::saveResults(const cv::Mat &image) const {
    if (options_.outputFileName.has_value()) {
        std::string path = OutputManager::constructPath(options_.outputFileName.value(), "result", "bmp");
        OutputManager::saveImage(image, path);
    }
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
            << commandToStringMap.find(CommandType::RESIZE)->second
            << "[-val=value] - resize an image using Nearest Neighbor Method.\n"
            << "\t -val - floating-point scale factor of new image.\n\n"
            << commandToStringMap.find(CommandType::MIDPOINT_FILTER)->second
            << "[-val=value] - apply midpoint filter.\n"
            << "\t -val - integer kernel size value.\n\n"
            << commandToStringMap.find(CommandType::ARITHMETIC_MEAN_FILTER)->second
            << " - apply arithmetic mean filter.\n"
            << "\t -val - integer kernel size value.\n\n"
            << commandToStringMap.find(CommandType::COMPARE_IMAGES)->second
            << " - save image comparison stats after all operations.\n\n"
            << commandToStringMap.find(CommandType::HISTOGRAM)->second
            << " - save histogram of an image to output_file_name_histogram.bmp\n"
            << "\t -val - channel of an image for which to compute and save histogram. "
            << "For grayscale images channel does not matter.\n\n"
            << commandToStringMap.find(CommandType::HISTOGRAM_UNIFORM)->second
            << " - improve the quality of an image based on its histogram.\n"
            << "\t -gMax - output image maximum intensity.\n"
            << "\t -gMin - output image minimum intensity.\n\n"
            << commandToStringMap.find(CommandType::HISTOGRAM_STATS)->second
            << " - compute stats based on the image histogram.\n\n"
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

void InputProcessor::process() {
    if (options_.showHelp) {
        printCommands();
    }
    cv::Mat image = imread(inputImagePath_, options_.imreadMode);
    setupChannelProcessingPipeline();
    setupImageProcessingPipeline();
    setupStatsPipeline();

    std::vector<cv::Mat> channels;
    split(image, channels);
    for (auto & channel : channels) {
        for (const auto& op : channelOperations_) {
            op->apply(channel);
        }
    }
    cv::merge(channels, image);

    for (const auto& op : imageOperations_) {
        op->apply(image);
    }

    for (const auto& op : statsOperations_) {
        op->apply(image);
    }

    saveResults(image);
}
