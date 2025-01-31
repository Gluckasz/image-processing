//
// Created by gluckasz on 11/24/24.
//

#include <gtest/gtest.h>
#include "../include/GrayscaleImageProcessor.h"

class GrayscaleImageProcessorTest : public testing::Test {
protected:
    GrayscaleImageProcessorTest() {
        grayscaleImageProcessor = std::make_unique<GrayscaleImageProcessor>();
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC1);
        largerBlackImageGrayscale = cv::Mat::zeros(cv::Size(3, 3), CV_8UC1);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
    }

    std::unique_ptr<GrayscaleImageProcessor> grayscaleImageProcessor;
    cv::Mat blackImageGrayscale;
    cv::Mat largerBlackImageGrayscale;
    cv::Mat whiteImageGrayscale;

    const uchar imageWidth= 2;
    const uchar imageHeight = 2;

    const int brightnessModVal = 10;
};

TEST_F(GrayscaleImageProcessorTest, ModifyBrightnessTestNormalCases) {
    cv::Mat brighterImage = grayscaleImageProcessor->modifyBrightness(blackImageGrayscale, brightnessModVal);
    cv::Mat darkerImage = grayscaleImageProcessor->modifyBrightness(whiteImageGrayscale, -brightnessModVal);
    ASSERT_FALSE(brighterImage.empty()) << "The brighterImage should not be empty.";
    ASSERT_FALSE(darkerImage.empty()) << "The darkerImage should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(row, col);
            int modifiedPixel = brighterImage.at<uchar>(row, col);
            EXPECT_EQ(originalPixel + brightnessModVal, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";

            originalPixel = whiteImageGrayscale.at<uchar>(row, col);
            modifiedPixel = darkerImage.at<uchar>(row, col);
            EXPECT_EQ(originalPixel - brightnessModVal, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, ModifyBrightnessTestEdgeCases) {
    cv::Mat brighterImage = grayscaleImageProcessor->modifyBrightness(blackImageGrayscale, -brightnessModVal);
    cv::Mat darkerImage = grayscaleImageProcessor->modifyBrightness(whiteImageGrayscale, brightnessModVal);
    ASSERT_FALSE(brighterImage.empty()) << "The brighterImage should not be empty.";
    ASSERT_FALSE(darkerImage.empty()) << "The darkerImage should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(row, col);
            int modifiedPixel = brighterImage.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";

            originalPixel = whiteImageGrayscale.at<uchar>(row, col);
            modifiedPixel = darkerImage.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, ModifyContrastLinearTestNormalCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int contrastLinearModVal = 10;

    blackImageGrayscale.at<uchar>(0, 0) = pixel1;
    blackImageGrayscale.at<uchar>(1, 0) = pixel2;
    blackImageGrayscale.at<uchar>(0, 1) = pixel3;
    blackImageGrayscale.at<uchar>(1, 1) = pixel4;

    cv::Mat modifiedMoreContrastImage = grayscaleImageProcessor->modifyContrastLinear(blackImageGrayscale, contrastLinearModVal);
    ASSERT_FALSE(modifiedMoreContrastImage.empty()) << "The modifiedMoreContrastImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 0), 16);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 1), 33);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 1), 50);

    cv::Mat modifiedLessContrastImage = grayscaleImageProcessor->modifyContrastLinear(blackImageGrayscale, -contrastLinearModVal);
    ASSERT_FALSE(modifiedLessContrastImage.empty()) << "The modifiedLessContrastImage should not be empty.";

    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 0), 20);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 0), 23);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 1), 26);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 1), 30);
}

TEST_F(GrayscaleImageProcessorTest, ModifyContrastLinearTestEdgeCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int contrastLinearModVal = 20;

    blackImageGrayscale.at<uchar>(0, 0) = pixel1;
    blackImageGrayscale.at<uchar>(1, 0) = pixel2;
    blackImageGrayscale.at<uchar>(0, 1) = pixel3;
    blackImageGrayscale.at<uchar>(1, 1) = pixel4;

    cv::Mat modifiedMoreContrastImage = grayscaleImageProcessor->modifyContrastLinear(blackImageGrayscale, contrastLinearModVal);
    ASSERT_FALSE(modifiedMoreContrastImage.empty()) << "The modifiedMoreContrastImage should not be empty.";

    // Calculated values for contrastLinearModVal = 20
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 0), 13);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 1), 36);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 1), 60);

    cv::Mat modifiedLessContrastImage = grayscaleImageProcessor->modifyContrastLinear(blackImageGrayscale, -contrastLinearModVal);
    ASSERT_FALSE(modifiedLessContrastImage.empty()) << "The modifiedLessContrastImage should not be empty.";

    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 0), 30);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 0), 30);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 1), 30);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 1), 30);
}

