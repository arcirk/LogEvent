cmake_minimum_required(VERSION 3.5)

project(LogEvent VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

SET(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/x64/)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql REQUIRED)
#find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        dialogoptions.cpp
        dialogoptions.h
        dialogoptions.ui
        dialogselectcolumn.cpp
        dialogselectcolumn.h
        dialogselectcolumn.ui
        dialogselectinfobase.cpp
        dialogselectinfobase.h
        dialogselectinfobase.ui
        infobases.cpp
        infobases.h
        querybuilder.cpp
        querybuilder.h
        resurce.qrc
        settings.cpp
        settings.h
        dialogsqlfilter.cpp
        dialogsqlfilter.h
        dialogsqlfilter.ui
        dialogselectitemsfilter.cpp
        dialogselectitemsfilter.h
        dialogselectitemsfilter.ui
        dialogabout.cpp
        dialogabout.h
        dialogabout.ui
        dialogselectedrow.cpp
        dialogselectedrow.h
        dialogselectedrow.ui
        dialogsavefilter.cpp
        dialogsavefilter.h
        dialogsavefilter.ui
        filteritem.cpp
        filteritem.h
        filtermanager.cpp
        filtermanager.h
        selectiondialog.cpp
        selectiondialog.h
        selectiondialog.ui
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LogEvent
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LogEvent SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LogEvent
                ${PROJECT_SOURCES}
                )
    endif()
endif()

target_link_libraries(LogEvent PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Core
        )

set_target_properties(LogEvent PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LogEvent)
endif()