//
// Created by gluckasz on 12/7/24.
//

#include "../include/ImageProcessor.h"




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

void ImageProcessor::FFT1D(cv::Mat row) {
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

void ImageProcessor::iFFT1D(cv::Mat row) {
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
        double theta = 2.0f * std::numbers::pi / len;
        std::complex<double> wlen(std::cos(theta), std::sin(theta));
        for (int i = 0; i < n; i += len) {
            std::complex<double> w(1, 0);
            for (int k = 0; k < len / 2; k++) {
                std::complex<double> u(row.at<cv::Vec2d>(i + k)[0], row.at<cv::Vec2d>(i + k)[1]);

                std::complex<double> t(row.at<cv::Vec2d>(i + k + len / 2)[0], row.at<cv::Vec2d>(i + k + len / 2)[1]);
                t *= w;

                std::complex<double> result1 = (u + t);
                std::complex<double> result2 = (u - t);

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
            row.at<cv::Vec2d>(0, y)[0] = fourierImage.at<cv::Vec2d>(x, y)[0];
        }
        FFT1D(row);
        for (int y = 0; y < N; y++) {
            fourierImage.at<cv::Vec2d>(x, y)[0] = row.at<cv::Vec2d>(0, y)[0];
            fourierImage.at<cv::Vec2d>(x, y)[1] = row.at<cv::Vec2d>(0, y)[1];
        }
    }
}

#pragma omp parallel
{
#pragma omp for
    for (int y = 0; y < N; y++) {
        cv::Mat col = cv::Mat::zeros(1, M, CV_64FC2);
        for (int x = 0; x < M; x++) {
            col.at<cv::Vec2d>(0, x)[0] = fourierImage.at<cv::Vec2d>(x, y)[0];
            col.at<cv::Vec2d>(0, x)[1] = fourierImage.at<cv::Vec2d>(x, y)[1];
        }
        FFT1D(col);
        for (int x = 0; x < M; x++) {
            fourierImage.at<cv::Vec2d>(x, y)[0] = col.at<cv::Vec2d>(0, x)[0];
            fourierImage.at<cv::Vec2d>(x, y)[1] = col.at<cv::Vec2d>(0, x)[1];
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

cv::Mat ImageProcessor::fftLowPass(cv::Mat fourierImage, int lowPassBandSize, const std::string& fourierVisPath) {
    cv::Mat result = fourierImage.clone();
    for (int x = 0; x < fourierImage.rows; x++) {
        for (int y = 0; y < fourierImage.cols; y++) {
            if (sqrt(pow(fourierImage.rows / 2 - x, 2) + pow(fourierImage.cols / 2 - y, 2)) >= lowPassBandSize) {
                result.at<cv::Vec2d>(x, y) = 0;
            }
        }
    }

    visualizeFourier(result, fourierVisPath);

    return result;
}

cv::Mat ImageProcessor::fftHighPass(cv::Mat fourierImage, int highPassBandSize, const std::string &fourierVisPath) {
    cv::Mat result = fourierImage.clone();
    std::complex<double> DCComponent(
        fourierImage.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[0],
        fourierImage.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[1]
        );
    for (int x = 0; x < fourierImage.rows; x++) {
        for (int y = 0; y < fourierImage.cols; y++) {
            if (sqrt(pow(fourierImage.rows / 2 - x, 2) + pow(fourierImage.cols / 2 - y, 2)) <= highPassBandSize) {
                result.at<cv::Vec2d>(x, y) = 0;
            }
        }
    }

    result.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[0] = DCComponent.real();
    result.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[1] = DCComponent.imag();

    visualizeFourier(result, fourierVisPath);

    return result;
}

cv::Mat ImageProcessor::fftBandPass(cv::Mat fourierImage, int lowCut, int highCut, const std::string &fourierVisPath) {
    cv::Mat result = fourierImage.clone();
    result = fftHighPass(result, lowCut, fourierVisPath);
    result = fftLowPass(result, highCut, fourierVisPath);

    visualizeFourier(result, fourierVisPath);

    return result;
}

cv::Mat ImageProcessor::fftBandCut(cv::Mat fourierImage, int lowPass, int highPass, const std::string &fourierVisPath) {
    cv::Mat result = fourierImage.clone();
    std::complex<double> DCComponent(
        fourierImage.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[0],
        fourierImage.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[1]
        );
    for (int x = 0; x < fourierImage.rows; x++) {
        for (int y = 0; y < fourierImage.cols; y++) {
            double distance = sqrt(pow(fourierImage.rows / 2 - x, 2) + pow(fourierImage.cols / 2 - y, 2));
            if (distance >= lowPass && distance <= highPass) {
                result.at<cv::Vec2d>(x, y) = 0;
            }
        }
    }

    result.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[0] = DCComponent.real();
    result.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[1] = DCComponent.imag();

    visualizeFourier(result, fourierVisPath);

    return result;
}

cv::Mat ImageProcessor::fftHighPassDirection(cv::Mat fourierImage, cv::Mat mask, const std::string &fourierVisPath) {
    cv::Mat result = fourierImage.clone();
    std::complex<double> DCComponent(
        fourierImage.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[0],
        fourierImage.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[1]
        );
    for (int x = 0; x < fourierImage.rows; x++) {
        for (int y = 0; y < fourierImage.cols; y++) {
            if (mask.at<uchar>(x, y) == 0) {
                result.at<cv::Vec2d>(x, y) = 0;
            }
        }
    }

    result.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[0] = DCComponent.real();
    result.at<cv::Vec2d>(fourierImage.rows / 2, fourierImage.cols / 2)[1] = DCComponent.imag();

    visualizeFourier(result, fourierVisPath);

    return result;
}

cv::Mat ImageProcessor::fftPhaseModifying(cv::Mat fourierImage, int k, int l, const std::string &fourierVisPath) {
    cv::Mat mask = cv::Mat::zeros(fourierImage.rows, fourierImage.cols, CV_64FC2);
    for (int n = 0; n < fourierImage.rows; n++) {
        for (int m = 0; m < fourierImage.cols; m++) {
            std::complex<double> complexResult = exp(std::complex<double>(
                0,
                -n * k * 2 * std::numbers::pi / fourierImage.rows
                + -m * l * 2 * std::numbers::pi / fourierImage.cols
                + (k + l) * std::numbers::pi
                ));
            mask.at<cv::Vec2d>(n, m)[0] = complexResult.real();
            mask.at<cv::Vec2d>(n, m)[1] = complexResult.imag();
        }
    }

    cv::Mat result = fourierImage.clone();
    for (int n = 0; n < fourierImage.rows; n++) {
        for (int m = 0; m < fourierImage.cols; m++) {
            double a = fourierImage.at<cv::Vec2d>(n, m)[0];
            double b = fourierImage.at<cv::Vec2d>(n, m)[1];
            double c = mask.at<cv::Vec2d>(n, m)[0];
            double d = mask.at<cv::Vec2d>(n, m)[1];

            result.at<cv::Vec2d>(n, m)[0] = a*c - b*d;
            result.at<cv::Vec2d>(n, m)[1] = a*d + b*c;
        }
    }

    visualizeFourier(result, fourierVisPath);

    return result;
}