TEST_F(GrayscaleImageProcessorTest, ModifyContrastGammaTestNormalCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int highGamma = 2;
    constexpr float lowGamma = 0.5f;

    blackImageGrayscale.at<uchar>(0, 0) = pixel1;
    blackImageGrayscale.at<uchar>(1, 0) = pixel2;
    blackImageGrayscale.at<uchar>(0, 1) = pixel3;
    blackImageGrayscale.at<uchar>(1, 1) = pixel4;

    cv::Mat highGammaImage = grayscaleImageProcessor->modifyContrastGamma(blackImageGrayscale, highGamma);
    ASSERT_FALSE(highGammaImage.empty()) << "The highGammaImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    EXPECT_EQ(highGammaImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 0), 2);
    EXPECT_EQ(highGammaImage.at<uchar>(0, 1), 4);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 1), 6);

    cv::Mat lowGammaImage = grayscaleImageProcessor->modifyContrastGamma(blackImageGrayscale, lowGamma);
    ASSERT_FALSE(lowGammaImage.empty()) << "The lowGammaImage should not be empty.";

    EXPECT_EQ(lowGammaImage.at<uchar>(0, 0), 50);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 0), 71);
    EXPECT_EQ(lowGammaImage.at<uchar>(0, 1), 87);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 1), 101);
}

TEST_F(GrayscaleImageProcessorTest, ModifyContrastGammaTestEdgeCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int highGamma = 10000;
    constexpr float lowGamma = 0.0001f;

    blackImageGrayscale.at<uchar>(0, 0) = pixel1;
    blackImageGrayscale.at<uchar>(1, 0) = pixel2;
    blackImageGrayscale.at<uchar>(0, 1) = pixel3;
    blackImageGrayscale.at<uchar>(1, 1) = pixel4;

    cv::Mat highGammaImage = grayscaleImageProcessor->modifyContrastGamma(blackImageGrayscale, highGamma);
    ASSERT_FALSE(highGammaImage.empty()) << "The highGammaImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    EXPECT_EQ(highGammaImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 0), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(0, 1), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 1), 0);

    cv::Mat lowGammaImage = grayscaleImageProcessor->modifyContrastGamma(blackImageGrayscale, lowGamma);
    ASSERT_FALSE(lowGammaImage.empty()) << "The lowGammaImage should not be empty.";

    EXPECT_EQ(lowGammaImage.at<uchar>(0, 0), 255);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 0), 255);
    EXPECT_EQ(lowGammaImage.at<uchar>(0, 1), 255);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 1), 255);
}

