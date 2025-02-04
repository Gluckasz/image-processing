//
// Created by gluckasz on 2/3/25.
//

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include "CommandOptions.h"


class CommandParser {
    /**
     * @brief Template function to read and validate numeric parameters
     * @tparam T Parameter type (int or float)
     */
    template<typename T>
    static bool readParam(const std::string &arg, const std::string &prefix, std::optional<T> &result,
                   const std::string &errorMsg);
public:
    static CommandOptions parse(int argc, char** argv);
};



#endif //COMMANDPARSER_H
