#include "listutils.h"
#include <fstream>
#include <stdexcept>

std::string ListUtils::EscapeData(const std::string& data) {
    std::string result;
    result.reserve(data.size());

    for (char c : data) {
        if      (c == '\\') result += "\\\\";
        else if (c == '\n') result += "\\n";
        else if (c == '\r') result += "\\r";
        else                result += c;
    }
    return result;
}

std::string ListUtils::UnescapeData(const std::string& data) {
    std::string result;
    result.reserve(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == '\\' && i + 1 < data.size()) {
            ++i;
            if      (data[i] == '\\') result += '\\';
            else if (data[i] == 'n')  result += '\n';
            else if (data[i] == 'r')  result += '\r';
            else {
                result += '\\';
                result += data[i];
            }
        } else {
            result += data[i];
        }
    }
    return result;
}

bool ListUtils::CompareTextFiles(const std::string& fileA, const std::string& fileB, std::ostream& report)
{
    std::ifstream finA(fileA);
    if (!finA.is_open())
        throw std::runtime_error("Cannot open file for comparison: " + fileA);

    std::ifstream finB(fileB);
    if (!finB.is_open())
        throw std::runtime_error("Cannot open file for comparison: " + fileB);

    bool equal = true;
    size_t lineNum = 0;
    std::string lineA, lineB;

    while (true) {
        bool gotA = static_cast<bool>(std::getline(finA, lineA));
        bool gotB = static_cast<bool>(std::getline(finB, lineB));

        if (!gotA && !gotB) break;

        ++lineNum;

        if (gotA != gotB) {
            report << "Line " << lineNum << ": files have different number of lines\n";
            equal = false;
            break;
        }

        if (lineA != lineB) {
            report << "Line " << lineNum << " differs:\n"
                   << "  [" << fileA << "]: " << lineA << '\n'
                   << "  [" << fileB << "]: " << lineB << '\n';
            equal = false;
        }
    }

    if (equal)
        report << "Files are identical (" << lineNum << " lines)\n";

    return equal;
}
