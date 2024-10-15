#include "../include/InputProcessor.h"

int main(int argc, char** argv) {
    InputProcessor inputProcessor(argc, argv);;
    inputProcessor.processInput();
    inputProcessor.processImage();
    return 0;
}



