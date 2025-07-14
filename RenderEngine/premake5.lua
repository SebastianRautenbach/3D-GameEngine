workspace "WizmEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "WizmEngine"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"


    targetdir ("bin/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/dependencies/add_on/scriptarray/scriptarray.cpp",
        "include/dependencies/add_on/scriptbuilder/scriptbuilder.cpp",
        "include/dependencies/add_on/scriptstdstring/scriptstdstring.cpp",
        "include/dependencies/add_on/scriptstdstring/scriptstdstring_utils.cpp",
        "include/dependencies/glad.c",
        "include/dependencies/imgui/backends/imgui_impl_glfw.cpp",
        "include/dependencies/imgui/backends/imgui_impl_opengl3.cpp",
        "include/dependencies/imgui/imgui.cpp",
        "include/dependencies/imgui/imgui_demo.cpp",
        "include/dependencies/imgui/imgui_draw.cpp",
        "include/dependencies/imgui/imgui_tables.cpp",
        "include/dependencies/imgui/imgui_widgets.cpp",
        "include/dependencies/ImGuizmo/ImCurveEdit.cpp",
        "include/dependencies/ImGuizmo/ImGradient.cpp",
        "include/dependencies/ImGuizmo/ImGuizmo.cpp",
        "include/dependencies/ImGuizmo/ImSequencer.cpp",
        "include/dependencies/sqlite3.c",
        "include/dependencies/tinyfiledialogs.c",
        "src/**.cpp",
        "src/**.c",
        "include/**.h",
        "include/**.hpp"
    }

    includedirs
    {
        "include/dependencies/imgui",
        "RenderEngine/include/dependencies/imgui",
        "include/dependencies",
        "include",
        "include/dependencies/fmod",
        "include/dependencies/angelscript/include"
    }

    libdirs 
    {
        "lib"
    }

    links 
    {
        "glfw3dll",
        "opengl32",
        "assimp-vc143-mtd",
        "angelscript64d",
        "fmod_vc",
        "fmodL_vc"
    }

    filter "system.windows"
        systemversion "latest"


    filter "configurations:Debug"
        runtime "Debug"
        staticruntime "On"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        staticruntime "On"
        optimize "On"