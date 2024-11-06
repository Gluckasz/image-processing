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
            << " - improve the quality of an image based on its histogram\n"
            << "\t -gMax - output image maximum intensity.\n"
            << "\t -gMin - output image minimum intensity.\n\n";
}

template<typename T>
bool InputProcessor::readParam(const std::string &arg, const std::string &prefix, std::optional<T> &result,
                               const std::string &errorMsg) {
    try {
        std::string valueStr = arg.substr(prefix.length());
        if (valueStr.empty()) {
            throw std::invalid_argument("Missing value after prefix");
        }
        if constexpr (std::is_same_v<T, int>) {
            result = std::stoi(valueStr);
        } else if constexpr (std::is_same_v<T, float>) {
            result = std::stof(valueStr);
        } else if constexpr (std::is_same_v<T, std::string>) {
            result = valueStr;
        } else {
            throw std::invalid_argument("Unsupported type for readParam");
        }
        return true;
    } catch (const std::exception &e) {
        std::cerr << errorMsg << ": " << e.what() << std::endl;
        return false;
    }
}

bool InputProcessor::readStringParam(const std::string &arg, const std::string &prefix, std::string &result,
                                     const std::string &errorMsg) {
    try {
        std::string valueStr = arg.substr(prefix.length());
        if (valueStr.empty()) {
            throw std::invalid_argument("Missing value after prefix");
        }
        result = valueStr;
        return true;
    } catch (const std::exception &e) {
        std::cerr << errorMsg << ": " << e.what() << std::endl;
        return false;
    }
}


void InputProcessor::processInput() {
    for (int i = 1; i < argc; i++) {
        std::string commandStr = argv[i];
        CommandType command = CommandType::UNKNOWN;

        // Find the command type in the map
        auto it = commandMap.find(commandStr);
        if (it != commandMap.end()) {
            command = it->second;
        }

        switch (command) {
            case CommandType::HELP:
                printCommands();
                return;
            case CommandType::OUTPUT:
                if (++i < argc)
                    readStringParam(argv[i], "-fileName=", outputFileName, "Invalid file name format.");
                break;
            case CommandType::GRAYSCALE:
                imreadMode = cv::IMREAD_GRAYSCALE;
                break;
            case CommandType::BRIGHTNESS:
                if (++i < argc)
                    readParam(argv[i], "-val=", brightnessModVal, "Brightness value must be an integer.");
                break;
            case CommandType::CONTRAST_LINEAR:
                if (++i < argc)
                    readParam(argv[i], "-val=", contrastLinearModVal, "Contrast value must be an integer.");
                break;
            case CommandType::CONTRAST_GAMMA:
                if (++i < argc)
                    readParam(argv[i], "-val=", contrastGammaModVal, "Gamma value must be a float.");
                break;
            case CommandType::NEGATIVE:
                isNegative = true;
                break;

            case CommandType::HORIZONTAL_FLIP:
                isHorizontalFlip = true;
                break;

            case CommandType::VERTICAL_FLIP:
                isVerticalFlip = true;
                break;

            case CommandType::DIAGONAL_FLIP:
                isDiagonalFlip = true;
                break;

            case CommandType::SHRINK:
                if (++i < argc)
                    readParam(argv[i], "-val=", shrinkModVal,
                              "Shrink scale factor must be a float between 0 and 1.");
                break;

            case CommandType::ENLARGE:
                if (++i < argc)
                    readParam(argv[i], "-val=", enlargeModVal,
                              "Enlarge scale factor must be a float greater than 1.");
                break;

            case CommandType::MIDPOINT_FILTER:
                if (++i < argc)
                    readParam(argv[i], "-val=", midpointKernelSize,
                              "Midpoint filter kernel size must be an integer.");
                break;

            case CommandType::ARITHMETIC_MEAN_FILTER:
                if (++i < argc)
                    readParam(argv[i], "-val=", arithmeticMeanKernelSize,
                              "Arithmetic mean filter kernel size must be an integer.");
                break;

            case CommandType::NO_NOISE_IMAGE:
                if (++i < argc)
                    readStringParam(argv[i], "-fileName=", noNoiseImage,
                                    "Invalid format for noise-free image file name.");
                isNoNoise = true;
                break;

            case CommandType::MSE:
                isMeanSquareError = true;
                break;

            case CommandType::PMSE:
                isPeakMeanSquareError = true;
                break;

            case CommandType::SNR:
                isSignalToNoise = true;
                break;

            case CommandType::PSNR:
                isPeakSignalToNoise = true;
                break;

            case CommandType::MAXIMUM_DIFFERENCE:
                isMaximumDifference = true;
                break;

            case CommandType::HISTOGRAM:
                if (++i < argc) {
                    readParam(argv[i], "-val=", histogramChannel, "Histogram channel must be an integer (0, 1, or 2).");
                }
                break;

            case CommandType::HISTOGRAM_UNIFORM:
                if (++i < argc) {
                    readParam(argv[i], "-gMax=", histogramUniformGMax, "Histogram gMax must be an integer.");
                }
                if (++i < argc) {
                    readParam(argv[i], "-gMin=", histogramUniformGMin, "Histogram gMin must be an integer.");
                }
                break;

            case CommandType::UNKNOWN:
            default:
                if (i > 1) {
                    std::cerr << "Unknown command: " << commandStr << std::endl;
                }
                break;
        }
    }
}

