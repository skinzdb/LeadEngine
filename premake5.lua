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
--IncludeDir["GLFW"] = "LeadEngine/vendor/GLFW/include"
--IncludeDir["Glad"] = "LeadEngine/vendor/Glad/include"
--IncludeDir["ImGui"] = "LeadEngine/vendor/imgui"
--IncludeDir["glm"] = "LeadEngine/vendor/glm"
--IncludeDir["stb_image"] = "LeadEngine/vendor/stb_image"

--group "Dependencies"
--	include "LeadEngine/vendor/GLFW"
--	include "LeadEngine/vendor/Glad"
--	include "LeadEngine/vendor/imgui"

group ""

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
		"%{prj.name}/src/**.cpp",
		--"%{prj.name}/vendor/stb_image/**.h",
		--"%{prj.name}/vendor/stb_image/**.cpp",
		--"%{prj.name}/vendor/glm/glm/**.hpp",
		--"%{prj.name}/vendor/glm/glm/**.inl",
	}

	--defines
	--{
	--	"_CRT_SECURE_NO_WARNINGS",
	--	"GLFW_INCLUDE_NONE"
	--}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
		--"%{IncludeDir.GLFW}",
		--"%{IncludeDir.Glad}",
		--"%{IncludeDir.ImGui}",
		--"%{IncludeDir.glm}",
		--"%{IncludeDir.stb_image}"
	}

	--links 
	--{ 
	--	"GLFW",
	--	"Glad",
	--	"ImGui",
	--	"opengl32.lib"
	--}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"LE_PLATFORM_WINDOWS",
			"LE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "LE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LE_DIST"
		runtime "Release"
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
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LE_DIST"
		runtime "Release"
		optimize "on"
