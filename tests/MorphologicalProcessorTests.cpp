//
// Created by gluckasz on 12/8/24.
//

#include <gtest/gtest.h>

#include "image-processing-lib/MorphologicalProcessor.h"

class ImageProcessorTest : public testing::Test {
protected:
    ImageProcessorTest() {
        blackImageGrayscale = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_8UC1);
        whiteImageGrayscale = cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC1, cv::Scalar(UCHAR_MAX));
    }

    cv::Mat blackImageGrayscale;
    cv::Mat whiteImageGrayscale;

    const uchar imageWidth= 4;
    const uchar imageHeight = 4;
};

TEST_F(ImageProcessorTest, DilationTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterDilation = MorphologicalProcessor::dilation(blackImageGrayscale, 1);
    cv::Mat whiteAfterDilation = MorphologicalProcessor::dilation(whiteImageGrayscale, 1);
    ASSERT_FALSE(blackAfterDilation.empty()) << "The image after modification should not be empty.";
    ASSERT_FALSE(whiteAfterDilation.empty()) << "The image after modification should not be empty.";

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            if (x == 1 && y == 0) {
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

    blackAfterDilation = MorphologicalProcessor::dilation(blackImageGrayscale, 6);
    whiteAfterDilation = MorphologicalProcessor::dilation(whiteImageGrayscale, 6);
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

            if (x == 3 && y == 3) {
                EXPECT_EQ(0, whiteAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else {
                EXPECT_EQ(255, whiteAfterDilation.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }

    blackAfterDilation = MorphologicalProcessor::dilation(
        blackImageGrayscale,
        5,
        Masks::hmtMaskMap
        );
    whiteAfterDilation = MorphologicalProcessor::dilation(
        whiteImageGrayscale,
        5,
        Masks::hmtMaskMap
        );
    ASSERT_FALSE(blackAfterDilation.empty()) << "The image after modification should not be empty.";
    ASSERT_FALSE(whiteAfterDilation.empty()) << "The image after modification should not be empty.";

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(255, blackAfterDilation.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";

            EXPECT_EQ(255, whiteAfterDilation.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, DilationErrorConditionsTest) {
    cv::Mat emptyImage;
    EXPECT_THROW(MorphologicalProcessor::dilation(emptyImage, 1), std::invalid_argument);
    int invalidMaskNumber = 0;
    EXPECT_THROW(MorphologicalProcessor::dilation(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = 11;
    EXPECT_THROW(MorphologicalProcessor::dilation(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = 0;
    EXPECT_THROW(MorphologicalProcessor::dilation(blackImageGrayscale, invalidMaskNumber,
        hmtMaskMap), std::out_of_range);
    invalidMaskNumber = 13;
    EXPECT_THROW(MorphologicalProcessor::dilation(blackImageGrayscale, invalidMaskNumber,
        hmtMaskMap), std::out_of_range);
}

TEST_F(ImageProcessorTest, ErosionTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterErosion = MorphologicalProcessor::erosion(blackImageGrayscale, 1);
    cv::Mat whiteAfterErosion = MorphologicalProcessor::erosion(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            if (x == 1 && (y == 0 || y == 1) || y == 3)
                EXPECT_EQ(0, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(255, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    blackAfterErosion = MorphologicalProcessor::erosion(blackImageGrayscale, 6);
    whiteAfterErosion = MorphologicalProcessor::erosion(whiteImageGrayscale, 6);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
            if (x == 3 || y == 3 || !(x == 1 && y == 1))
                EXPECT_EQ(0, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            else
                EXPECT_EQ(255, whiteAfterErosion.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }

    blackAfterErosion = MorphologicalProcessor::erosion(
        blackImageGrayscale,
        5,
        hmtMaskMap
        );
    whiteAfterErosion = MorphologicalProcessor::erosion(
        whiteImageGrayscale,
        5,
        hmtMaskMap
        );

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";

            EXPECT_EQ(0, whiteAfterErosion.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";
        }
    }
}

TEST_F(ImageProcessorTest, ErosionErrorConditionsTest) {
    cv::Mat emptyImage;
    EXPECT_THROW(MorphologicalProcessor::erosion(emptyImage, 1), std::invalid_argument);
    int invalidMaskNumber = -1;
    EXPECT_THROW(MorphologicalProcessor::erosion(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = 11;
    EXPECT_THROW(MorphologicalProcessor::erosion(blackImageGrayscale, invalidMaskNumber), std::out_of_range);
    invalidMaskNumber = -1;
    EXPECT_THROW(MorphologicalProcessor::erosion(blackImageGrayscale, invalidMaskNumber,
        hmtMaskMap), std::out_of_range);
    invalidMaskNumber = 13;
    EXPECT_THROW(MorphologicalProcessor::erosion(blackImageGrayscale, invalidMaskNumber,
        hmtMaskMap), std::out_of_range);
}

TEST_F(ImageProcessorTest, OpeningTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterOpening = MorphologicalProcessor::opening(blackImageGrayscale, 1);
    cv::Mat whiteAfterOpening = MorphologicalProcessor::opening(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            EXPECT_EQ(0, blackAfterOpening.at<uchar>(x, y))
            << "Mismatch at pixel (" << x << ", " << y << ")";

            if (y == 3 || (x == 1 && y == 0)) {
                EXPECT_EQ(0, whiteAfterOpening.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else {
                EXPECT_EQ(255, whiteAfterOpening.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }
}

TEST_F(ImageProcessorTest, ClosingTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterClosing = MorphologicalProcessor::closing(blackImageGrayscale, 1);
    cv::Mat whiteAfterClosing = MorphologicalProcessor::closing(whiteImageGrayscale, 1);

    for (int x = 0; x < blackImageGrayscale.rows; x++) {
        for (int y = 0; y < blackImageGrayscale.cols; y++) {
            if (x == 1 && y == 0) {
                EXPECT_EQ(255, blackAfterClosing.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else {
                EXPECT_EQ(0, blackAfterClosing.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }

            if (y == 3) {
                EXPECT_EQ(0, whiteAfterClosing.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            } else {
                EXPECT_EQ(255, whiteAfterClosing.at<uchar>(x, y))
                << "Mismatch at pixel (" << x << ", " << y << ")";
            }
        }
    }
}

TEST_F(ImageProcessorTest, HmtTest) {
    blackImageGrayscale.at<uchar>(1, 1) = 255;
    whiteImageGrayscale.at<uchar>(1, 1) = 0;

    cv::Mat blackAfterHmt = MorphologicalProcessor::hmt(blackImageGrayscale, 1);
    cv::Mat whiteAfterHmt = MorphologicalProcessor::hmt(whiteImageGrayscale, 1);

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