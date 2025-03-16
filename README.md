# Image Processing CLI App

A command-line tool for image processing, designed for academic purposes. This application leverages OpenCV for image I/O and implements all other image processing operations from scratch in C++. The Google Test framework is used for testing.


## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Testing](#testing)
- [License](#license)


## Features

- **Custom Image Processing Algorithms:** All image processing operations are implemented from scratch.
- **OpenCV Integration:** Utilizes OpenCV for reading and writing image files.
- **OpenMP Parallelization:** Leverages OpenMP to speed up compute-intensive tasks.
- **Command-Line Interface:** Easy-to-use CLI for processing images.
- **Automated Testing:** Comprehensive test suite using Google Test.


## Prerequisites

- **C++ Compiler:** C++20 (or higher) with OpenMP support
- **OpenCV:** Version 4.10.0 (or higher)
- **Google Test:** Version 1.14.0 (or higher)- included as a subdirectory


## Installation

### From Source 

1. **Clone the Repository**
2. cd image_processing
3. cmake .
4. make

   
## Usage

To use this app first install it using installation instructions. Then call the built file in the command-line like this:
```
/path/to/built/app /path/to/image/to/process --arguments
```
To see availible commands use the argument:
```
--help
```
Output images produced by the app will be in the build/output directory.

## Testing
This project includes a suite of tests built with Google Test (gtest) to ensure all functionalities work as expected.

### Building Tests
The tests are built along with the application when you run CMake.

### Running Tests
To execute the test suite, run the test executable directly:

```
./image_processing_tests
```

Alternatively, you can use CTest:
```
ctest
```
Any test failures will be reported in the output.


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
