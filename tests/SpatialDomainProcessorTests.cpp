//
// Created by gluckasz on 2/10/25.
//

#include <gtest/gtest.h>
#include <image-processing-lib/SpatialDomainProcessor.h>

class SpatialDomainProcessorTest : public testing::Test {
protected:
    SpatialDomainProcessorTest() {
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC1);
        largerBlackImageGrayscale = cv::Mat::zeros(cv::Size(3, 3), CV_8UC1);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
        blackImageGrayscaleColor = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC3);
        largerBlackImageGrayscaleColor = cv::Mat::zeros(cv::Size(3, 3), CV_8UC3);
        whiteImageGrayscaleColor = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC3,
                                           cv::Scalar(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX));
    }

    cv::Mat blackImageGrayscale;
    cv::Mat largerBlackImageGrayscale;
    cv::Mat whiteImageGrayscale;
    cv::Mat blackImageGrayscaleColor;
    cv::Mat largerBlackImageGrayscaleColor;
    cv::Mat whiteImageGrayscaleColor;

    const uchar imageWidth = 2;
    const uchar imageHeight = 2;

    const int brightnessModVal = 10;
};

TEST_F(SpatialDomainProcessorTest, ModifyBrightnessTestNormalCases) {
    cv::Mat brighterImage = SpatialDomainProcessor::modifyBrightness(blackImageGrayscale, brightnessModVal);
    cv::Mat darkerImage = SpatialDomainProcessor::modifyBrightness(whiteImageGrayscale, -brightnessModVal);
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

TEST_F(SpatialDomainProcessorTest, ModifyBrightnessTestEdgeCases) {
    cv::Mat brighterImage = SpatialDomainProcessor::modifyBrightness(blackImageGrayscale, -brightnessModVal);
    cv::Mat darkerImage = SpatialDomainProcessor::modifyBrightness(whiteImageGrayscale, brightnessModVal);
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

TEST_F(SpatialDomainProcessorTest, ModifyContrastLinearTestNormalCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int contrastLinearModVal = 10;

    blackImageGrayscale.at<uchar>(0, 0) = pixel1;
    blackImageGrayscale.at<uchar>(1, 0) = pixel2;
    blackImageGrayscale.at<uchar>(0, 1) = pixel3;
    blackImageGrayscale.at<uchar>(1, 1) = pixel4;

    cv::Mat modifiedMoreContrastImage = SpatialDomainProcessor::modifyContrastLinear(
        blackImageGrayscale, contrastLinearModVal);
    ASSERT_FALSE(modifiedMoreContrastImage.empty()) << "The modifiedMoreContrastImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 0), 16);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 1), 33);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 1), 50);

    cv::Mat modifiedLessContrastImage = SpatialDomainProcessor::modifyContrastLinear(
        blackImageGrayscale, -contrastLinearModVal);
    ASSERT_FALSE(modifiedLessContrastImage.empty()) << "The modifiedLessContrastImage should not be empty.";

    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 0), 20);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 0), 23);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 1), 26);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 1), 30);
}

TEST_F(SpatialDomainProcessorTest, ModifyContrastLinearTestEdgeCases) {
    constexpr uchar pixel1 = 10;
    constexpr uchar pixel2 = 20;
    constexpr uchar pixel3 = 30;
    constexpr uchar pixel4 = 40;
    constexpr int contrastLinearModVal = 20;

    blackImageGrayscale.at<uchar>(0, 0) = pixel1;
    blackImageGrayscale.at<uchar>(1, 0) = pixel2;
    blackImageGrayscale.at<uchar>(0, 1) = pixel3;
    blackImageGrayscale.at<uchar>(1, 1) = pixel4;

    cv::Mat modifiedMoreContrastImage = SpatialDomainProcessor::modifyContrastLinear(
        blackImageGrayscale, contrastLinearModVal);
    ASSERT_FALSE(modifiedMoreContrastImage.empty()) << "The modifiedMoreContrastImage should not be empty.";

    // Calculated values for contrastLinearModVal = 20
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 0), 13);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(0, 1), 36);
    EXPECT_EQ(modifiedMoreContrastImage.at<uchar>(1, 1), 60);

    cv::Mat modifiedLessContrastImage = SpatialDomainProcessor::modifyContrastLinear(
        blackImageGrayscale, -contrastLinearModVal);
    ASSERT_FALSE(modifiedLessContrastImage.empty()) << "The modifiedLessContrastImage should not be empty.";

    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 0), 30);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 0), 30);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(0, 1), 30);
    EXPECT_EQ(modifiedLessContrastImage.at<uchar>(1, 1), 30);
}

