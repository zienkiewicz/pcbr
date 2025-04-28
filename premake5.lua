workspace "pcbr"
	configurations { "Debug", "Release" }
	location "build"

project "pcbr"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	-- Get include and library flags from wx-config
	local cxxflags = os.outputof("wx-config --cxxflags")
	local libs = os.outputof("wx-config --libs")
	-- filter {}
	buildoptions { cxxflags }
	linkoptions { libs }

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
