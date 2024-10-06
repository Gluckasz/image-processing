#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

#include "../include/GrayscaleImageProcessor.h"
#include "../include/ImageProcessor.h"

using namespace cv;
using namespace std;

// Constants
constexpr int INPUT_IMAGE_POS = 1;

// Print available functions in command-line
void printCommands() {
    cout << "Available commands:\n"
    << "--help - print list of all the available commands with detailed description of their arguments.\n"
    << "--output [-outputFileName=value] - provide output file name.\n"
    << "\t -outputFileName - output file name (default is output.bmp).\n"
    << "--grayscale - read image in grayscale.\n"
    << "--brightness [-modVal=value] - modify brightness of an image.\n"
    << "\t -modVal - integral value to add to each pixel (can be negative).\n";
}

// Command-line input variables
string outputFileName = "output.bmp";
ImreadModes imreadMode = IMREAD_COLOR;
bool isBrightnessModified = false;
int brightnessModVal;

// Process user input from command-line
void processInput(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (static_cast<string>(argv[i]) == "--help") {
            printCommands();
        }
        else if (static_cast<string>(argv[i]) == "--output") {
            string paramVal = argv[++i];
            if (paramVal.substr(0, 16) != "-outputFileName=") {
                cout << "The --output parameter has to be of format: -outputFileName=[value]. Skipping outputFileName modification.";
            }
            else {
                outputFileName = paramVal.substr(paramVal.find('=') + 1);
            }
        }
        else if (static_cast<string>(argv[i]) == "--grayscale") {
            imreadMode = IMREAD_GRAYSCALE;
        }
        else if (static_cast<string>(argv[i]) == "--brightness") {
            isBrightnessModified = true;
            string paramVal = argv[++i];
            if (paramVal.substr(0, 8) != "-modVal=") {
                cout << "The --brightness parameter has to be of format: -modVal=[value]. Skipping brightness modification.";
                isBrightnessModified = false;
            }
            try {
                brightnessModVal = stoi(paramVal.substr(paramVal.find('=') + 1));
            }
            catch (const invalid_argument & e) {
                cout << "-modVal has to be an integer. Skipping brightness modification.";
                isBrightnessModified = false;
            }
            catch (const out_of_range & e) {
                cout << "-modVal is out of bounds. Skipping brightness modification.";
                isBrightnessModified = false;
            }
        }
    }
}

void saveImage(Mat image) {
    if (!filesystem::is_directory("output") || !filesystem::exists("output")) {
        filesystem::create_directory("output");
    }
    imwrite("output/" + outputFileName, image);
}

int main(int argc, char** argv) {
    processInput(argc, argv);
    Mat image = imread(argv[INPUT_IMAGE_POS], imreadMode);

    if (!image.data) {
        cout << "No image data \n";
        return -1;
    }

    unique_ptr<ImageProcessor> imageProcessor;
    if (imreadMode == IMREAD_COLOR) {
        // TODO
        return 0;
    }
    else {
        imageProcessor = make_unique<GrayscaleImageProcessor>(
            image,
            isBrightnessModified,
            brightnessModVal
            );
    }

    if (isBrightnessModified) {
        imageProcessor->modifyBrightness();
    }
    saveImage(image);
    return 0;
}



