//
// Created by gluckasz on 11/24/24.
//

#include <gtest/gtest.h>
#include "../include/RGBImageProcessor.h"

class RGBImageProcessorTest : public testing::Test {
protected:
    RGBImageProcessorTest() {
        rgbImageProcessor = std::make_unique<RGBImageProcessor>();
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC3);
        largerBlackImageGrayscale = cv::Mat::zeros(cv::Size(3, 3), CV_8UC3);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC3, cv::Scalar(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX));
    }

    std::unique_ptr<RGBImageProcessor> rgbImageProcessor;
    cv::Mat blackImageGrayscale;
    cv::Mat largerBlackImageGrayscale;
    cv::Mat whiteImageGrayscale;

    const uchar imageWidth= 2;
    const uchar imageHeight = 2;

    const int brightnessModVal = 10;
};

TEST_F(RGBImageProcessorTest, ModifyBrightnessTestNormalCases) {
    cv::Mat brighterImage = rgbImageProcessor->modifyBrightness(blackImageGrayscale, brightnessModVal);
    cv::Mat darkerImage = rgbImageProcessor->modifyBrightness(whiteImageGrayscale, -brightnessModVal);
    ASSERT_FALSE(brighterImage.empty()) << "The brighterImage should not be empty.";
    ASSERT_FALSE(darkerImage.empty()) << "The darkerImage should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(row, col)[z];
                int modifiedPixel = brighterImage.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel + brightnessModVal, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";

                originalPixel = whiteImageGrayscale.at<cv::Vec3b>(row, col)[z];
                modifiedPixel = darkerImage.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel - brightnessModVal, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, ModifyBrightnessTestEdgeCases) {
    cv::Mat brighterImage = rgbImageProcessor->modifyBrightness(blackImageGrayscale, -brightnessModVal);
    cv::Mat darkerImage = rgbImageProcessor->modifyBrightness(whiteImageGrayscale, brightnessModVal);
    ASSERT_FALSE(brighterImage.empty()) << "The brighterImage should not be empty.";
    ASSERT_FALSE(darkerImage.empty()) << "The darkerImage should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(row, col)[z];
                int modifiedPixel = brighterImage.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";

                originalPixel = whiteImageGrayscale.at<cv::Vec3b>(row, col)[z];
                modifiedPixel = darkerImage.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, ModifyContrastLinearTestNormalCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int contrastLinearModVal = 10;

    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = pixel1;
        blackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = pixel2;
        blackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = pixel3;
        blackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = pixel4;
    }

    cv::Mat modifiedMoreContrastImage = rgbImageProcessor->modifyContrastLinear(blackImageGrayscale, contrastLinearModVal);
    ASSERT_FALSE(modifiedMoreContrastImage.empty()) << "The modifiedMoreContrastImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(0, 0)[z], 0);
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(1, 0)[z], 16);
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(0, 1)[z], 33);
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(1, 1)[z], 50);
    }

    cv::Mat modifiedLessContrastImage = rgbImageProcessor->modifyContrastLinear(blackImageGrayscale, -contrastLinearModVal);
    ASSERT_FALSE(modifiedLessContrastImage.empty()) << "The modifiedLessContrastImage should not be empty.";

    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(0, 0)[z], 20);
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(1, 0)[z], 23);
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(0, 1)[z], 26);
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(1, 1)[z], 30);
    }
}

TEST_F(RGBImageProcessorTest, ModifyContrastLinearTestEdgeCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int contrastLinearModVal = 20;

    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = pixel1;
        blackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = pixel2;
        blackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = pixel3;
        blackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = pixel4;
    }

    cv::Mat modifiedMoreContrastImage = rgbImageProcessor->modifyContrastLinear(blackImageGrayscale, contrastLinearModVal);
    ASSERT_FALSE(modifiedMoreContrastImage.empty()) << "The modifiedMoreContrastImage should not be empty.";

    // Calculated values for contrastLinearModVal = 20
    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(0, 0)[z], 0);
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(1, 0)[z], 13);
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(0, 1)[z], 36);
        EXPECT_EQ(modifiedMoreContrastImage.at<cv::Vec3b>(1, 1)[z], 60);
    }

    cv::Mat modifiedLessContrastImage = rgbImageProcessor->modifyContrastLinear(blackImageGrayscale, -contrastLinearModVal);
    ASSERT_FALSE(modifiedLessContrastImage.empty()) << "The modifiedLessContrastImage should not be empty.";

    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(0, 0)[z], 30);
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(1, 0)[z], 30);
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(0, 1)[z], 30);
        EXPECT_EQ(modifiedLessContrastImage.at<cv::Vec3b>(1, 1)[z], 30);
    }
}

