#include "../include/InputProcessor.h"
// TODO ImageProcessor methods should return image objects.
int main(int argc, char** argv) {
    InputProcessor inputProcessor(argc, argv);;
    inputProcessor.processInput();
    inputProcessor.processImage();
    return 0;
}



