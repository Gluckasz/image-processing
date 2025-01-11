//
// Created by gluckasz on 12/7/24.
//

#include "../include/ImageProcessor.h"



cv::Mat ImageProcessor::complement(cv::Mat image) {
    cv::Mat result = image.clone();
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            result.at<uchar>(x, y) = 255 - image.at<uchar>(x, y);
        }
    }

    return result;
}

bool ImageProcessor::areEqual(cv::Mat image1, cv::Mat image2) {
    if (image1.rows != image2.rows || image1.cols != image2.cols) {
        return false;
    }
    for (int x = 0; x < image1.rows; x++) {
        for (int y = 0; y < image1.cols; y++) {
            if (image1.at<uchar>(x, y) != image2.at<uchar>(x, y)) {
                return false;
            }
        }
    }
    return true;
}

cv::Mat ImageProcessor::imagesUnion(cv::Mat image1, cv::Mat image2) {
    cv::Mat result = image1.clone();

    for (int x = 0; x < image1.rows; x++) {
        for (int y = 0; y < image1.cols; y++) {
            result.at<uchar>(x, y) = std::max(image1.at<uchar>(x, y), image2.at<uchar>(x, y));
        }
    }

    return result;
}


const std::unordered_map<int, std::vector<std::vector<ImageProcessor::FieldType>>> ImageProcessor::maskMap = {
    {1, {{FieldType::WHITE_MARKER, FieldType::WHITE}}},
    {2, {
            {FieldType::WHITE_MARKER},
            {FieldType::WHITE}
    }},
    {3, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {4, {
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::INACTIVE}
    }},
    {5, {
            {FieldType::WHITE_MARKER, FieldType::WHITE},
            {FieldType::WHITE, FieldType::INACTIVE}
    }},
    {6, {
            {FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::WHITE, FieldType::INACTIVE}
    }},
    {7, {{FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE}}},
    {8, {{FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::WHITE}}},
    {9, {
            {FieldType::WHITE, FieldType::WHITE_MARKER},
            {FieldType::WHITE, FieldType::INACTIVE}
    }},
    {10, {
            {FieldType::WHITE, FieldType::WHITE},
            {FieldType::WHITE_MARKER, FieldType::INACTIVE}
    }},
};

