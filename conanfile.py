from os import path
from conans import ConanFile, CMake

class FramelessWindowQt(ConanFile):
    binary_dir = "bin"
    library_dir = "lib"
    settings = "os", "compiler", "build_type", "arch"
    requires = ["qt/6.3.1"]
    generators = ["cmake_find_package", "cmake_paths"]
    default_options = '''
        qt:shared=True
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
        qt:with_zstd=False
    '''

    def imports(self):
        if self.settings.os == "Linux":
            self.copy("*.so", src="bin", dst=path.join(self.install_folder, self.binary_dir))
        else:
            self.copy("*.dll", src="bin", dst=path.join(self.install_folder, self.binary_dir))
        self.copy("*", src="res/archdatadir/plugins", dst=path.join(self.install_folder, self.binary_dir))

    def generate(self):
        cmake = CMake(self)
        cmake.definitions["PROJECT_BINARY_OUTPUT"] = path.join(self.install_folder, self.binary_dir)
        cmake.definitions["PROJECT_LIBRARY_OUTPUT"] = path.join(self.install_folder, self.library_dir)
        cmake.configure(build_folder=self.install_folder)

    def build(self):
        cmake = CMake(self)
        cmake.build()
