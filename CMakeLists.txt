cmake_minimum_required(VERSION 3.5)

project(frameless_window_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(WIN32)
	set(PROJECT_PLATFORM_SOURCES
		frameless_window_windows.hpp
		frameless_window_windows.cpp)
ELSEIF(UNIX)
    set(PROJECT_PLATFORM_SOURCES
		frameless_window_linux.hpp
		frameless_window_linux.cpp)
ENDIF()

set(PROJECT_SOURCES
	main.cpp
	${PROJECT_PLATFORM_SOURCES}
	icaption_bar.h
	iframeless_window_native.hpp
	frameless_window.hpp
	caption_bar.h
	caption_bar.cpp
	main_window.cpp
	main_window.h
	ui/caption_bar.ui
	ui/main_window.ui
	resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(frameless_window_qt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET frameless_window_qt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(frameless_window_qt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(frameless_window_qt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(frameless_window_qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(frameless_window_qt PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(frameless_window_qt)
endif()