TEST_F(RGBImageProcessorTest, ModifyContrastGammaTestNormalCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int highGamma = 2;
    constexpr float lowGamma = 0.5f;

    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = pixel1;
        blackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = pixel2;
        blackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = pixel3;
        blackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = pixel4;
    }

    cv::Mat highGammaImage = rgbImageProcessor->modifyContrastGamma(blackImageGrayscale, highGamma);
    ASSERT_FALSE(highGammaImage.empty()) << "The highGammaImage should not be empty.";

    for (int z = 0; z < 3; z++) {
        // Calculated values for contrastLinearModVal = 10
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(0, 0)[z], 0);
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(1, 0)[z], 2);
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(0, 1)[z], 4);
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(1, 1)[z], 6);
    }


    cv::Mat lowGammaImage = rgbImageProcessor->modifyContrastGamma(blackImageGrayscale, lowGamma);
    ASSERT_FALSE(lowGammaImage.empty()) << "The lowGammaImage should not be empty.";

    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(0, 0)[z], 50);
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(1, 0)[z], 71);
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(0, 1)[z], 87);
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(1, 1)[z], 101);
    }
}

TEST_F(RGBImageProcessorTest, ModifyContrastGammaTestEdgeCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int highGamma = 10000;
    constexpr float lowGamma = 0.0001f;

    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = pixel1;
        blackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = pixel2;
        blackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = pixel3;
        blackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = pixel4;
    }

    cv::Mat highGammaImage = rgbImageProcessor->modifyContrastGamma(blackImageGrayscale, highGamma);
    ASSERT_FALSE(highGammaImage.empty()) << "The highGammaImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(0, 0)[z], 0);
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(1, 0)[z], 0);
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(0, 1)[z], 0);
        EXPECT_EQ(highGammaImage.at<cv::Vec3b>(1, 1)[z], 0);
    }

    cv::Mat lowGammaImage = rgbImageProcessor->modifyContrastGamma(blackImageGrayscale, lowGamma);
    ASSERT_FALSE(lowGammaImage.empty()) << "The lowGammaImage should not be empty.";

    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(0, 0)[z], 255);
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(1, 0)[z], 255);
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(0, 1)[z], 255);
        EXPECT_EQ(lowGammaImage.at<cv::Vec3b>(1, 1)[z], 255);
    }
}

