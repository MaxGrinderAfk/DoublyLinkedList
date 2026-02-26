#ifndef DOUBLYLINKEDLISTSERIALIZER_H
#define DOUBLYLINKEDLISTSERIALIZER_H

#include "doublylinkedlistrandaccess.h"
#include <string>
#include <fstream>

class DoublyLinkedListSerializer {
private:
    template <typename T>
    static void ReadBinary(std::ifstream& fin, T& value);

public:
    DoublyLinkedListSerializer() = delete;

    static void LoadFromText(DoublyLinkedListRandAccess& list, const std::string& filename);
    static void SaveToText(const DoublyLinkedListRandAccess& list, const std::string& filename);
    static void Serialize(const DoublyLinkedListRandAccess& list, const std::string& filename);
    static void Deserialize(DoublyLinkedListRandAccess& list, const std::string& filename);
};

#endif // DOUBLYLINKEDLISTSERIALIZER_H
