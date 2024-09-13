workspace "GamepadLib"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Distribution"
	}

	startproject "Examples"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GamepadLib"
	location "GamepadLib"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		filter "configurations:Debug"
		defines{
			"DRV_DEBUG"
		}

		symbols "On"
		optimize "Off"
		staticruntime "Off"
		runtime "Debug"

	filter "configurations:Release"
		defines{
			"DRV_RELEASE"
		}
		
		symbols "On"
		optimize "On"
		staticruntime "Off"
		runtime "Release"

	filter "configurations:Distribution"
		defines{
			"DRV_DISTRIBUITION"
		}
		
		symbols "Off"
		optimize "Full"
		staticruntime "Off"
		runtime "Release"

project "Examples"
	location "Examples"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"GamepadLib/src",
	}

	links{
		"Hid",
		"SetupAPI",
		"GamepadLib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		defines{
			"DRV_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines{
			"DRV_DEBUG"
		}
		
		symbols "On"
		optimize "Off"
		staticruntime "Off"
		runtime "Debug"

	filter "configurations:Release"
		defines{
			"DRV_RELEASE"
		}
		
		symbols "On"
		optimize "On"
		staticruntime "Off"
		runtime "Release"

	filter "configurations:Distribution"
		defines{
			"DRV_DISTRIBUITION"
		}
		
		symbols "Off"
		optimize "On"
		staticruntime "Off"
		runtime "Release"