#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], IMREAD_GRAYSCALE );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    for (int x = 0; x < image.rows; x ++) {
        for (int y = 0; y < image.cols; y++) {
            if (image.at<uchar>(x, y) <= UCHAR_MAX - 10) {
                image.at<uchar>(x, y) += 100;
            }
        }
    }

    if (!std::filesystem::is_directory("output") || !std::filesystem::exists("output")) {
        std::filesystem::create_directory("output");
    }
    imwrite("output/output2.bmp", image);
    return 0;
}
