//
// Created by gluckasz on 2/3/25.
//

#ifndef IMAGEOPERATION_H
#define IMAGEOPERATION_H
#include <opencv2/opencv.hpp>

class ImageOperation {
public:
    virtual ~ImageOperation() = default;

    virtual void apply(cv::Mat &image) const = 0;
};
#endif //IMAGEOPERATION_H
