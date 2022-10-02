if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES qt/6.3.1
                      GENERATORS cmake_find_package
					       GENERATORS cmake_paths
                      IMPORTS "bin, *.dll -> ${PROJECT_BINARY_OUTPUT}/"
                      IMPORTS "res/archdatadir/plugins, * -> ${PROJECT_BINARY_OUTPUT}/"
                      OPTIONS qt:shared=True
                              qt:gui=True
                              qt:multiconfiguration=False
                              qt:opengl=desktop
                              qt:openssl=True
                              qt:qt5compat=False
                              qt:qtdeclarative=False
                              qt:qtdoc=False
                              qt:qtquick3d=False
                              qt:qtquicktimeline=False
                              qt:qtshadertools=False
                              qt:qtsvg=False
                              qt:qttools=False
                              qt:qttranslations=False
                              qt:qtwayland=False
                              qt:shared=False
                              qt:widgets=True
                              qt:with_brotli=True
                              qt:with_dbus=False
                              qt:with_doubleconversion=True
                              qt:with_fontconfig=True
                              qt:with_freetype=True
                              qt:with_glib=False
                              qt:with_gssapi=False
                              qt:with_harfbuzz=True
                              qt:with_icu=True
                              qt:with_libjpeg=False
                              qt:with_libpng=True
                              qt:with_md4c=False
                              qt:with_mysql=False
                              qt:with_odbc=False
                              qt:with_pcre2=True
                              qt:with_pq=False
                              qt:with_sqlite3=False
                              qt:with_vulkan=False
                              qt:with_zstd=False)

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
