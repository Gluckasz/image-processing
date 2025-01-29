//
// Created by gluckasz on 1/29/25.
//

#ifndef MORPHOLOGICALPROCESSOR_H
#define MORPHOLOGICALPROCESSOR_H
#include <unordered_map>
#include <vector>
#include <opencv2/opencv.hpp>


class MorphologicalProcessor {
public:
    /**
     * Compute complement (negative) of binary image.
     * @param image Input binary image
     * @return Complemented image
     */
    static cv::Mat complement(cv::Mat image);

    /**
     * Compute the union of two binary images.
     * @param image1 First input binary image
     * @param image2 Second input binary image
     * @return Union of input images
     */
    static cv::Mat imagesUnion(cv::Mat image1, cv::Mat image2);

    /**
     * Check if two images are equal.
     * @param image1 First image to compare
     * @param image2 Second image to compare
     * @return true if images are equal, false otherwise
     */
    static bool areEqual(cv::Mat image1, cv::Mat image2);

    enum class FieldType {
        WHITE,
        BLACK,
        WHITE_MARKER,
        BLACK_MARKER,
        INACTIVE
    };

    static const std::unordered_map<int, std::vector<std::vector<FieldType> > > maskMap;
    static const std::unordered_map<int, std::vector<std::vector<FieldType> > > hmtMaskMap;
    static const std::unordered_map<int, std::vector<std::vector<FieldType> > > hmtComplementMaskMap;

    /**
     * Apply dilation morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Dilated image
     */
    static cv::Mat dilation(cv::Mat image, int maskNumber,
                     const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply erosion morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Eroded image
     */
    static cv::Mat erosion(cv::Mat image, int maskNumber,
                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply opening morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Opened image
     */
    static cv::Mat opening(const cv::Mat &image, int maskNumber,
                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply closing morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Closed image
     */
    static cv::Mat closing(const cv::Mat &image, int maskNumber,
                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply hit-or-miss transform.
     * @param image Input binary image
     * @param maskNumber HMT mask number
     * @param maskMapping Map of available HMT masks
     * @return Transformed image
     */
    static cv::Mat hmt(const cv::Mat &image, int maskNumber,
                       const std::unordered_map<int, std::vector<std::vector<FieldType>>> &maskMapping = hmtMaskMap);

    /**
     * Create a color map for visualization.
     * @return Vector of BGR colors
     */
    static std::vector<cv::Vec3b> createColorMap();

    /**
     * Apply a color map to grayscale image.
     * @param grayscaleMask Input grayscale image
     * @param colorMap Color map to apply
     * @return Colored image
     */
    static cv::Mat applyColorMap(const cv::Mat &grayscaleMask, const std::vector<cv::Vec3b> &colorMap);
};


#endif //MORPHOLOGICALPROCESSOR_H
