#include "../include/InputProcessor.h"
//TODO correct linear contrast in RGB
int main(int argc, char** argv) {
    InputProcessor inputProcessor(argc, argv);;
    inputProcessor.processInput();
    inputProcessor.processImage();
    return 0;
}



