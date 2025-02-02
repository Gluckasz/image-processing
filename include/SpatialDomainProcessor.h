//
// Created by gluckasz on 10/6/24.
//

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>

class SpatialDomainProcessor {
    template<typename TPixel>
    struct AccTypeFor;

    template<>
    struct AccTypeFor<uchar> {
        using type = int;
    };

    template<>
    struct AccTypeFor<cv::Vec3b> {
        using type = cv::Vec3i;
    };

    template<typename TPixel>
    static double getIntensity(const TPixel& pixel);

    template<>
    static double getIntensity<uchar>(const uchar& pixel) {
        return pixel;
    }

    template<>
    static double getIntensity<cv::Vec3b>(const cv::Vec3b& pixel) {
        return pixel[0] + pixel[1] + pixel[2];
    }

    static cv::Mat padImage(const cv::Mat &image, const int borderSize) {
        cv::Mat padded;
        copyMakeBorder(image, padded, borderSize, borderSize, borderSize, borderSize, cv::BORDER_REPLICATE);
        return padded;
    }
public:
     /**
     * Modify the brightness of an image by a constant factor.
     * @param image image to modify
     * @param modVal brightness modification value (positive makes image brighter and negative makes image darker)
     * @return image with modified brightness
     */
    static cv::Mat modifyBrightness(const cv::Mat& image, int modVal);

    /**
     * Modify contrast of an image using linear contrast stretching.
     * @param image image to modify
     * @param modVal contrast stretch modification value (positive makes contrast larger
     * and negative makes contrast smaller)
     * @return image with modified contrast
     */
    static cv::Mat modifyContrastLinear(const cv::Mat& image, int modVal);

    /**
     * Modify contrast of an image using gamma contrast correction.
     * @param image image to modify
     * @param modVal floating-point value indicating value of gamma
     * @return image with modified contrast
     */
    static cv::Mat modifyContrastGamma(const cv::Mat& image, float modVal);

    /**
     * Take the negative of an image.
     * @param image image to take the negative of
     * @return negative of an image
     */
    static cv::Mat negative(const cv::Mat& image);

    /**
     * Flip the image horizontally.
     * @param image to be flipped
     * @return flipped image horizontally
     */
    static cv::Mat flipHorizontally(const cv::Mat& image);

    /**
     * Flip the image vertically.
     * @param image to be flipped
     * @return flipped image vertically
     */
    static cv::Mat flipVertically(const cv::Mat& image);

    /**
     * Flip the image diagonally.
     * @param image to be flipped
     * @return flipped image diagonally
     */
    static cv::Mat flipDiagonally(const cv::Mat& image) ;

    /**
     * Resize an image by a factor.
     * @param image to be resized
     * @param factor of the resize operation
     * @return image of size = oldImage * factor
     */
    static cv::Mat resize(cv::Mat image, float factor);

    /**
     * Apply a midpoint filter.
     * @param image to apply filter to
     * @param kernelSize of the filter
     * @return image after applying the filter
     */
    static cv::Mat midpointFilter(cv::Mat image, int kernelSize);

    /**
     * Apply an arithmetic filter.
     * @param image to apply filter to
     * @param kernelSize of the filter
     * @return image after applying the filter
     */
    static cv::Mat arithmeticMeanFilter(cv::Mat image, int kernelSize);

    /**
     * Apply Laplacian-edge detection filter.
     * @param image Input image
     * @param laplaceMask Type of Laplacian mask to use
     * @return Edge detected image
     */
    template<typename TPixel>
    cv::Mat laplacianFilter(const cv::Mat& image, int laplaceMask);

    /**
     * Apply optimized Laplacian filter for edge detection.
     * @param image Input image
     * @return Edge detected image
     */
    template<typename TPixel>
    cv::Mat optimizedLaplacianFilter(cv::Mat image);

    /**
     * Apply Roberts cross gradient operator for edge detection.
     * @param image Input image
     * @return Edge detected image
     */
    template<typename TPixel>
    cv::Mat robertsOperator1(cv::Mat image);

    /**
     * Perform region growing segmentation.
     * @param image Input image
     * @param criterion Region growing criterion value
     * @return Segmented image
     */
    template<typename TPixel>
    cv::Mat regionGrowing(cv::Mat image, int criterion);
};

#endif //IMAGEPROCESSOR_H
