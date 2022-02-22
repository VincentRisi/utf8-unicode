from conans import ConanFile, CMake
from conans.tools import load

import os


def get_version():
    try:
        folder = os.path.dirname(os.path.abspath(__file__))
        filename = os.path.join(folder, "../../../__BUILD_VERSION__.txt")
        version = load(filename)
        return version
    except Exception as e:
        return None


class MainConan(ConanFile):
    name = "odbc_general"
    version = get_version()
    license = "custom"
    description = "General ODBC Support by BBD"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports = "__BUILD_VERSION__.txt"
    exports_sources = "include/*", "src/*", "CMakeLists.txt"
    requires = (
        ("libmain/[1.x.x]@bbdvanguard/stable"),
        ("bbdcmake/[1.x.x]@bbdvanguard/stable"),
    )

    def build(self): 
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("include/*", dst=".")

    def package_info(self):
        self.cpp_info.libs.append(self.name)
        if self.settings.os != "Windows":
            self.cpp_info.system_libs.append("odbc")
        else:
            self.cpp_info.system_libs.append("odbc32")
            self.cpp_info.system_libs.append("winmm")
