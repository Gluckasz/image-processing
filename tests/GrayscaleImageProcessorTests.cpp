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
    const uchar pixel1 = 10;
    const uchar pixel2 = 20;
    const uchar pixel3 = 30;
    const uchar pixel4 = 40;
    const int contrastLinearModVal = 10;

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
    const uchar pixel1 = 10;
    const uchar pixel2 = 20;
    const uchar pixel3 = 30;
    const uchar pixel4 = 40;
    const int contrastLinearModVal = 20;

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
