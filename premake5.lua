workspace "pcbr"
	configurations { "Debug", "Release" }
	location "build"

project "pcbr"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	-- Get include and library flags from wx-config and sdl-config
	local wx_cxxflags = os.outputof("wx-config --cxxflags")
	local wx_libs = os.outputof("wx-config --libs")
	local sdl_cxxflags = os.outputof("sdl2-config --cxxflags")
	local sdl_libs = os.outputof("sdl2-config --libs")

	buildoptions { wx_cxxflags, sdl_cxxflags }
	linkoptions { wx_libs, sdl_libs }

	files {
		"src/**.cpp",
		"include/**.h",
		"extern/kicad/libs/sexpr/**.cpp",
		"extern/kicad/libs/sexpr/**.h",
	}

	includedirs {
		"include",
		"extern/kicad/include",
		"extern/kicad/libs/sexpr/include",
		"extern/kicad/libs/sexpr/include/sexpr",
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
