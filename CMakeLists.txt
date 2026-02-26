cmake_minimum_required(VERSION 3.16)

project(task1Intership LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task1Intership main.cpp
    doublylinkedlistrandaccess.h doublylinkedlistrandaccess.cpp
    doublylinkedlistserializer.h doublylinkedlistserializer.cpp

    listutils.h listutils.cpp
    doublylinkedlistStarter.h)

include(GNUInstallDirs)
install(TARGETS task1Intership
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
