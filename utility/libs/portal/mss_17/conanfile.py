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
    name = "mss17"
    version = get_version()
    license = "custom"
    description = "SQL Server 2017 ODBC Support by BBD"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports = "__BUILD_VERSION__.txt"
    exports_sources = "include/*", "src/*", "mss/*", "CMakeLists.txt"
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
        self.copy("mss/*", dst=".")
        self.copy("include/*", dst=".")

    def package_info(self):
        self.cpp_info.libs.append(self.name)
        self.cpp_info.defines.append("MSS17API")
        if self.settings.os != "Windows":
            self.cpp_info.system_libs.append("odbc")
            self.cpp_info.includedirs.append("mss/linux/include")
        else:
            self.cpp_info.system_libs.append("odbc32")
            self.cpp_info.system_libs.append("winmm")
            self.cpp_info.libdirs.append("mss/win/lib/x64" if self.settings.arch == "x86_64" else "mss/win/lib/x86")
            self.cpp_info.libs.append("msodbcsql17.lib")
            self.cpp_info.includedirs.append("mss/win/include")