void InputProcessor::applyImageTransformations(cv::Mat &image,
                                               std::unique_ptr<ImageProcessor> &imageProcessor)
const {
    // Apply brightness modification if set
    if (brightnessModVal.has_value()) {
        image = imageProcessor->modifyBrightness(image, brightnessModVal.value());
    }

    // Apply linear contrast modification if set
    if (contrastLinearModVal.has_value()) {
        image = imageProcessor->modifyContrastLinear(image, contrastLinearModVal.value());
    }

    // Apply gamma contrast modification if set
    if (contrastGammaModVal.has_value()) {
        image = imageProcessor->modifyContrastGamma(image, contrastGammaModVal.value());
    }

    // Apply negative transformation
    if (isNegative) {
        image = imageProcessor->negative(image);
    }

    // Apply horizontal flip if requested
    if (isHorizontalFlip) {
        image = imageProcessor->flipHorizontally(image);
    }

    // Apply vertical flip if requested
    if (isVerticalFlip) {
        image = imageProcessor->flipVertically(image);
    }

    // Apply diagonal flip if requested
    if (isDiagonalFlip) {
        image = imageProcessor->flipDiagonally(image);
    }

    // Apply a shrink transformation if set
    if (shrinkModVal.has_value()) {
        image = imageProcessor->resize(image, shrinkModVal.value());
    }

    // Apply an enlarge transformation if set
    if (enlargeModVal.has_value()) {
        image = imageProcessor->resize(image, enlargeModVal.value());
    }

    // Apply midpoint filter if kernel size is set
    if (midpointKernelSize.has_value()) {
        image = imageProcessor->midpointFilter(image, midpointKernelSize.value());
    }

    // Apply arithmetic mean filter if kernel size is set
    if (arithmeticMeanKernelSize.has_value()) {
        image = imageProcessor->arithmeticMeanFilter(image, arithmeticMeanKernelSize.value());
    }

    if (histogramUniformGMax.has_value() && histogramUniformGMin.has_value()) {
        image = imageProcessor->histogramUniform(image, histogramUniformGMax.value(), histogramUniformGMin.value());
    }
}

void InputProcessor::calculateAndSaveImageStatistics(
    const cv::Mat &compareImage,
    const cv::Mat &originalImage,
    const cv::Mat &newImage,
    std::unique_ptr<ImageProcessor> &imageProcessor
) const {
    std::stringstream ss;
    ss << "Image stats:\n";
    if (isMeanSquareError)
        ss << imageProcessor->meanSquareError(
            compareImage, originalImage, newImage);
    if (isPeakMeanSquareError)
        ss << imageProcessor->peakMeanSquareError(
            compareImage, originalImage, newImage);
    if (isSignalToNoise)
        ss << imageProcessor->signalToNoiseRatio(
            compareImage, originalImage, newImage);
    if (isPeakSignalToNoise)
        ss << imageProcessor->peakSignalToNoiseRatio(
            compareImage, originalImage, newImage);
    if (isMaximumDifference)
        ss << imageProcessor->maximumDifference(
            compareImage, originalImage, newImage);
    std::ofstream statsFile;
    statsFile.open(
        OUTPUT_DIR_NAME + "/" + outputFileName.substr(0, outputFileName.length() - 4) + ".txt");
    statsFile << ss.rdbuf();
    statsFile.close();
}


void InputProcessor::processImage() const {
    cv::Mat originalImage = imread(argv[INPUT_IMAGE_POS], imreadMode);
    if (!originalImage.data) {
        std::cout << "No image data \n";
        return;
    }
    std::unique_ptr<ImageProcessor> imageProcessor;
    if (imreadMode == cv::IMREAD_COLOR) {
        imageProcessor = std::make_unique<RGBImageProcessor>();
    } else {
        imageProcessor = std::make_unique<GrayscaleImageProcessor>();
    }

    cv::Mat newImage;
    originalImage.copyTo(newImage);
    applyImageTransformations(newImage, imageProcessor);
    if (histogramChannel) {
        std::string histogramFileName = outputFileName.substr(0, outputFileName.length() - 4) +
                                        "_histogram.png";
        saveImage(imageProcessor->histogram(newImage, histogramChannel.value()), histogramFileName);
    }
    saveImage(newImage, this->outputFileName);

    if (isNoNoise) {
        cv::Mat compareImage = imread(noNoiseImage, imreadMode);
        calculateAndSaveImageStatistics(compareImage, originalImage, newImage, imageProcessor);
    }
}


void InputProcessor::saveImage(cv::Mat image, std::string outputFileName) const {
    if (!std::filesystem::is_directory(OUTPUT_DIR_NAME) || !
        std::filesystem::exists(OUTPUT_DIR_NAME)) {
        std::filesystem::create_directory(OUTPUT_DIR_NAME);
    }
    imwrite(OUTPUT_DIR_NAME + "/" + outputFileName, image);
}
