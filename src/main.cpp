#include "../include/InputProcessor.h"
// TODO implement copyto method, optimize space complexity.
int main(int argc, char** argv) {
    InputProcessor inputProcessor(argc, argv);;
    inputProcessor.processInput();
    inputProcessor.processImage();
    return 0;
}



