//
// Created by gluckasz on 12/8/24.
//

#include <gtest/gtest.h>

#include "GrayscaleImageProcessor.h"
#include "../include/ImageProcessor.h"

class ImageProcessorTest : public testing::Test {
protected:
    ImageProcessorTest() {
        grayscaleImageProcessor = std::make_unique<GrayscaleImageProcessor>();
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC1);
        largerBlackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth * 2, imageHeight * 2), CV_8UC1);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
    }

    std::unique_ptr<ImageProcessor> grayscaleImageProcessor;
    cv::Mat blackImageGrayscale;
    cv::Mat largerBlackImageGrayscale;
    cv::Mat whiteImageGrayscale;

    const uchar imageWidth= 4;
    const uchar imageHeight = 4;
};

TEST_F(ImageProcessorTest, DilationTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterDilation = grayscaleImageProcessor->dilation(blackImageGrayscale, 1);
    cv::Mat whiteAfterDilation = grayscaleImageProcessor->dilation(whiteImageGrayscale, 1);
    ASSERT_FALSE(blackAfterDilation.empty()) << "The image after modification should not be empty.";
    ASSERT_FALSE(whiteAfterDilation.empty()) << "The image after modification should not be empty.";

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            if (x == 0 && y == 1) {
                EXPECT_EQ(255, blackAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else {
                EXPECT_EQ(blackImageGrayscale.at<uchar>(x, y), blackAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
            EXPECT_EQ(255, whiteAfterDilation.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    blackAfterDilation = grayscaleImageProcessor->dilation(blackImageGrayscale, 6);
    whiteAfterDilation = grayscaleImageProcessor->dilation(whiteImageGrayscale, 6);
    ASSERT_FALSE(blackAfterDilation.empty()) << "The image after modification should not be empty.";
    ASSERT_FALSE(whiteAfterDilation.empty()) << "The image after modification should not be empty.";

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            if (x == 1 && y == 1) {
                EXPECT_EQ(0, blackAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else if (x != 3 && y != 3) {
                EXPECT_EQ(255, blackAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }

            EXPECT_EQ(255, whiteAfterDilation.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    blackAfterDilation = grayscaleImageProcessor->dilation(
        blackImageGrayscale,
        5,
        ImageProcessor::hmtMaskMap
        );
    whiteAfterDilation = grayscaleImageProcessor->dilation(
        whiteImageGrayscale,
        5,
        ImageProcessor::hmtMaskMap
        );
    ASSERT_FALSE(blackAfterDilation.empty()) << "The image after modification should not be empty.";
    ASSERT_FALSE(whiteAfterDilation.empty()) << "The image after modification should not be empty.";

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            if (
                (x == 1 && y == 1)
                || (x == 1 && y == 2)
                || (x == 2 && y == 1)
                || (x == 2 && y == 2)
                )
                EXPECT_EQ(255, blackAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(0, blackAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";

            EXPECT_EQ(255, whiteAfterDilation.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, DilationErrorConditionsTest) {
    cv::Mat emptyImage;
    EXPECT_THROW(grayscaleImageProcessor->dilation(emptyImage, 1), std::invalid_argument);
    int invalidMaskNumber = 0;
    EXPECT_THROW(grayscaleImageProcessor->dilation(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = 11;
    EXPECT_THROW(grayscaleImageProcessor->dilation(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = 0;
    EXPECT_THROW(grayscaleImageProcessor->dilation(blackImageGrayscale, invalidMaskNumber,
        ImageProcessor::hmtMaskMap), std::out_of_range);
    invalidMaskNumber = 13;
    EXPECT_THROW(grayscaleImageProcessor->dilation(blackImageGrayscale, invalidMaskNumber,
        ImageProcessor::hmtMaskMap), std::out_of_range);
}

TEST_F(ImageProcessorTest, ErosionTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterErosion = grayscaleImageProcessor->erosion(blackImageGrayscale, 1);
    cv::Mat whiteAfterErosion = grayscaleImageProcessor->erosion(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            if (x == 1 && (y == 0 || y == 1))
                EXPECT_EQ(0, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(255, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    blackAfterErosion = grayscaleImageProcessor->erosion(blackImageGrayscale, 6);
    whiteAfterErosion = grayscaleImageProcessor->erosion(whiteImageGrayscale, 6);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            if (x != 3 && y != 3 && !(x == 1 && y == 1))
                EXPECT_EQ(0, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(255, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    blackAfterErosion = grayscaleImageProcessor->erosion(
        blackImageGrayscale,
        5,
        ImageProcessor::hmtMaskMap
        );
    whiteAfterErosion = grayscaleImageProcessor->erosion(
        whiteImageGrayscale,
        5,
        ImageProcessor::hmtMaskMap
        );

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";

            if (
                (x == 1 && y == 1)
                || (x == 1 && y == 2)
                || (x == 2 && y == 1)
                || (x == 2 && y == 2)
                )
                EXPECT_EQ(0, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(255, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, ErosionErrorConditionsTest) {
    cv::Mat emptyImage;
    EXPECT_THROW(grayscaleImageProcessor->erosion(emptyImage, 1), std::invalid_argument);
    int invalidMaskNumber = -1;
    EXPECT_THROW(grayscaleImageProcessor->erosion(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = 11;
    EXPECT_THROW(grayscaleImageProcessor->erosion(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = -1;
    EXPECT_THROW(grayscaleImageProcessor->erosion(blackImageGrayscale, invalidMaskNumber,
        ImageProcessor::hmtMaskMap), std::out_of_range);
    invalidMaskNumber = 13;
    EXPECT_THROW(grayscaleImageProcessor->erosion(blackImageGrayscale, invalidMaskNumber,
        ImageProcessor::hmtMaskMap), std::out_of_range);
}

TEST_F(ImageProcessorTest, OpeningTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterOpening = grayscaleImageProcessor->opening(blackImageGrayscale, 1);
    cv::Mat whiteAfterOpening = grayscaleImageProcessor->opening(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterOpening.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            EXPECT_EQ(255, whiteAfterOpening.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, ClosingTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterClosing = grayscaleImageProcessor->closing(blackImageGrayscale, 1);
    cv::Mat whiteAfterClosing = grayscaleImageProcessor->closing(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterClosing.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            EXPECT_EQ(255, whiteAfterClosing.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, HmtTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterHmt = grayscaleImageProcessor->hmt(blackImageGrayscale, 1);
    cv::Mat whiteAfterHmt = grayscaleImageProcessor->hmt(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterHmt.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            if (x == 1 && y == 1)
                EXPECT_EQ(255, whiteAfterHmt.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(0, whiteAfterHmt.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, TaskM4Test) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterTaskM4 = grayscaleImageProcessor->taskM4(blackImageGrayscale);
    cv::Mat whiteAfterTaskM4 = grayscaleImageProcessor->taskM4(whiteImageGrayscale);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            if (x == 1 && y == 1) {
                EXPECT_EQ(255, blackAfterTaskM4.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
                EXPECT_EQ(0, whiteAfterTaskM4.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else {
                EXPECT_EQ(0, blackAfterTaskM4.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
                EXPECT_EQ(255, whiteAfterTaskM4.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }
}