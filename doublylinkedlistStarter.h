#ifndef DOUBLYLINKEDLISTSTARTER_H
#define DOUBLYLINKEDLISTSTARTER_H

#include "doublylinkedlistrandaccess.h"
#include "doublylinkedlistserializer.h"
#include "listutils.h"
#include <iostream>
#include <string>

const std::string FILENAMEIN         = "data/inlet.in";
const std::string FILENAMEOUT        = "outlet.out";
const std::string FILENAMEDESERIALIZE = "deserializeTest.in";

enum UserChoice {
    LOADTEXTANDSERIALIZECASE = 0,
    DESERIALIZECASE          = 1,
    EXIT                     = 2
};

void appStarter() {
    DoublyLinkedListRandAccess testList{};
    DoublyLinkedListRandAccess testListRestored{};

    std::cout << "Enter 0 to load data from inlet.in and serialize it to outlet.out\n"
              << "Enter 1 to deserialize from outlet.out and write it to deserializeTest.in\n"
              << "Enter any digit or character to leave\n"
              << "\nChoice: ";

    int input = 0;
    while (input != EXIT && std::cin >> input) {
        switch (static_cast<UserChoice>(input)) {
        case LOADTEXTANDSERIALIZECASE: {
            DoublyLinkedListSerializer::LoadFromText(testList, FILENAMEIN);
            DoublyLinkedListSerializer::Serialize(testList, FILENAMEOUT);
            std::cout << "Done: loaded from " << FILENAMEIN
                      << " and serialized to " << FILENAMEOUT << "\n";
            break;
        }
        case DESERIALIZECASE: {
            DoublyLinkedListSerializer::Deserialize(testListRestored, FILENAMEOUT);
            DoublyLinkedListSerializer::SaveToText(testListRestored, FILENAMEDESERIALIZE);
            ListUtils::CompareTextFiles(FILENAMEIN, FILENAMEDESERIALIZE);
            break;
        }
        default:
            input = EXIT;
            break;
        }

        if (input != EXIT) {
            std::cout << "\nChoice: ";
        }
    }
}

#endif // DOUBLYLINKEDLISTSTARTER_H
