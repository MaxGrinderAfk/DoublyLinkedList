#ifndef LISTUTILS_H
#define LISTUTILS_H

#include <string>
#include <ostream>
#include <iostream>

class ListUtils {
public:
    ListUtils() = delete;

    static std::string EscapeData(const std::string& data);
    static std::string UnescapeData(const std::string& data);
    static bool CompareTextFiles(const std::string& fileA, const std::string& fileB, std::ostream& report = std::cout);
};

#endif // LISTUTILS_H
