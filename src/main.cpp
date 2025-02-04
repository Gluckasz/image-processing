#include "../include/input-processing-lib/InputProcessor.h"
int main(int argc, char** argv) {
    InputProcessor inputProcessor(argc, argv);;
    inputProcessor.processInput();
    inputProcessor.processImage();
    return 0;
}



