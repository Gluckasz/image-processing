//
// Created by gluckasz on 2/10/25.
//

#include <gtest/gtest.h>
#include <image-processing-lib/HistogramProcessor.h>

class HistogramProcessorTest : public testing::Test {
protected:
    HistogramProcessorTest() {
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC1);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
    }

    cv::Mat blackImageGrayscale;
    cv::Mat whiteImageGrayscale;

    const uchar imageWidth = 2;
    const uchar imageHeight = 2;
};

TEST_F(HistogramProcessorTest, ComputeHistogramTest) {
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(whiteImageGrayscale);
    ASSERT_EQ(histogram[UCHAR_MAX], 4);
    for (int i = 0; i < UCHAR_MAX; i++) {
        EXPECT_EQ(histogram[i], 0);
    }
}

TEST_F(HistogramProcessorTest, HistogramTest) {
    constexpr int binWidth = 8;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(whiteImageGrayscale);
    const cv::Mat histogramVis = HistogramProcessor::histogramVisualization(histogram, binWidth);
    ASSERT_EQ(4, histogramVis.rows);
    ASSERT_EQ(2056, histogramVis.cols);
    for (int y = UCHAR_MAX * binWidth; y < 2048; y++) {
        for (int x = 0; x < histogramVis.rows; x++) {
            EXPECT_EQ(histogramVis.at<uchar>(x, y), 255)
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
    for (int y = 0; y < UCHAR_MAX * binWidth; y++) {
        for (int x = 0; x < histogramVis.rows; x++) {
            EXPECT_EQ(histogramVis.at<uchar>(x, y), 0)
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
    for (int y = 2048; y < histogramVis.cols; y++) {
        for (int x = 0; x < histogramVis.rows; x++) {
            EXPECT_EQ(histogramVis.at<uchar>(x, y), 0)
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(HistogramProcessorTest, HistogramEqualizationTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 1;
    blackImageGrayscale.at<uchar>(0, 1) = 1;
    blackImageGrayscale.at<uchar>(1, 0) = 254;
    blackImageGrayscale.at<uchar>(1, 1) = 254;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    cv::Mat imageAfterModification = HistogramProcessor::histogramEqualization(blackImageGrayscale, histogram, 255, 0);
    ASSERT_FALSE(imageAfterModification.empty()) << "The imageAfterModification should not be empty.";

    EXPECT_EQ(128, imageAfterModification.at<uchar>(0, 0));
    EXPECT_EQ(128, imageAfterModification.at<uchar>(0, 1));
    EXPECT_EQ(255, imageAfterModification.at<uchar>(1, 0));
    EXPECT_EQ(255, imageAfterModification.at<uchar>(1, 1));
}

TEST_F(HistogramProcessorTest, MeanTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double mean = HistogramProcessor::mean(histogram);

    EXPECT_EQ(25, mean);
}

TEST_F(HistogramProcessorTest, VarianceTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double variance = HistogramProcessor::variance(histogram);

    EXPECT_EQ(125, variance);
}

TEST_F(HistogramProcessorTest, StandardDeviationTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double std = HistogramProcessor::standardDeviation(histogram);

    EXPECT_EQ(sqrt(125), std);
}

TEST_F(HistogramProcessorTest, Variation1Test) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double var1 = HistogramProcessor::variation1(histogram);

    EXPECT_EQ(5, var1);
}

TEST_F(HistogramProcessorTest, AsymmetryTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double asym = HistogramProcessor::asymmetry(histogram);

    EXPECT_EQ(0, asym);
}

TEST_F(HistogramProcessorTest, FlatteningTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double flat = HistogramProcessor::flattening(histogram);

    EXPECT_EQ(-1.36, flat);
}

TEST_F(HistogramProcessorTest, Variation2Test) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double var2 = HistogramProcessor::variation2(histogram);

    EXPECT_EQ(0.25, var2);
}

TEST_F(HistogramProcessorTest, EntropyTest) {
    blackImageGrayscale.at<uchar>(0, 0) = 10;
    blackImageGrayscale.at<uchar>(0, 1) = 20;
    blackImageGrayscale.at<uchar>(1, 0) = 30;
    blackImageGrayscale.at<uchar>(1, 1) = 40;
    const std::array<uint, UCHAR_MAX + 1> histogram = HistogramProcessor::computeHistogram(blackImageGrayscale);
    const double ent = HistogramProcessor::entropy(histogram);

    EXPECT_EQ(2, ent);
}
