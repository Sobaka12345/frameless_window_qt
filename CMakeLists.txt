cmake_minimum_required(VERSION 3.17)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_BINARY_OUTPUT ${CMAKE_BINARY_DIR}/bin CACHE PATH "Compiled Binaries")
set(PROJECT_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib CACHE PATH "Compiled Libraries")

include(${CMAKE_SOURCE_DIR}/PrecompiledHeader.cmake)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

if(WIN32)
	set(PROJECT_PLATFORM_SOURCES
		frameless_window/frameless_window_windows.hpp
		frameless_window/frameless_window_windows.cpp)
ELSEIF(UNIX)
    set(PROJECT_PLATFORM_SOURCES
		frameless_window/frameless_window_linux.hpp
		frameless_window/frameless_window_linux.cpp)
ENDIF()

set(PROJECT_SOURCES
	main.cpp
	${PROJECT_PLATFORM_SOURCES}
	frameless_window/iframeless_window_native.hpp
	frameless_window/frameless_window.hpp
	caption_bar/icaption_bar.h
	caption_bar/caption_bar.h
	caption_bar/caption_bar.cpp
	main_window.cpp
	main_window.h
	ui/caption_bar.ui
	ui/main_window.ui
	resources/resources.qrc
	pchheader.h
	pchheader.cpp
)

add_executable(frameless_window_qt
    ${PROJECT_SOURCES}
)

add_precompiled_header(${PROJECT_NAME} pchheader.h SOURCE_CXX pchheader.cpp FORCEINCLUDE)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_LIBRARY_OUTPUT}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_LIBRARY_OUTPUT}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_OUTPUT}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_OUTPUT}"
)