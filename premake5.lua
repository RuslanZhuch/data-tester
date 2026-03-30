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
