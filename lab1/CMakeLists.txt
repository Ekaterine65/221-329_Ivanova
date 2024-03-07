cmake_minimum_required(VERSION 3.5)

project(crypto VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        credentialwidget.cpp
        credentialwidget.h
        credentialwidget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(crypto
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET crypto APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(crypto SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(crypto
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


#include(FindOpenSSL) # средствами CMake автоматически найти установленную OpenSSL на данной машине, если это возможно
#target_link_libraries(crypto PRIVATE ${CMAKE_DL_LIBS} OpenSSL::SSL OpenSSL::Crypto)

target_include_directories(
    crypto
    PRIVATE
    C:/Qt/Tools/OpenSSLv3/Win_x64/include)
target_link_libraries(
    crypto
    PRIVATE
    C:/Qt/Tools/OpenSSLv3/Win_x64/lib/libcrypto.lib)

set_target_properties(crypto PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS crypto
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(crypto)
endif()

configure_file(
    credentials_en.json
    credentials_en.json
)
