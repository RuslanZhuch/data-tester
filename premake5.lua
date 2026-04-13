workspace "DataTester"
    architecture "x86_64"
    startproject "Tests"

    configurations { "Debug", "Release" }

    flags { "MultiProcessorCompile" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- -----------------------------------------------------------------------
    -- Vendor: GoogleTest
    -- -----------------------------------------------------------------------
    project "GoogleTest"
        location "libs/googletest"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
        warnings "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "libs/googletest/googletest/src/gtest-all.cc",
        }

        includedirs {
            "libs/googletest/googletest",
            "libs/googletest/googletest/include",
        }

        filter "system:windows"
            systemversion "latest"
            defines { "PLATFORM_WINDOWS" }

        filter "system:linux"
            defines { "PLATFORM_LINUX" }
            buildoptions { "-pthread" }

        filter "system:macosx"
            defines { "PLATFORM_MACOS" }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter {}

    -- -----------------------------------------------------------------------
    -- Library: data-tester  (main deliverable)
    -- -----------------------------------------------------------------------
    project "data-tester"
        location "data-tester"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
        warnings "Extra"
        flags { "FatalWarnings" }

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "data-tester/include/**.h",
            "data-tester/include/**.hpp",
            "data-tester/src/**.cpp",
        }

        includedirs {
            "data-tester/include",
        }

        filter "system:windows"
            systemversion "latest"
            defines { "PLATFORM_WINDOWS" }

        filter "system:linux"
            defines { "PLATFORM_LINUX" }

        filter "system:macosx"
            defines { "PLATFORM_MACOS" }

        filter "configurations:Debug"
            defines { "DEBUG", "_DEBUG" }
            runtime "Debug"
            symbols "on"
            optimize "off"

        filter "configurations:Release"
            defines { "NDEBUG" }
            runtime "Release"
            symbols "on"
            optimize "on"

        filter {}

    -- -----------------------------------------------------------------------
    -- Vendor: GLFW
    -- -----------------------------------------------------------------------
    project "glfw"
        location "libs/glfw"
        kind "StaticLib"
        language "C"
        staticruntime "on"
        warnings "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        -- Common platform-independent sources
        files {
            "libs/glfw/src/context.c",
            "libs/glfw/src/init.c",
            "libs/glfw/src/input.c",
            "libs/glfw/src/monitor.c",
            "libs/glfw/src/platform.c",
            "libs/glfw/src/vulkan.c",
            "libs/glfw/src/window.c",
            "libs/glfw/src/egl_context.c",
            "libs/glfw/src/osmesa_context.c",
            "libs/glfw/src/null_init.c",
            "libs/glfw/src/null_joystick.c",
            "libs/glfw/src/null_monitor.c",
            "libs/glfw/src/null_window.c",
        }

        includedirs {
            "libs/glfw/include",
        }

        filter "system:windows"
            systemversion "latest"
            defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }
            files {
                "libs/glfw/src/win32_init.c",
                "libs/glfw/src/win32_joystick.c",
                "libs/glfw/src/win32_module.c",
                "libs/glfw/src/win32_monitor.c",
                "libs/glfw/src/win32_thread.c",
                "libs/glfw/src/win32_time.c",
                "libs/glfw/src/win32_window.c",
                "libs/glfw/src/wgl_context.c",
            }

        filter "system:linux"
            defines { "_GLFW_X11" }
            files {
                "libs/glfw/src/x11_init.c",
                "libs/glfw/src/x11_monitor.c",
                "libs/glfw/src/x11_window.c",
                "libs/glfw/src/xkb_unicode.c",
                "libs/glfw/src/posix_module.c",
                "libs/glfw/src/posix_poll.c",
                "libs/glfw/src/posix_thread.c",
                "libs/glfw/src/posix_time.c",
                "libs/glfw/src/glx_context.c",
                "libs/glfw/src/linux_joystick.c",
            }

        filter "system:macosx"
            defines { "_GLFW_COCOA" }
            files {
                "libs/glfw/src/cocoa_init.m",
                "libs/glfw/src/cocoa_joystick.m",
                "libs/glfw/src/cocoa_monitor.m",
                "libs/glfw/src/cocoa_window.m",
                "libs/glfw/src/nsgl_context.m",
                "libs/glfw/src/macos_time.c",
                "libs/glfw/src/posix_thread.c",
                "libs/glfw/src/posix_module.c",
            }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter {}

    -- -----------------------------------------------------------------------
    -- Vendor: Dear ImGui  (core + GLFW/OpenGL3 backends)
    -- -----------------------------------------------------------------------
    project "imgui"
        location "libs/imgui"
        kind "StaticLib"
        language "C++"
        cppdialect "C++11"
        staticruntime "on"
        warnings "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "libs/imgui/imgui.cpp",
            "libs/imgui/imgui_draw.cpp",
            "libs/imgui/imgui_tables.cpp",
            "libs/imgui/imgui_widgets.cpp",
            "libs/imgui/backends/imgui_impl_glfw.cpp",
            "libs/imgui/backends/imgui_impl_opengl3.cpp",
        }

        includedirs {
            "libs/imgui",
            "libs/imgui/backends",
            "libs/glfw/include",
        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter {}

    -- -----------------------------------------------------------------------
    -- Vendor: ImPlot
    -- -----------------------------------------------------------------------
    project "implot"
        location "libs/implot"
        kind "StaticLib"
        language "C++"
        cppdialect "C++11"
        staticruntime "on"
        warnings "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "libs/implot/implot.cpp",
            "libs/implot/implot_items.cpp",
        }

        includedirs {
            "libs/implot",
            "libs/imgui",
        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter {}

    -- -----------------------------------------------------------------------
    -- Tests (Google Test runner)
    -- -----------------------------------------------------------------------
    project "Tests"
        location "Tests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
        warnings "Extra"
        flags { "FatalWarnings" }

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "Tests/src/**.cpp",
            "Tests/include/**.h",
            "Tests/include/**.hpp",
        }

        includedirs {
            "Tests/include",
            "data-tester/include",
            "libs/googletest/googletest/include",
        }

        links {
            "data-tester",
            "GoogleTest",
        }

        filter "system:windows"
            systemversion "latest"
            defines { "PLATFORM_WINDOWS" }

        filter "system:linux"
            defines { "PLATFORM_LINUX" }
            links { "pthread" }

        filter "system:macosx"
            defines { "PLATFORM_MACOS" }

        filter "configurations:Debug"
            defines { "DEBUG", "_DEBUG" }
            runtime "Debug"
            symbols "on"
            optimize "off"

        filter "configurations:Release"
            defines { "NDEBUG" }
            runtime "Release"
            symbols "on"
            optimize "on"

        filter {}

    -- -----------------------------------------------------------------------
    -- DataVisualizer  (plots test data with ImPlot)
    -- -----------------------------------------------------------------------
    project "DataVisualizer"
        location "DataVisualizer"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
        warnings "Extra"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "DataVisualizer/src/**.cpp",
            "DataVisualizer/include/**.h",
            "DataVisualizer/include/**.hpp",
        }

        includedirs {
            "DataVisualizer/include",
            "data-tester/include",
            "libs/imgui",
            "libs/imgui/backends",
            "libs/implot",
            "libs/glfw/include",
        }

        links {
            "data-tester",
            "imgui",
            "implot",
            "glfw",
        }

        filter "system:windows"
            systemversion "latest"
            defines { "PLATFORM_WINDOWS" }
            links { "opengl32", "gdi32", "user32", "shell32" }

        filter "system:linux"
            defines { "PLATFORM_LINUX" }
            links { "pthread", "GL", "dl", "X11" }

        filter "system:macosx"
            defines { "PLATFORM_MACOS" }
            links { "OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

        filter "configurations:Debug"
            defines { "DEBUG", "_DEBUG" }
            runtime "Debug"
            symbols "on"
            optimize "off"

        filter "configurations:Release"
            defines { "NDEBUG" }
            runtime "Release"
            symbols "on"
            optimize "on"

        filter {}
