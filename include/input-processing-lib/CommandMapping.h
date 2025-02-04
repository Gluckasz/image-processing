//
// Created by gluckasz on 2/3/25.
//

#ifndef COMMANDMAPPING_H
#define COMMANDMAPPING_H
#include <string>
#include <unordered_map>
/**
 * @brief Command type enumeration for argument parsing
 */
enum class CommandType {
    HELP,
    OUTPUT,
    GRAYSCALE,
    BRIGHTNESS,
    CONTRAST_LINEAR,
    CONTRAST_GAMMA,
    NEGATIVE,
    HORIZONTAL_FLIP,
    VERTICAL_FLIP,
    DIAGONAL_FLIP,
    RESIZE,
    MIDPOINT_FILTER,
    ARITHMETIC_MEAN_FILTER,
    COMPARE_IMAGES,
    HISTOGRAM,
    HISTOGRAM_UNIFORM,
    HISTOGRAM_STATS,
    LAPLACE,
    OPTIMIZED_LAPLACE,
    ROBERTS_OPERATOR,
    DILATION,
    EROSION,
    OPENING,
    CLOSING,
    HMT,
    REGION_GROWING,
    FOURIER_TRANSFORM,
    FAST_FOURIER_TRANSFORM,
    FFT_LOW_PASS,
    FFT_HIGH_PASS,
    FFT_BAND_PASS,
    FFT_BAND_CUT,
    FFT_HIGH_PASS_DIRECTION,
    FFT_PHASE_MODIFYING,
    UNKNOWN // For unrecognized commands
};

/**
 * @brief Maps command strings to CommandType enum values
 */
const std::unordered_map<std::string, CommandType> commandMap = {
    {"--help", CommandType::HELP},
    {"--output", CommandType::OUTPUT},
    {"--grayscale", CommandType::GRAYSCALE},
    {"--brightness", CommandType::BRIGHTNESS},
    {"--contrastLinear", CommandType::CONTRAST_LINEAR},
    {"--contrastGamma", CommandType::CONTRAST_GAMMA},
    {"--negative", CommandType::NEGATIVE},
    {"--hflip", CommandType::HORIZONTAL_FLIP},
    {"--vflip", CommandType::VERTICAL_FLIP},
    {"--dflip", CommandType::DIAGONAL_FLIP},
    {"--shrink", CommandType::RESIZE},
    {"--mid", CommandType::MIDPOINT_FILTER},
    {"--amean", CommandType::ARITHMETIC_MEAN_FILTER},
    {"--compareImages", CommandType::COMPARE_IMAGES},
    {"--histogram", CommandType::HISTOGRAM},
    {"--huniform", CommandType::HISTOGRAM_UNIFORM},
    {"--histogramStats", CommandType::HISTOGRAM_STATS},
    {"--slaplace", CommandType::LAPLACE},
    {"--oslaplace", CommandType::OPTIMIZED_LAPLACE},
    {"--orobertsi", CommandType::ROBERTS_OPERATOR},
    {"--dilation", CommandType::DILATION},
    {"--erosion", CommandType::EROSION},
    {"--opening", CommandType::OPENING},
    {"--closing", CommandType::CLOSING},
    {"--hmt", CommandType::HMT},
    {"--regionGrowing", CommandType::REGION_GROWING},
    {"--fourierTransform", CommandType::FOURIER_TRANSFORM},
    {"--fastFourierTransform", CommandType::FAST_FOURIER_TRANSFORM},
    {"--fftLowPass", CommandType::FFT_LOW_PASS},
    {"--fftHighPass", CommandType::FFT_HIGH_PASS},
    {"--fftBandPass", CommandType::FFT_BAND_PASS},
    {"--fftBandCut", CommandType::FFT_BAND_CUT},
    {"--fftHighPassDirection", CommandType::FFT_HIGH_PASS_DIRECTION},
    {"--fftPhaseModifying", CommandType::FFT_PHASE_MODIFYING},
};

/**
 * @brief Maps CommandType enum values to their string representations
 */
const std::unordered_map<CommandType, std::string> commandToStringMap = {
    {CommandType::HELP, "--help"},
    {CommandType::OUTPUT, "--output"},
    {CommandType::GRAYSCALE, "--grayscale"},
    {CommandType::BRIGHTNESS, "--brightness"},
    {CommandType::CONTRAST_LINEAR, "--contrastLinear"},
    {CommandType::CONTRAST_GAMMA, "--contrastGamma"},
    {CommandType::NEGATIVE, "--negative"},
    {CommandType::HORIZONTAL_FLIP, "--hflip"},
    {CommandType::VERTICAL_FLIP, "--vflip"},
    {CommandType::DIAGONAL_FLIP, "--dflip"},
    {CommandType::RESIZE, "--shrink"},
    {CommandType::MIDPOINT_FILTER, "--mid"},
    {CommandType::ARITHMETIC_MEAN_FILTER, "--amean"},
    {CommandType::COMPARE_IMAGES, "--compareImages"},
    {CommandType::HISTOGRAM, "--histogram"},
    {CommandType::HISTOGRAM_UNIFORM, "--huniform"},
    {CommandType::HISTOGRAM_STATS, "--histogramStats"},
    {CommandType::LAPLACE, "--slaplace"},
    {CommandType::OPTIMIZED_LAPLACE, "--oslaplace"},
    {CommandType::ROBERTS_OPERATOR, "--orobertsi"},
    {CommandType::DILATION, "--dilation"},
    {CommandType::EROSION, "--erosion"},
    {CommandType::OPENING, "--opening"},
    {CommandType::CLOSING, "--closing"},
    {CommandType::HMT, "--hmt"},
    {CommandType::REGION_GROWING, "--regionGrowing"},
    {CommandType::FOURIER_TRANSFORM, "--fourierTransform"},
    {CommandType::FAST_FOURIER_TRANSFORM, "--fastFourierTransform"},
    {CommandType::FFT_LOW_PASS, "--fftLowPass"},
    {CommandType::FFT_HIGH_PASS, "--fftHighPass"},
    {CommandType::FFT_BAND_PASS, "--fftBandPass"},
    {CommandType::FFT_BAND_CUT, "--fftBandCut"},
    {CommandType::FFT_HIGH_PASS_DIRECTION, "--fftHighPassDirection"},
    {CommandType::FFT_PHASE_MODIFYING, "--fftPhaseModifying"},
};
#endif //COMMANDMAPPING_H
