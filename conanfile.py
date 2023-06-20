from os import path
from conans import ConanFile, CMake

class FramelessWindowQt(ConanFile):
    binary_dir = "bin"
    library_dir = "lib"
    settings = "os", "compiler", "build_type", "arch"
    requires = ["qt/6.3.1"]
    generators = ["cmake_find_package", "cmake_paths"]

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