const std::unordered_map<int, std::vector<std::vector<ImageProcessor::FieldType>>> ImageProcessor::hmtMaskMap = {
    {1, {
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {2, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {3, {
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE}
    }},
    {4, {
        {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
        {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {5, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {6, {
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
    }},
    {7, {
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
        {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
    }},
    {8, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
        {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
        {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
    }},
    {9, {
        {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {10, {
        {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
        {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
    }},
    {11, {
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
        {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
    }},
    {12, {
        {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
        {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
        {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
    }},
};

const std::unordered_map<int, std::vector<std::vector<ImageProcessor::FieldType>>> ImageProcessor::hmtComplementMaskMap = {
    {1, {
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {2, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
    }},
    {3, {
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK}
    }},
    {4, {
            {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
            {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {5, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
    }},
    {6, {
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
    }},
    {7, {
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
            {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
    }},
    {8, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
            {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
            {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
    }},
    {9, {
            {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
    }},
    {10, {
            {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
            {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
    }},
    {11, {
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
            {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
    }},
    {12, {
            {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
            {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
            {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
    }},
};

cv::Mat ImageProcessor::dilation(cv::Mat image, int maskNumber,
    std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    if (image.empty()) {
        throw std::invalid_argument("Image cannot be empty");
    }
    if (maskMapping == maskMap) {
        if (maskNumber < 1 || maskNumber > 10) {
            throw std::out_of_range("Mask number has to be between 1 and 10 inclusive for maskMap");
        }
    } else {
        if (maskNumber < 1 || maskNumber > 12) {
            throw std::out_of_range("Mask number has to be between 1 and 12 inclusive for hmtMaskMap");
        }
    }


    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMapping.find(maskNumber)->second;

    int markerX = 0, markerY = 0;
    bool foundMarker = false;
    for (int i = 0; i < mask.size() && !foundMarker; i++) {
        for (int j = 0; j < mask[i].size() && !foundMarker; j++) {
            if (mask[i][j] == FieldType::BLACK_MARKER ||
                mask[i][j] == FieldType::WHITE_MARKER) {
                markerX = i;
                markerY = j;
                foundMarker = true;
            }
        }
    }

#pragma omp parallel for collapse(2)
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            if (x + mask.size() > image.rows || y + mask[0].size() > image.cols) {
                continue;
            }

            bool shouldDilate = false;
            for (int i = 0; i < mask.size() && !shouldDilate; i++) {
                for (int j = 0; j < mask[i].size() && !shouldDilate; j++) {
                    if (mask[i][j] == FieldType::INACTIVE) {
                        continue;
                    }
                    if (mask[i][j] == FieldType::BLACK_MARKER ||
                        mask[i][j] == FieldType::BLACK) {
                        if (image.at<uchar>(x + j, y + i) == 0) {
                            shouldDilate = true;
                        }
                    }
                    else if (mask[i][j] == FieldType::WHITE_MARKER ||
                             mask[i][j] == FieldType::WHITE) {
                        if (image.at<uchar>(x + j, y + i) == 255) {
                            shouldDilate = true;
                        }
                    }
                }
            }

            result.at<uchar>(x + markerX, y + markerY) = shouldDilate ? 255 : 0;
        }
    }

    return result;
}

cv::Mat ImageProcessor::erosion(cv::Mat image, int maskNumber,
    std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    if (image.empty()) {
        throw std::invalid_argument("Image cannot be empty");
    }
    if (maskMapping == maskMap) {
        if (maskNumber < 1 || maskNumber > 10) {
            throw std::out_of_range("Mask number has to be between 1 and 10 inclusive for maskMap");
        }
    } else {
        if (maskNumber < 1 || maskNumber > 12) {
            throw std::out_of_range("Mask number has to be between 1 and 12 inclusive for hmtMaskMap");
        }
    }

    cv::Mat result = image.clone();
    std::vector<std::vector<FieldType>> mask = maskMapping.find(maskNumber)->second;

    int markerX = 0, markerY = 0;
    bool foundMarker = false;
    for (int i = 0; i < mask.size() && !foundMarker; i++) {
        for (int j = 0; j < mask[i].size() && !foundMarker; j++) {
            if (mask[i][j] == FieldType::BLACK_MARKER ||
                mask[i][j] == FieldType::WHITE_MARKER) {
                markerX = i;
                markerY = j;
                foundMarker = true;
            }
        }
    }

#pragma omp parallel for collapse(2)
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            if (x + mask.size() > image.rows || y + mask[0].size() > image.cols) {
                continue;
            }

            bool shouldErode = false;
            for (int i = 0; i < mask.size() && !shouldErode; i++) {
                for (int j = 0; j < mask[i].size() && !shouldErode; j++) {
                    if (mask[i][j] == FieldType::INACTIVE) {
                        continue;
                    }
                    if (mask[i][j] == FieldType::BLACK_MARKER ||
                        mask[i][j] == FieldType::BLACK) {
                        if (image.at<uchar>(x + i, y + j) == 255) {
                            shouldErode = true;
                        }
                    }
                    else if (mask[i][j] == FieldType::WHITE_MARKER ||
                             mask[i][j] == FieldType::WHITE) {
                        if (image.at<uchar>(x + i, y + j) == 0) {
                            shouldErode = true;
                        }
                    }
                }
            }

            result.at<uchar>(x + markerX, y + markerY) = shouldErode ? 0 : 255;
        }
    }

    return result;
}

cv::Mat ImageProcessor::opening(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat result = image.clone();
    result = this->erosion(result, maskNumber, maskMapping);
    result = this->dilation(result, maskNumber, maskMapping);
    return result;
}

cv::Mat ImageProcessor::closing(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat result = image.clone();
    result = this->dilation(result, maskNumber, maskMapping);
    result = this->erosion(result, maskNumber, maskMapping);
    return result;
}

cv::Mat ImageProcessor::hmt(cv::Mat image, int maskNumber, std::unordered_map<int, std::vector<std::vector<FieldType>>> maskMapping) {
    cv::Mat foregroundMatch = image.clone();
    cv::Mat backgroundMatch = image.clone();

    foregroundMatch = this->erosion(foregroundMatch, maskNumber, maskMapping);
    backgroundMatch = this->complement(backgroundMatch);

    backgroundMatch = this->erosion(backgroundMatch, maskNumber, hmtComplementMaskMap);

    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
#pragma omp parallel for collapse(2)
    for (int x = 0; x < foregroundMatch.rows; x++) {
        for (int y = 0; y < foregroundMatch.cols; y++) {
            if (foregroundMatch.at<uchar>(x, y) == UCHAR_MAX && backgroundMatch.at<uchar>(x, y) == UCHAR_MAX) {
                result.at<uchar>(x, y) = 255;
            }
        }
    }

    return result;
}

cv::Mat ImageProcessor::taskM4(cv::Mat image) {
    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    const int MAX_ITERATIONS = 10;

    for (int i = 1; i <= 4; i++) {
        cv::Mat currentElement = image.clone();
        cv::Mat nextElement;

        int iterations = 0;
        do {
            nextElement = hmt(currentElement, i);
            nextElement = imagesUnion(nextElement, image);

            if (areEqual(currentElement, nextElement)) {
                break;
            }

            currentElement = nextElement.clone();
            iterations++;
        } while (iterations < MAX_ITERATIONS);

        if (iterations >= MAX_ITERATIONS) {
            std::cout << "Warning: Maximum iterations reached for element " << i << std::endl;
        }

        result = imagesUnion(result, nextElement);
    }
    return result;
}

std::vector<cv::Vec3b> ImageProcessor::createColorMap() {
    std::vector<cv::Vec3b> colorMap(256, cv::Vec3b(0, 0, 0));
    for (int i = 1; i <= 255; ++i) {
        int hue = (i * 179) / 255;
        cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255, 255));
        cv::Mat rgb;
        cv::cvtColor(hsv, rgb, cv::COLOR_HSV2BGR);
        colorMap[i] = rgb.at<cv::Vec3b>(0, 0);
    }
    return colorMap;
}

cv::Mat ImageProcessor::applyColorMap(const cv::Mat &grayscaleMask, const std::vector<cv::Vec3b> &colorMap) {
    cv::Mat colorMask(grayscaleMask.size(), CV_8UC3);
    for (int x = 0; x < grayscaleMask.rows; ++x) {
        for (int y = 0; y < grayscaleMask.cols; ++y) {
            int grayValue = grayscaleMask.at<uchar>(x, y);
            colorMask.at<cv::Vec3b>(x, y) = colorMap[grayValue];
        }
    }
    return colorMask;
}

void ImageProcessor::visualizeFourier(cv::Mat fourierImage, const std::string& fourierVisPath) {
    int M = fourierImage.rows;
    int N = fourierImage.cols;

    cv::Mat magnitudeSpectrum = cv::Mat::zeros(M, N, CV_64FC1);
#pragma omp parallel for collapse(2)
    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            magnitudeSpectrum.at<double>(u, v) = std::log(1
            + std::sqrt(std::pow(fourierImage.at<cv::Vec2d>(u, v)[0], 2)
                + std::pow(fourierImage.at<cv::Vec2d>(u, v)[1], 2)));
        }
    }

    cv::Mat visualization = cv::Mat::zeros(M, N, CV_8UC1);

    double minVal = DBL_MAX;
    double maxVal = -DBL_MAX;

#pragma omp parallel for collapse(2) reduction(min:minVal) reduction(max:maxVal)
    for(int u = 0; u < M; u++) {
        for(int v = 0; v < N; v++) {
            double val = magnitudeSpectrum.at<double>(u, v);
            minVal = std::min(minVal, val);
            maxVal = std::max(maxVal, val);
        }
    }

#pragma omp parallel for collapse(2)
    for(int u = 0; u < M; u++) {
        for(int v = 0; v < N; v++) {
            double val = magnitudeSpectrum.at<double>(u, v);
            double scaledValue = (val - minVal) / (maxVal - minVal) * 255.0;
            visualization.at<uchar>(u, v) = static_cast<uchar>(scaledValue);
        }
    }

    imwrite(fourierVisPath, visualization);
}


cv::Mat ImageProcessor::fourierTransform(cv::Mat image, const std::string& fourierVisPath) {
    const int M = image.rows;
    const int N = image.cols;

    cv::Mat fourierImage = cv::Mat::zeros(M, N, CV_64FC2);

    std::vector<std::complex<double>> rowExps(M * M), colExps(N * N);
#pragma omp parallel for
    for (int u = 0; u < M; u++) {
        for (int x = 0; x < M; x++) {
            rowExps[u * M + x] = std::exp(std::complex<double>(0, -2.0 * std::numbers::pi * (static_cast<double>(u * x) / M)));
        }
    }
#pragma omp parallel for
    for (int v = 0; v < N; v++) {
        for (int y = 0; y < N; y++) {
            colExps[v * N + y] = std::exp(std::complex<double>(0, -2.0 * std::numbers::pi * (static_cast<double>(v * y) / N)));
        }
    }

#pragma omp parallel for
    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            std::complex<double> sum(0, 0);

            for (int x = 0; x < M; x++) {
                const auto factorX = rowExps[u * M + x];
                for (int y = 0; y < N; y++) {
                    sum += static_cast<double>(image.at<uchar>(x, y)) * factorX * colExps[v * N + y];
                }
            }

            fourierImage.at<cv::Vec2d>(u, v)[0] = sum.real();
            fourierImage.at<cv::Vec2d>(u, v)[1] = sum.imag();
        }
        std::cout << "Done: " << u << " u iterations." << "\n";
    }

#pragma omp parallel for
    for (int u = 0; u < M / 2; u++) {
        for (int v = 0; v < N / 2; v++) {
            cv::Vec2d temp = fourierImage.at<cv::Vec2d>(u, v);
            fourierImage.at<cv::Vec2d>(u, v) = fourierImage.at<cv::Vec2d>(u + M/2, v + N/2);
            fourierImage.at<cv::Vec2d>(u + M/2, v + N/2) = temp;

            temp = fourierImage.at<cv::Vec2d>(u + M/2, v);
            fourierImage.at<cv::Vec2d>(u + M/2, v) = fourierImage.at<cv::Vec2d>(u, v + N/2);
            fourierImage.at<cv::Vec2d>(u, v + N/2) = temp;
        }
    }

    visualizeFourier(fourierImage, fourierVisPath);

    return fourierImage;
}

void ImageProcessor::fft1D(cv::Mat row) {
    int n = row.cols;
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i < j) {
            std::swap(row.at<cv::Vec2d>(i)[0], row.at<cv::Vec2d>(j)[0]);
            std::swap(row.at<cv::Vec2d>(i)[1], row.at<cv::Vec2d>(j)[1]);
        }
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j |= bit;
    }

    for (int len = 2; len <= n; len <<= 1) {
        double theta = -2.0f * std::numbers::pi / len;
        std::complex<double> wlen(std::cos(theta), std::sin(theta));
        for (int i = 0; i < n; i += len) {
            std::complex<double> w(1, 0);
            for (int k = 0; k < len / 2; k++) {
                std::complex<double> u(row.at<cv::Vec2d>(i + k)[0], row.at<cv::Vec2d>(i + k)[1]);

                std::complex<double> t(row.at<cv::Vec2d>(i + k + len / 2)[0], row.at<cv::Vec2d>(i + k + len / 2)[1]);
                t *= w;

                std::complex<double> result1 = u + t;
                std::complex<double> result2 = u - t;

                row.at<cv::Vec2d>(i + k)[0] = result1.real();
                row.at<cv::Vec2d>(i + k)[1] = result1.imag();

                row.at<cv::Vec2d>(i + k + len / 2)[0] = result2.real();
                row.at<cv::Vec2d>(i + k + len / 2)[1] = result2.imag();
                w *= wlen;
            }
        }
    }
}


cv::Mat ImageProcessor::fastFourierTransform(cv::Mat image, const std::string &fourierVisPath) {
    const int M = image.rows;
    const int N = image.cols;

    cv::Mat fourierImage = cv::Mat::zeros(M, N, CV_64FC2);

#pragma omp parallel for collapse(2)
    for (int x = 0; x < M; x++) {
        for (int y = 0; y < N; y++) {
            fourierImage.at<cv::Vec2d>(x, y)[0] = image.at<uchar>(x, y);
        }
    }

#pragma omp parallel
    {
#pragma omp for
        for (int x = 0; x < M; x++) {
            cv::Mat row = cv::Mat::zeros(1, N, CV_64FC2);
            for (int y = 0; y < N; y++) {
                row.at<cv::Vec2d>(y)[0] = fourierImage.at<cv::Vec2d>(x, y)[0];
            }
            fft1D(row);
            for (int y = 0; y < N; y++) {
                fourierImage.at<cv::Vec2d>(x, y)[0] = row.at<cv::Vec2d>(y)[0];
                fourierImage.at<cv::Vec2d>(x, y)[1] = row.at<cv::Vec2d>(y)[1];
            }
        }
    }

#pragma omp parallel
    {
#pragma omp for
        for (int y = 0; y < N; y++) {
            cv::Mat col = cv::Mat::zeros(1, M, CV_64FC2);
            for (int x = 0; x < M; x++) {
                col.at<cv::Vec2d>(y)[0] = fourierImage.at<cv::Vec2d>(x, y)[0];
            }
            fft1D(col);
            for (int x = 0; x < M; x++) {
                fourierImage.at<cv::Vec2d>(x, y)[0] = col.at<cv::Vec2d>(y)[0];
                fourierImage.at<cv::Vec2d>(x, y)[1] = col.at<cv::Vec2d>(y)[1];
            }
        }
    }

#pragma omp parallel for
    for (int u = 0; u < M / 2; u++) {
        for (int v = 0; v < N / 2; v++) {
            cv::Vec2d temp = fourierImage.at<cv::Vec2d>(u, v);
            fourierImage.at<cv::Vec2d>(u, v) = fourierImage.at<cv::Vec2d>(u + M/2, v + N/2);
            fourierImage.at<cv::Vec2d>(u + M/2, v + N/2) = temp;

            temp = fourierImage.at<cv::Vec2d>(u + M/2, v);
            fourierImage.at<cv::Vec2d>(u + M/2, v) = fourierImage.at<cv::Vec2d>(u, v + N/2);
            fourierImage.at<cv::Vec2d>(u, v + N/2) = temp;
        }
    }

    visualizeFourier(fourierImage, fourierVisPath);

    return fourierImage;
}

