#include "../include/input-processing-lib/InputProcessor.h"
#include "input-processing-lib/CommandParser.h"

int main(int argc, char** argv) {
    auto options = CommandParser::parse(argc, argv);
    InputProcessor processor(options, argv[INPUT_IMAGE_POS]);
    processor.process();
    return 0;
}



