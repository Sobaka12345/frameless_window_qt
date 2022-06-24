cmake_minimum_required(VERSION 3.5.17)

project(frameless_window_qt VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/configure_conan.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

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

add_executable(frameless_window_qt
    ${PROJECT_SOURCES}
)

target_link_libraries(frameless_window_qt PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)