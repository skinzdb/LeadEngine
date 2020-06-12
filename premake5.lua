workspace "LeadEngine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "LeadEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "LeadEngine/vendor/Glad/include"

include "LeadEngine/vendor/GLFW"
include "LeadEngine/vendor/Glad"

project "LeadEngine"
	location "LeadEngine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "le_pch.h"
	pchsource "LeadEngine/src/le_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"LE_PLATFORM_WINDOWS",
			"LE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "LE_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "LE_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "LE_DIST"
		buildoptions "/MD"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"LeadEngine/vendor/spdlog/include",
		"LeadEngine/src",
		"LeadEngine/vendor",
		--"%{IncludeDir.glm}"
	}

	links
	{
		"LeadEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"LE_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "LE_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "LE_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "LE_DIST"
		buildoptions "/MD"
		optimize "on"