TEST_F(GrayscaleImageProcessorTest, NegativeTest) {
    cv::Mat brighterImage = grayscaleImageProcessor->negative(blackImageGrayscale);
    cv::Mat darkerImage = grayscaleImageProcessor->negative(whiteImageGrayscale);
    ASSERT_FALSE(brighterImage.empty()) << "The brighterImage should not be empty.";
    ASSERT_FALSE(darkerImage.empty()) << "The darkerImage should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(row, col);
            int modifiedPixel = brighterImage.at<uchar>(row, col);
            EXPECT_EQ(255 - originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";

            originalPixel = whiteImageGrayscale.at<uchar>(row, col);
            modifiedPixel = darkerImage.at<uchar>(row, col);
            EXPECT_EQ(255 - originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, HorizontalFlipTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    cv::Mat imageAfterFlip = grayscaleImageProcessor->flipHorizontally(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(row, blackImageGrayscale.cols - 1 - col);
            int modifiedPixel = imageAfterFlip.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, VerticalFlipTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    cv::Mat imageAfterFlip = grayscaleImageProcessor->flipVertically(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(blackImageGrayscale.rows - 1 - row, col);
            int modifiedPixel = imageAfterFlip.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, DiagonalFlipTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    cv::Mat imageAfterFlip = grayscaleImageProcessor->flipDiagonally(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(blackImageGrayscale.rows - 1 - row, blackImageGrayscale.cols - 1 - col);
            int modifiedPixel = imageAfterFlip.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, ResizeTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    float factor = 2;

    cv::Mat imageAfterEnlargement = grayscaleImageProcessor->resize(blackImageGrayscale, factor);
    cv::Mat imageAfterShrinking = grayscaleImageProcessor->resize(blackImageGrayscale, 1 / factor);
    ASSERT_FALSE(imageAfterEnlargement.empty()) << "The imageAfterEnlargement should not be empty.";
    ASSERT_FALSE(imageAfterShrinking.empty()) << "The imageAfterShrinking should not be empty.";
    ASSERT_EQ(imageAfterEnlargement.rows, 4);
    ASSERT_EQ(imageAfterEnlargement.cols, 4);
    ASSERT_EQ(imageAfterShrinking.rows, 1);
    ASSERT_EQ(imageAfterShrinking.cols, 1);

    int originalPixel = blackImageGrayscale.at<uchar>(0, 0);
    int modifiedPixel = imageAfterShrinking.at<uchar>(0, 0);
    EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << 0 << ", " << 0 << ")";

    for (int row = 0; row < imageAfterEnlargement.rows; row++) {
        for (int col = 0; col < imageAfterEnlargement.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(row / factor, col / factor);
            int modifiedPixel = imageAfterEnlargement.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, MidpointFilterTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    cv::Mat imageAfterModification = grayscaleImageProcessor->midpointFilter(blackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    int expectedValue = (10 + 0) / 2;
    int modifiedPixel = imageAfterModification.at<uchar>(0, 0);
    EXPECT_EQ(expectedValue, modifiedPixel)
        << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
}

TEST_F(GrayscaleImageProcessorTest, ArithmeticMeanFilterTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 10;
    cv::Mat imageAfterModification = grayscaleImageProcessor->arithmeticMeanFilter(blackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    int expectedValue = (10 + 10 + 0 + 0) / 4;
    int modifiedPixel = imageAfterModification.at<uchar>(0, 0);
    EXPECT_EQ(expectedValue, modifiedPixel)
        << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
}

TEST_F(GrayscaleImageProcessorTest, MeanSquareErrorTest) {
    double mse = grayscaleImageProcessor->meanSquareError(blackImageGrayscale, whiteImageGrayscale);

    double expectedValue = pow(255, 2);
    EXPECT_EQ(expectedValue, mse);
}

TEST_F(GrayscaleImageProcessorTest, PeakMeanSquareErrorTest) {
    double pmse = grayscaleImageProcessor->peakMeanSquareError(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 1;
    EXPECT_EQ(expectedValue, pmse);
}

TEST_F(GrayscaleImageProcessorTest, SignalToNoiseRatioTest) {
    double snr = grayscaleImageProcessor->signalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, snr);
}

TEST_F(GrayscaleImageProcessorTest, PeakSignalToNoiseRatioTest) {
    double psnr = grayscaleImageProcessor->peakSignalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, psnr);
}

TEST_F(GrayscaleImageProcessorTest, MaximumDifferenceTest) {
    double md = grayscaleImageProcessor->maximumDifference(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 255;
    EXPECT_EQ(expectedValue, md);
}

TEST_F(GrayscaleImageProcessorTest, ComputeHistogramTest) {
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(whiteImageGrayscale, 0 , temp);
    ASSERT_EQ(histogram[UCHAR_MAX], 4);
    for (int i = 0; i < UCHAR_MAX; i++) {
        EXPECT_EQ(histogram[i], 0);
    }
}

TEST_F(GrayscaleImageProcessorTest, HistogramTest) {
    cv::Mat histogram = grayscaleImageProcessor->histogram(whiteImageGrayscale, 0);
    ASSERT_EQ(4, histogram.rows);
    ASSERT_EQ(2056 , histogram.cols);
    for (int y = 255 * 8; y < 2048; y++) {
        for (int x = 0; x < histogram.rows; x++) {
            EXPECT_EQ(histogram.at<uchar>(x, y), 255)
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
    for (int y = 0; y < 255 * 8; y++) {
        for (int x = 0; x < histogram.rows; x++) {
            EXPECT_EQ(histogram.at<uchar>(x, y), 0)
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
    for (int y = 2048; y < histogram.cols; y++) {
        for (int x = 0; x < histogram.rows; x++) {
            EXPECT_EQ(histogram.at<uchar>(x, y), 0)
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, HistogramUniformTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    blackImageGrayscale.at<uchar>(0, 1) = 1;
    blackImageGrayscale.at<uchar>(1, 0) = 254;
    blackImageGrayscale.at<uchar>(1, 1) = 254;
    cv::Mat imageAfterModification = grayscaleImageProcessor->histogramUniform(blackImageGrayscale, 255, 0);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    EXPECT_EQ(128, imageAfterModification.at<uchar>(0, 0));
    EXPECT_EQ(128, imageAfterModification.at<uchar>(0, 1));
    EXPECT_EQ(255, imageAfterModification.at<uchar>(1, 0));
    EXPECT_EQ(255, imageAfterModification.at<uchar>(1, 1));
}

TEST_F(GrayscaleImageProcessorTest, MeanTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double mean = grayscaleImageProcessor->mean(histogram);

    EXPECT_EQ(25, mean);
}

TEST_F(GrayscaleImageProcessorTest, VarianceTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double variance = grayscaleImageProcessor->variance(histogram);

    EXPECT_EQ(125, variance);
}

TEST_F(GrayscaleImageProcessorTest, StandardDeviationTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double std = grayscaleImageProcessor->standardDeviation(histogram);

    EXPECT_EQ(sqrt(125), std);
}

TEST_F(GrayscaleImageProcessorTest, Variation1Test) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double var1 = grayscaleImageProcessor->variation1(histogram);

    EXPECT_EQ(5, var1);
}

TEST_F(GrayscaleImageProcessorTest, AsymmetryTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double asym = grayscaleImageProcessor->asymmetry(histogram);

    EXPECT_EQ(0, asym);
}

TEST_F(GrayscaleImageProcessorTest, FlatteningTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double flat = grayscaleImageProcessor->flattening(histogram);

    EXPECT_EQ(-1.36, flat);
}

TEST_F(GrayscaleImageProcessorTest, Variation2Test) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double var2 = grayscaleImageProcessor->variation2(histogram);

    EXPECT_EQ(0.25, var2);
}

TEST_F(GrayscaleImageProcessorTest, EntropyTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = grayscaleImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double ent = grayscaleImageProcessor->entropy(histogram);

    EXPECT_EQ(2, ent);
}

TEST_F(GrayscaleImageProcessorTest, LaplacianFilterTest) {
    largerBlackImageGrayscale.at<uchar>(0, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 0) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 1) = 40;
    largerBlackImageGrayscale.at<uchar>(2, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 2) = 20;

    cv::Mat imageAfterModification = grayscaleImageProcessor->laplacianFilter(largerBlackImageGrayscale, 0);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            if (x == 1 && y == 1)
                EXPECT_EQ(80, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(40, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    imageAfterModification = grayscaleImageProcessor->laplacianFilter(largerBlackImageGrayscale, 1);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            if (x == 1 && y == 1)
                EXPECT_EQ(240, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(80, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    imageAfterModification = grayscaleImageProcessor->laplacianFilter(largerBlackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            if (x == 1 && y == 1)
                EXPECT_EQ(0, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(40, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, OptimizedLaplacianFilterTest) {
    largerBlackImageGrayscale.at<uchar>(0, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 0) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 1) = 40;
    largerBlackImageGrayscale.at<uchar>(2, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 2) = 20;

    cv::Mat imageAfterModification = grayscaleImageProcessor->optimizedLaplacianFilter(largerBlackImageGrayscale);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            if (x == 1 && y == 1)
                EXPECT_EQ(80, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(40, imageAfterModification.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(GrayscaleImageProcessorTest, RobertsOperator1Test) {
    largerBlackImageGrayscale.at<uchar>(0, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 0) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 1) = 40;
    largerBlackImageGrayscale.at<uchar>(2, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 2) = 20;

    cv::Mat imageAfterModification = grayscaleImageProcessor->robertsOperator1(largerBlackImageGrayscale);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    EXPECT_EQ(40, imageAfterModification.at<uchar>(0, 0));
    EXPECT_EQ(40, imageAfterModification.at<uchar>(1, 0));
    EXPECT_EQ(40, imageAfterModification.at<uchar>(0, 1));
    EXPECT_EQ(40, imageAfterModification.at<uchar>(1, 1));
    EXPECT_EQ(0, imageAfterModification.at<uchar>(0, 2));
    EXPECT_EQ(0, imageAfterModification.at<uchar>(2, 0));
    EXPECT_EQ(20, imageAfterModification.at<uchar>(2, 1));
    EXPECT_EQ(20, imageAfterModification.at<uchar>(1, 2));
    EXPECT_EQ(0, imageAfterModification.at<uchar>(2, 2));
}

TEST_F(GrayscaleImageProcessorTest, RegionGrowingTest) {
    cv::Mat image = cv::Mat::zeros(cv::Size(40, 40), CV_8UC1);
    image.at<uchar>(20, 19) = 99;
    image.at<uchar>(20, 20) = 100;
    image.at<uchar>(20, 21) = 255;

    for (int i = 0; i < 3; i++) {
        cv::Mat segmentationMasks = grayscaleImageProcessor->regionGrowing(image, i);
        for (int x = 0; x < image.rows; x++) {
            for (int y = 0; y < image.cols; y++) {
                if ((x == 20 && y == 20) || (x == 20 && y == 19)) {
                    EXPECT_EQ(2, segmentationMasks.at<uchar>(x, y));
                } else if (x == 20 && y == 21) {
                    EXPECT_EQ(0, segmentationMasks.at<uchar>(x, y));
                } else {
                    EXPECT_EQ(1, segmentationMasks.at<uchar>(x, y));
                }
            }
        }
    }
}