TEST_F(SpatialDomainProcessorTest, ModifyContrastGammaTestNormalCases) {
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

    cv::Mat highGammaImage = SpatialDomainProcessor::modifyContrastGamma(blackImageGrayscale, highGamma);
    ASSERT_FALSE(highGammaImage.empty()) << "The highGammaImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    EXPECT_EQ(highGammaImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 0), 2);
    EXPECT_EQ(highGammaImage.at<uchar>(0, 1), 4);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 1), 6);

    cv::Mat lowGammaImage = SpatialDomainProcessor::modifyContrastGamma(blackImageGrayscale, lowGamma);
    ASSERT_FALSE(lowGammaImage.empty()) << "The lowGammaImage should not be empty.";

    EXPECT_EQ(lowGammaImage.at<uchar>(0, 0), 50);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 0), 71);
    EXPECT_EQ(lowGammaImage.at<uchar>(0, 1), 87);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 1), 101);
}

TEST_F(SpatialDomainProcessorTest, ModifyContrastGammaTestEdgeCases) {
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

    cv::Mat highGammaImage = SpatialDomainProcessor::modifyContrastGamma(blackImageGrayscale, highGamma);
    ASSERT_FALSE(highGammaImage.empty()) << "The highGammaImage should not be empty.";

    // Calculated values for contrastLinearModVal = 10
    EXPECT_EQ(highGammaImage.at<uchar>(0, 0), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 0), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(0, 1), 0);
    EXPECT_EQ(highGammaImage.at<uchar>(1, 1), 0);

    cv::Mat lowGammaImage = SpatialDomainProcessor::modifyContrastGamma(blackImageGrayscale, lowGamma);
    ASSERT_FALSE(lowGammaImage.empty()) << "The lowGammaImage should not be empty.";

    EXPECT_EQ(lowGammaImage.at<uchar>(0, 0), 255);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 0), 255);
    EXPECT_EQ(lowGammaImage.at<uchar>(0, 1), 255);
    EXPECT_EQ(lowGammaImage.at<uchar>(1, 1), 255);
}

