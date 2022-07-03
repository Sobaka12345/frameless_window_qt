if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES qt/6.0.4
                      GENERATORS cmake_find_package
					  GENERATORS cmake_paths
                      IMPORTS "bin, *.dll -> ${PROJECT_BINARY_OUTPUT}/"
                      IMPORTS "res/archdatadir/plugins, * -> ${PROJECT_BINARY_OUTPUT}/"
                      OPTIONS qt:shared=True)

SET(PROFILE_PATH "AUTOMATIC" CACHE FILEPATH "Path to conan profile file")

if(NOT ${PROFILE_PATH} STREQUAL AUTOMATIC)
   message("Using profile file: ${PROFILE_PATH}")
   conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_BINARY_DIR}/
                       REMOTE conancenter
                       BUILD missing
                       PROFILE ${PROFILE_PATH})
else()
   message("Using automatically detected profile.")
   conan_cmake_autodetect(CONAN_SETTINGS)
   conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_BINARY_DIR}/
                       REMOTE conancenter
                       BUILD missing
                       SETTINGS ${CONAN_SETTINGS})
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