TEST_F(RGBImageProcessorTest, NegativeTest) {
    cv::Mat brighterImage = rgbImageProcessor->negative(blackImageGrayscale);
    cv::Mat darkerImage = rgbImageProcessor->negative(whiteImageGrayscale);
    ASSERT_FALSE(brighterImage.empty()) << "The brighterImage should not be empty.";
    ASSERT_FALSE(darkerImage.empty()) << "The darkerImage should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(row, col)[z];
                int modifiedPixel = brighterImage.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(255 - originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";

                originalPixel = whiteImageGrayscale.at<cv::Vec3b>(row, col)[z];
                modifiedPixel = darkerImage.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(255 - originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, HorizontalFlipTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 1;
    cv::Mat imageAfterFlip = rgbImageProcessor->flipHorizontally(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(row, blackImageGrayscale.cols - 1 - col)[z];
                int modifiedPixel = imageAfterFlip.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, VerticalFlipTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 1;
    cv::Mat imageAfterFlip = rgbImageProcessor->flipVertically(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(blackImageGrayscale.rows - 1 - row, col)[z];
                int modifiedPixel = imageAfterFlip.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, DiagonalFlipTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 1;
    cv::Mat imageAfterFlip = rgbImageProcessor->flipDiagonally(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(blackImageGrayscale.rows - 1 - row, blackImageGrayscale.cols - 1 - col)[z];
                int modifiedPixel = imageAfterFlip.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, ResizeTest) {
    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = 1;
    }
    float factor = 2;

    cv::Mat imageAfterEnlargement = rgbImageProcessor->resize(blackImageGrayscale, factor);
    cv::Mat imageAfterShrinking = rgbImageProcessor->resize(blackImageGrayscale, 1 / factor);
    ASSERT_FALSE(imageAfterEnlargement.empty()) << "The imageAfterEnlargement should not be empty.";
    ASSERT_FALSE(imageAfterShrinking.empty()) << "The imageAfterShrinking should not be empty.";
    ASSERT_EQ(imageAfterEnlargement.rows, 4);
    ASSERT_EQ(imageAfterEnlargement.cols, 4);
    ASSERT_EQ(imageAfterShrinking.rows, 1);
    ASSERT_EQ(imageAfterShrinking.cols, 1);

    for (int z = 0; z < 3; z++) {
        int originalPixel = blackImageGrayscale.at<cv::Vec3b>(0, 0)[z];
        int modifiedPixel = imageAfterShrinking.at<cv::Vec3b>(0, 0)[z];
        EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
    }

    for (int row = 0; row < imageAfterEnlargement.rows; row++) {
        for (int col = 0; col < imageAfterEnlargement.cols; col++) {
            for (int z = 0; z < 3; z++) {
                int originalPixel = blackImageGrayscale.at<cv::Vec3b>(row / factor, col / factor)[z];
                int modifiedPixel = imageAfterEnlargement.at<cv::Vec3b>(row, col)[z];
                EXPECT_EQ(originalPixel, modifiedPixel)
                    << "Mismatch at pixel (" << row << ", " << col << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, MidpointFilterTest) {
    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = 10;
    }
    cv::Mat imageAfterModification = rgbImageProcessor->midpointFilter(blackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    int expectedValue = (10 + 0) / 2;
    for (int z = 0; z < 3; z++) {
        int modifiedPixel = imageAfterModification.at<cv::Vec3b>(0, 0)[z];
        EXPECT_EQ(expectedValue, modifiedPixel)
            << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
    }
}

TEST_F(RGBImageProcessorTest, ArithmeticMeanFilterTest) {
    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = 10;
        blackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = 10;
    }
    cv::Mat imageAfterModification = rgbImageProcessor->arithmeticMeanFilter(blackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    int expectedValue = (10 + 10 + 0 + 0) / 4;
    for (int z = 0; z < 3; z++) {
        int modifiedPixel = imageAfterModification.at<cv::Vec3b>(0, 0)[z];
        EXPECT_EQ(expectedValue, modifiedPixel)
            << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
    }
}

TEST_F(RGBImageProcessorTest, MeanSquareErrorTest) {
    double mse = rgbImageProcessor->meanSquareError(blackImageGrayscale, whiteImageGrayscale);

    double expectedValue = pow(255, 2);
    EXPECT_EQ(expectedValue, mse);
}

TEST_F(RGBImageProcessorTest, PeakMeanSquareErrorTest) {
    double pmse = rgbImageProcessor->peakMeanSquareError(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 1;
    EXPECT_EQ(expectedValue, pmse);
}

TEST_F(RGBImageProcessorTest, SignalToNoiseRatioTest) {
    double snr = rgbImageProcessor->signalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, snr);
}

TEST_F(RGBImageProcessorTest, PeakSignalToNoiseRatioTest) {
    double psnr = rgbImageProcessor->peakSignalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, psnr);
}

TEST_F(RGBImageProcessorTest, MaximumDifferenceTest) {
    double md = rgbImageProcessor->maximumDifference(whiteImageGrayscale, blackImageGrayscale);

    double expectedValue = 255;
    EXPECT_EQ(expectedValue, md);
}

TEST_F(RGBImageProcessorTest, ComputeHistogramTest) {
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(whiteImageGrayscale, 0 , temp);
    ASSERT_EQ(histogram[UCHAR_MAX], 4);
    for (int i = 0; i < UCHAR_MAX; i++) {
        EXPECT_EQ(histogram[i], 0);
    }
}

TEST_F(RGBImageProcessorTest, HistogramTest) {
    for (int z = 0; z < 3; z++) {
        cv::Mat histogram = rgbImageProcessor->histogram(whiteImageGrayscale, z);
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
}

TEST_F(RGBImageProcessorTest, HistogramUniformTest) {
    for (int z = 0; z < 3; z++) {
        blackImageGrayscale.at<cv::Vec3b>(0, 0)[z] = 1;
        blackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = 1;
        blackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = 254;
        blackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = 254;
    }
    cv::Mat imageAfterModification = rgbImageProcessor->histogramUniform(blackImageGrayscale, 255, 0);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(128, imageAfterModification.at<cv::Vec3b>(0, 0)[z]);
        EXPECT_EQ(128, imageAfterModification.at<cv::Vec3b>(0, 1)[z]);
        EXPECT_EQ(255, imageAfterModification.at<cv::Vec3b>(1, 0)[z]);
        EXPECT_EQ(255, imageAfterModification.at<cv::Vec3b>(1, 1)[z]);
    }
}

TEST_F(RGBImageProcessorTest, MeanTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double mean = rgbImageProcessor->mean(histogram);

    EXPECT_EQ(25, mean);
}

TEST_F(RGBImageProcessorTest, VarianceTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double variance = rgbImageProcessor->variance(histogram);

    EXPECT_EQ(125, variance);
}

TEST_F(RGBImageProcessorTest, StandardDeviationTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double std = rgbImageProcessor->standardDeviation(histogram);

    EXPECT_EQ(sqrt(125), std);
}

TEST_F(RGBImageProcessorTest, Variation1Test) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double var1 = rgbImageProcessor->variation1(histogram);

    EXPECT_EQ(5, var1);
}

TEST_F(RGBImageProcessorTest, AsymmetryTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double asym = rgbImageProcessor->asymmetry(histogram);

    EXPECT_EQ(0, asym);
}

TEST_F(RGBImageProcessorTest, FlatteningTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double flat = rgbImageProcessor->flattening(histogram);

    EXPECT_EQ(-1.36, flat);
}

TEST_F(RGBImageProcessorTest, Variation2Test) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double var2 = rgbImageProcessor->variation2(histogram);

    EXPECT_EQ(0.25, var2);
}

TEST_F(RGBImageProcessorTest, EntropyTest) {
    blackImageGrayscale.at<cv::Vec3b>(0, 0) = 10;
    blackImageGrayscale.at<cv::Vec3b>(0, 1) = 20;
    blackImageGrayscale.at<cv::Vec3b>(1, 0) = 30;
    blackImageGrayscale.at<cv::Vec3b>(1, 1) = 40;
    uint temp = 0;
    std::array<uint, UCHAR_MAX + 1> histogram = rgbImageProcessor->computeHistogram(blackImageGrayscale, 0 , temp);
    double ent = rgbImageProcessor->entropy(histogram);

    EXPECT_EQ(2, ent);
}

TEST_F(RGBImageProcessorTest, LaplacianFilterTest) {
    for (int z = 0; z < 3; z++) {
        largerBlackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = 40;
        largerBlackImageGrayscale.at<cv::Vec3b>(2, 1)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 2)[z] = 20;
    }

    cv::Mat imageAfterModification = rgbImageProcessor->laplacianFilter(largerBlackImageGrayscale, 0);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (x == 1 && y == 1)
                    EXPECT_EQ(80, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
                else
                    EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }

    imageAfterModification = rgbImageProcessor->laplacianFilter(largerBlackImageGrayscale, 1);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (x == 1 && y == 1)
                    EXPECT_EQ(240, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
                else
                    EXPECT_EQ(80, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }

    imageAfterModification = rgbImageProcessor->laplacianFilter(largerBlackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (x == 1 && y == 1)
                    EXPECT_EQ(0, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
                else
                    EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, OptimizedLaplacianFilterTest) {
    for (int z = 0; z < 3; z++) {
        largerBlackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = 40;
        largerBlackImageGrayscale.at<cv::Vec3b>(2, 1)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 2)[z] = 20;
    }

    cv::Mat imageAfterModification = rgbImageProcessor->optimizedLaplacianFilter(largerBlackImageGrayscale);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int x = 0; x < imageAfterModification.rows; x++) {
        for (int y = 0; y < imageAfterModification.cols; y++) {
            for (int z = 0; z < 3; z++) {
                if (x == 1 && y == 1)
                    EXPECT_EQ(80, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
                else
                    EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(x, y)[z])
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }
}

TEST_F(RGBImageProcessorTest, RobertsOperator1Test) {
    for (int z = 0; z < 3; z++) {
        largerBlackImageGrayscale.at<cv::Vec3b>(0, 1)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 0)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 1)[z] = 40;
        largerBlackImageGrayscale.at<cv::Vec3b>(2, 1)[z] = 20;
        largerBlackImageGrayscale.at<cv::Vec3b>(1, 2)[z] = 20;
    }

    cv::Mat imageAfterModification = rgbImageProcessor->robertsOperator1(largerBlackImageGrayscale);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    for (int z = 0; z < 3; z++) {
        EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(0, 0)[z]);
        EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(1, 0)[z]);
        EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(0, 1)[z]);
        EXPECT_EQ(40, imageAfterModification.at<cv::Vec3b>(1, 1)[z]);
        EXPECT_EQ(0, imageAfterModification.at<cv::Vec3b>(0, 2)[z]);
        EXPECT_EQ(0, imageAfterModification.at<cv::Vec3b>(2, 0)[z]);
        EXPECT_EQ(20, imageAfterModification.at<cv::Vec3b>(2, 1)[z]);
        EXPECT_EQ(20, imageAfterModification.at<cv::Vec3b>(1, 2)[z]);
        EXPECT_EQ(0, imageAfterModification.at<cv::Vec3b>(2, 2)[z]);
    }
}

TEST_F(RGBImageProcessorTest, RegionGrowingTest) {
    cv::Mat image = cv::Mat::zeros(cv::Size(40, 40), CV_8UC3);
    image.at<cv::Vec3b>(20, 19) = 99;
    image.at<cv::Vec3b>(20, 20) = 100;
    image.at<cv::Vec3b>(20, 21) = 255;

    for (int i = 0; i < 3; i++) {
        cv::Mat segmentationMasks = rgbImageProcessor->regionGrowing(image, i);
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