TEST_F(SpatialDomainProcessorTest, NegativeTest) {
    cv::Mat brighterImage = SpatialDomainProcessor::negative(blackImageGrayscale);
    cv::Mat darkerImage = SpatialDomainProcessor::negative(whiteImageGrayscale);
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

TEST_F(SpatialDomainProcessorTest, HorizontalFlipTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    cv::Mat imageAfterFlip = SpatialDomainProcessor::flipHorizontally(blackImageGrayscale);
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

TEST_F(SpatialDomainProcessorTest, VerticalFlipTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    cv::Mat imageAfterFlip = SpatialDomainProcessor::flipVertically(blackImageGrayscale);
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

TEST_F(SpatialDomainProcessorTest, DiagonalFlipTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    cv::Mat imageAfterFlip = SpatialDomainProcessor::flipDiagonally(blackImageGrayscale);
    ASSERT_FALSE(imageAfterFlip.empty()) << "The imageAfterFlip should not be empty.";

    for (int row = 0; row < blackImageGrayscale.rows; row++) {
        for (int col = 0; col < blackImageGrayscale.cols; col++) {
            int originalPixel = blackImageGrayscale.at<uchar>(blackImageGrayscale.rows - 1 - row,
                                                              blackImageGrayscale.cols - 1 - col);
            int modifiedPixel = imageAfterFlip.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(SpatialDomainProcessorTest, ResizeTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    constexpr float factor = 2;

    cv::Mat imageAfterEnlargement = SpatialDomainProcessor::resize(blackImageGrayscale, factor);
    cv::Mat imageAfterShrinking = SpatialDomainProcessor::resize(blackImageGrayscale, 1 / factor);
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
            originalPixel = blackImageGrayscale.at<uchar>(static_cast<int>(static_cast<float>(row) / factor),
                                                          static_cast<int>(static_cast<float>(col) / factor));
            modifiedPixel = imageAfterEnlargement.at<uchar>(row, col);
            EXPECT_EQ(originalPixel, modifiedPixel)
                << "Mismatch at pixel (" << row << ", " << col << ")";
        }
    }
}

TEST_F(SpatialDomainProcessorTest, MidpointFilterTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    cv::Mat imageAfterModification = SpatialDomainProcessor::midpointFilter(blackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    constexpr int expectedValue = (10 + 0) / 2;
    const int modifiedPixel = imageAfterModification.at<uchar>(0, 0);
    EXPECT_EQ(expectedValue, modifiedPixel)
        << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
}

TEST_F(SpatialDomainProcessorTest, ArithmeticMeanFilterTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 10;
    cv::Mat imageAfterModification = SpatialDomainProcessor::arithmeticMeanFilter(blackImageGrayscale, 2);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    constexpr int expectedValue = (10 + 10 + 0 + 0) / 4;
    const int modifiedPixel = imageAfterModification.at<uchar>(0, 0);
    EXPECT_EQ(expectedValue, modifiedPixel)
        << "Mismatch at pixel (" << 0 << ", " << 0 << ")";
}

TEST_F(SpatialDomainProcessorTest, LaplacianFilterGrayscaleTest) {
    largerBlackImageGrayscale.at<uchar>(0, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 0) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 1) = 40;
    largerBlackImageGrayscale.at<uchar>(2, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 2) = 20;

    cv::Mat imageAfterModification = SpatialDomainProcessor::laplacianFilter(largerBlackImageGrayscale, 0);
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

    imageAfterModification = SpatialDomainProcessor::laplacianFilter(largerBlackImageGrayscale, 1);
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

    imageAfterModification = SpatialDomainProcessor::laplacianFilter(largerBlackImageGrayscale, 2);
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

TEST_F(SpatialDomainProcessorTest, OptimizedLaplacianFilterGrayscaleTest) {
    largerBlackImageGrayscale.at<uchar>(0, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 0) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 1) = 40;
    largerBlackImageGrayscale.at<uchar>(2, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 2) = 20;

    cv::Mat imageAfterModification = SpatialDomainProcessor::optimizedLaplacianFilter(largerBlackImageGrayscale);
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

TEST_F(SpatialDomainProcessorTest, RobertsOperator1GrayscaleTest) {
    largerBlackImageGrayscale.at<uchar>(0, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 0) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 1) = 40;
    largerBlackImageGrayscale.at<uchar>(2, 1) = 20;
    largerBlackImageGrayscale.at<uchar>(1, 2) = 20;

    cv::Mat imageAfterModification = SpatialDomainProcessor::robertsOperator1(largerBlackImageGrayscale);
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

TEST_F(SpatialDomainProcessorTest, RegionGrowingGrayscaleTest) {
    cv::Mat image = cv::Mat::zeros(cv::Size(40, 40), CV_8UC1);
    image.at<uchar>(20, 19) = 99;
    image.at<uchar>(20, 20) = 100;
    image.at<uchar>(20, 21) = 255;

    for (int i = 0; i < 3; i++) {
        cv::Mat segmentationMasks = SpatialDomainProcessor::regionGrowing<uchar>(image, i);
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

TEST_F(SpatialDomainProcessorTest, RegionGrowingTest) {
    cv::Mat image = cv::Mat::zeros(cv::Size(40, 40), CV_8UC3);
    image.at<cv::Vec3b>(20, 19) = 99;
    image.at<cv::Vec3b>(20, 20) = 100;
    image.at<cv::Vec3b>(20, 21) = 255;

    for (int i = 0; i < 3; i++) {
        cv::Mat segmentationMasks = SpatialDomainProcessor::regionGrowing<cv::Vec3b>(image, i);
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
