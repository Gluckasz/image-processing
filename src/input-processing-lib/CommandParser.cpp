//
// Created by gluckasz on 2/3/25.
//

#include "../include/input-processing-lib/CommandParser.h"

#include "input-processing-lib/CommandMapping.h"

template<typename T>
bool CommandParser::readParam(const std::string &arg, const std::string &prefix, std::optional<T> &result,
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

CommandOptions CommandParser::parse(const int argc, char **argv) {
    CommandOptions commandOptions;
    for (int i = 1; i < argc; i++) {
        std::string commandStr = argv[i];
        auto command = CommandType::UNKNOWN;

        // Find the command type in the map
        if (auto it = commandMap.find(commandStr); it != commandMap.end()) {
            command = it->second;
        }

        switch (command) {
            case CommandType::HELP:
                commandOptions.showHelp = true;
                break;
            case CommandType::OUTPUT:
                if (++i < argc)
                    readParam(argv[i], "-fileName=", commandOptions.outputFileName, "Invalid file name format.");
                break;
            case CommandType::GRAYSCALE:
                commandOptions.imreadMode = cv::IMREAD_GRAYSCALE;
                break;
            case CommandType::BRIGHTNESS:
                if (++i < argc)
                    readParam(argv[i], "-val=", commandOptions.brightnessModVal,
                              "Brightness value must be an integer.");
                break;
            case CommandType::CONTRAST_LINEAR:
                if (++i < argc)
                    readParam(argv[i], "-val=", commandOptions.contrastLinearModVal,
                              "Contrast value must be an integer.");
                break;
            case CommandType::CONTRAST_GAMMA:
                if (++i < argc)
                    readParam(argv[i], "-val=", commandOptions.contrastGammaModVal, "Gamma value must be a float.");
                break;
            case CommandType::NEGATIVE:
                commandOptions.isNegative = true;
                break;

            case CommandType::HORIZONTAL_FLIP:
                commandOptions.isHorizontalFlip = true;
                break;

            case CommandType::VERTICAL_FLIP:
                commandOptions.isVerticalFlip = true;
                break;

            case CommandType::DIAGONAL_FLIP:
                commandOptions.isDiagonalFlip = true;
                break;

            case CommandType::RESIZE:
                if (++i < argc)
                    readParam(argv[i], "-val=", commandOptions.resizeModVal,
                              "Shrink scale factor must be a float between 0 and 1.");
                break;

            case CommandType::MIDPOINT_FILTER:
                if (++i < argc)
                    readParam(argv[i], "-val=", commandOptions.midpointKernelSize,
                              "Midpoint filter kernel size must be an integer.");
                break;

            case CommandType::ARITHMETIC_MEAN_FILTER:
                if (++i < argc)
                    readParam(argv[i], "-val=", commandOptions.arithmeticMeanKernelSize,
                              "Arithmetic mean filter kernel size must be an integer.");
                break;

            case CommandType::COMPARE_IMAGES:
                commandOptions.isCompareImages = true;
                break;

            case CommandType::HISTOGRAM:
                if (++i < argc) {
                    readParam(argv[i], "-val=", commandOptions.histogramChannel,
                              "Histogram channel must be an integer (0, 1, or 2).");
                }
                break;

            case CommandType::HISTOGRAM_UNIFORM:

                try {
                    if (++i < argc) {
                        readParam(argv[i], "-gMax=", commandOptions.histogramUniformGMax,
                                  "Histogram gMax must be an integer.");
                    }
                    if (++i < argc) {
                        readParam(argv[i], "-gMin=", commandOptions.histogramUniformGMin,
                                  "Histogram gMin must be an integer.");
                    }
                    if (commandOptions.histogramUniformGMax < commandOptions.histogramUniformGMin) {
                        throw std::invalid_argument("gMax has to be greater than gMin");
                    }
                } catch ([[maybe_unused]] const std::exception &e) {
                    throw;
                }

                break;

            case CommandType::HISTOGRAM_STATS:
                commandOptions.isHistogramStats = true;
                break;

            case CommandType::LAPLACE:
                if (++i < argc) {
                    readParam(argv[i], "-val=", commandOptions.laplaceMask,
                              "Laplace mask number must be an integer (0, 1, or 2).");
                }
                break;

            case CommandType::OPTIMIZED_LAPLACE:
                commandOptions.isOptimizedLaplacian = true;
                break;

            case CommandType::ROBERTS_OPERATOR:
                commandOptions.isRobertsOperator = true;
                break;

            case CommandType::DILATION:
                if (++i < argc) {
                    readParam(argv[i], "-mask=", commandOptions.dilationMask,
                              "Dilation mask number must be an integer between 1 and 10.");
                }
                break;

            case CommandType::EROSION:
                if (++i < argc) {
                    readParam(argv[i], "-mask=", commandOptions.erosionMask,
                              "Erosion mask number must be an integer between 1 and 10.");
                }
                break;

            case CommandType::OPENING:
                if (++i < argc) {
                    readParam(argv[i], "-mask=", commandOptions.openingMask,
                              "Opening mask number must be an integer between 1 and 10.");
                }
                break;

            case CommandType::CLOSING:
                if (++i < argc) {
                    readParam(argv[i], "-mask=", commandOptions.closingMask,
                              "Closing mask number must be an integer between 1 and 10.");
                }
                break;

            case CommandType::HMT:
                if (++i < argc) {
                    readParam(argv[i], "-mask=", commandOptions.hmtMask,
                              "HMT mask number must be an integer between 1 and 10.");
                }
                break;

            case CommandType::REGION_GROWING:
                if (++i < argc) {
                    readParam(argv[i], "-criterion=", commandOptions.regionGrowing,
                              "Criterion must be an integer between 0 and 2.");
                }
                break;

            case CommandType::FOURIER_TRANSFORM:
                commandOptions.isFourierTransform = true;
                break;

            case CommandType::FAST_FOURIER_TRANSFORM:
                commandOptions.isFastFourierTransform = true;
                break;

            case CommandType::FFT_LOW_PASS:
                commandOptions.isFastFourierTransform = true;
                if (++i < argc) {
                    readParam(argv[i], "-bandValue=", commandOptions.lowPassBandSize, "Band value must be an integer.");
                }
                break;

            case CommandType::FFT_HIGH_PASS:
                commandOptions.isFastFourierTransform = true;
                if (++i < argc) {
                    readParam(argv[i], "-bandValue=", commandOptions.highPassBandSize,
                              "Band value must be an integer.");
                }
                break;

            case CommandType::FFT_BAND_PASS:
                commandOptions.isFastFourierTransform = true;
                if (++i < argc) {
                    readParam(argv[i], "-lowCut=", commandOptions.lowCut, "Band value must be an integer.");
                }
                if (++i < argc) {
                    readParam(argv[i], "-highCut=", commandOptions.highCut, "Band value must be an integer.");
                }
                break;

            case CommandType::FFT_BAND_CUT:
                commandOptions.isFastFourierTransform = true;
                if (++i < argc) {
                    readParam(argv[i], "-lowPass=", commandOptions.lowPass, "Band value must be an integer.");
                }
                if (++i < argc) {
                    readParam(argv[i], "-highPass=", commandOptions.highPass, "Band value must be an integer.");
                }
                break;

            case CommandType::FFT_PHASE_MODIFYING:
                commandOptions.isFastFourierTransform = true;
                if (++i < argc) {
                    readParam(argv[i], "-k=", commandOptions.taskF6k, "K must be an integer.");
                }
                if (++i < argc) {
                    readParam(argv[i], "-l=", commandOptions.taskF6l, "K must be an integer.");
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

    return commandOptions;
}
