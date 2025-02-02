//
// Created by gluckasz on 1/29/25.
//

#ifndef MORPHOLOGICALPROCESSOR_H
#define MORPHOLOGICALPROCESSOR_H
#include <unordered_map>
#include <vector>
#include <opencv2/opencv.hpp>
#include <Masks.hpp>


using Masks::FieldType;
using Masks::maskMap;
using Masks::hmtMaskMap;
using Masks::hmtComplementMaskMap;

namespace MorphologicalProcessor {
    /**
     * Compute complement (negative) of binary image.
     * @param image Input binary image
     * @return Complemented image
     */
    cv::Mat complement(cv::Mat image);

    /**
     * Compute the union of two binary images.
     * @param image1 First input binary image
     * @param image2 Second input binary image
     * @return Union of input images
     */
    cv::Mat imagesUnion(cv::Mat image1, cv::Mat image2);

    /**
     * Check if two images are equal.
     * @param image1 First image to compare
     * @param image2 Second image to compare
     * @return true if images are equal, false otherwise
     */
    bool areEqual(cv::Mat image1, cv::Mat image2);

    /**
     * Apply dilation morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Dilated image
     */
    cv::Mat dilation(cv::Mat image, int maskNumber,
                     const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply erosion morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Eroded image
     */
    cv::Mat erosion(cv::Mat image, int maskNumber,
                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply opening morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Opened image
     */
    cv::Mat opening(const cv::Mat &image, int maskNumber,
                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply closing morphological operation.
     * @param image Input binary image
     * @param maskNumber Structural element number
     * @param maskMapping Map of available structural elements
     * @return Closed image
     */
    cv::Mat closing(const cv::Mat &image, int maskNumber,
                    const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = maskMap);

    /**
     * Apply hit-or-miss transform.
     * @param image Input binary image
     * @param maskNumber HMT mask number
     * @param maskMapping Map of available HMT masks
     * @return Transformed image
     */
    cv::Mat hmt(const cv::Mat &image, int maskNumber,
                const std::unordered_map<int, std::vector<std::vector<FieldType> > > &maskMapping = hmtMaskMap);
}


#endif //MORPHOLOGICALPROCESSOR_H
