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
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(blackImageGrayscale, whiteImageGrayscale), std::invalid_argument);
    const double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, snr);
}

TEST_F(ImageComparerTest, PeakSignalToNoiseRatioTest) {
    const double psnr = ImageComparer::peakSignalToNoiseRatio(whiteImageGrayscale, blackImageGrayscale);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(whiteImageGrayscale, whiteImageGrayscale),
                 std::invalid_argument);
    const double expectedValue = 10 * std::log10(1);
    EXPECT_EQ(expectedValue, psnr);
}

TEST_F(ImageComparerTest, MaximumDifferenceTest) {
    const double md = ImageComparer::maximumDifference(whiteImageGrayscale, blackImageGrayscale);

    constexpr double expectedValue = 255;
    EXPECT_EQ(expectedValue, md);
}

TEST_F(ImageComparerTest, EmptyImageTest) {
    cv::Mat emptyImage(0, 0, CV_8UC1);

    EXPECT_THROW(ImageComparer::meanSquareError(whiteImageGrayscale, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakMeanSquareError(whiteImageGrayscale, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(whiteImageGrayscale, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(whiteImageGrayscale, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::maximumDifference(whiteImageGrayscale, emptyImage), std::invalid_argument);

    EXPECT_THROW(ImageComparer::meanSquareError(emptyImage, whiteImageGrayscale), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakMeanSquareError(emptyImage, whiteImageGrayscale), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(emptyImage, whiteImageGrayscale), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(emptyImage, whiteImageGrayscale), std::invalid_argument);
    EXPECT_THROW(ImageComparer::maximumDifference(emptyImage, whiteImageGrayscale), std::invalid_argument);

    EXPECT_THROW(ImageComparer::meanSquareError(emptyImage, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakMeanSquareError(emptyImage, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(emptyImage, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(emptyImage, emptyImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::maximumDifference(emptyImage, emptyImage), std::invalid_argument);
}

TEST_F(ImageComparerTest, ZeroValueImageTest) {
    EXPECT_THROW(ImageComparer::peakMeanSquareError(blackImageGrayscale, blackImageGrayscale), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(blackImageGrayscale, blackImageGrayscale), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(blackImageGrayscale, blackImageGrayscale),
                 std::invalid_argument);
}

TEST_F(ImageComparerTest, DifferentSizeImagesTest) {
    cv::Mat largerImage = cv::Mat::zeros(cv::Size(imageWidth + 1, imageHeight + 1), CV_8UC1);
    EXPECT_THROW(ImageComparer::meanSquareError(blackImageGrayscale, largerImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakMeanSquareError(blackImageGrayscale, largerImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(blackImageGrayscale, largerImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(blackImageGrayscale, largerImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::maximumDifference(blackImageGrayscale, largerImage), std::invalid_argument);

    cv::Mat differentColsImage = cv::Mat::zeros(cv::Size(imageWidth, imageHeight + 1), CV_8UC1);
    EXPECT_THROW(ImageComparer::meanSquareError(blackImageGrayscale, differentColsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakMeanSquareError(blackImageGrayscale, differentColsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(blackImageGrayscale, differentColsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(blackImageGrayscale, differentColsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::maximumDifference(blackImageGrayscale, differentColsImage), std::invalid_argument);

    cv::Mat differentRowsImage = cv::Mat::zeros(cv::Size(1, 2), CV_8UC1);
    EXPECT_THROW(ImageComparer::meanSquareError(blackImageGrayscale, differentRowsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakMeanSquareError(blackImageGrayscale, differentRowsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::signalToNoiseRatio(blackImageGrayscale, differentRowsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::peakSignalToNoiseRatio(blackImageGrayscale, differentRowsImage), std::invalid_argument);
    EXPECT_THROW(ImageComparer::maximumDifference(blackImageGrayscale, differentRowsImage), std::invalid_argument);
}

TEST_F(ImageComparerTest, IdenticalMixedPixelTest) {
    cv::Mat image1(cv::Size(imageWidth, imageHeight), CV_8UC1);
    image1.at<uchar>(0, 0) = 100;
    image1.at<uchar>(0, 1) = 150;
    image1.at<uchar>(1, 0) = 200;
    image1.at<uchar>(1, 1) = 250;

    EXPECT_EQ(ImageComparer::meanSquareError(image1, image1), 0);
    EXPECT_EQ(ImageComparer::maximumDifference(image1, image1), 0);
}
