-- premake5.lua
workspace "SFML-Project-Workspace"
   configurations { "simple", "debug", "release" }

project "easing-01"

   language    "C++"
   cppdialect  "C++17"
   targetdir   "../bin/"
   location    "../"
   objdir      "%{cfg.buildcfg}/obj"

   files {
      "../src/**.cpp"
   }
   includedirs    { "../include", "/c/lib/SFML-2.5.1/include", "/c/lib/nlohmann" }
   libdirs        { "/c/lib/SFML-2.5.1/lib/" }
   links          { "stdc++fs" }

   enablewarnings { "all", "extra", "shadow", "unused" }

   filter "configurations:simple"
      kind     "ConsoleApp"
      targetname "easing-01"
      defines  { "DEBUG" }
      symbols  "Off"
      links    { "sfml-system-d", "sfml-window-d", "sfml-graphics-d" }
      disablewarnings { "unused" }

   filter "configurations:debug"
      kind     "ConsoleApp"
      targetname "easing-01-dbg"
      defines  { "DEBUG" }
      symbols  "On"
      optimize "Debug"
      links    { "sfml-system-d", "sfml-window-d", "sfml-graphics-d" }

   filter "configurations:release"
      kind     "WindowedApp"
      targetdir  "../Release/"
      targetname "easing-01-Release"
      symbols  "Off"
      defines  { "NDEBUG" }
      optimize "On"
      links    { "sfml-system", "sfml-window", "sfml-graphics" }
