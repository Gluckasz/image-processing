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
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
    }

    std::unique_ptr<GrayscaleImageProcessor> grayscaleImageProcessor;
    cv::Mat blackImageGrayscale;
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
    for (int y = 255 * 8; y < histogram.cols; y++) {
        for (int x = 4; x < histogram.rows; x++) {
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
}