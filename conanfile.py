import os
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMake, cmake_layout


class Game2DByPower(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    default_options = {"glad*:gl_profile": "core", "glad*:gl_version": "4.4"}

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("imgui/1.92.5")
        self.requires("glad/0.1.36")
        self.requires("glm/1.0.1")

        if self.settings.os == "Linux":
            self.default_options["glad*:with_wayland"] = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()

    def layout(self):
        cmake_layout(self)

    def generate(self):
        copy(
            self,
            "*glfw*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
            os.path.join(self.source_folder, "extern", "imgui", "bindings"),
        )

        copy(
            self,
            "*opengl3*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
            os.path.join(self.source_folder, "extern", "imgui", "bindings"),
        )

        copy(
            self,
            "*stdlib*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "misc", "cpp"),
            os.path.join(self.source_folder, "extern", "imgui", "stdlib"),
        )
