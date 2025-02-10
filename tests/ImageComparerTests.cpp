//
// Created by gluckasz on 2/10/25.
//

#include <gtest/gtest.h>
#include <image-processing-lib/ImageComparer.h>

class ImageComparerTest : public testing::Test {
protected:
    ImageComparerTest() {
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC1);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
    }

    cv::Mat blackImageGrayscale;
    cv::Mat whiteImageGrayscale;

    const uchar imageWidth = 2;
    const uchar imageHeight = 2;
};

TEST_F(ImageComparerTest, MeanSquareErrorTest) {
    const double mse = ImageComparer::meanSquareError(blackImageGrayscale, whiteImageGrayscale);

    const double expectedValue = pow(255, 2);
    EXPECT_EQ(expectedValue, mse);
}

TEST_F(ImageComparerTest, PeakMeanSquareErrorTest) {
    const double pmse = ImageComparer::peakMeanSquareError(whiteImageGrayscale, blackImageGrayscale);

    constexpr double expectedValue = 1;
    EXPECT_EQ(expectedValue, pmse);
}

TEST_F(ImageComparerTest, SignalToNoiseRatioTest) {
    const double snr = ImageComparer::signalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);

    const double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, snr);
}

TEST_F(ImageComparerTest, PeakSignalToNoiseRatioTest) {
    const double psnr = ImageComparer::peakSignalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);

    const double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, psnr);
}

TEST_F(ImageComparerTest, MaximumDifferenceTest) {
    const double md = ImageComparer::maximumDifference(whiteImageGrayscale, blackImageGrayscale);

    constexpr double expectedValue = 255;
    EXPECT_EQ(expectedValue, md);
}
