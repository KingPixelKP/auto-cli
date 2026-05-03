set_project("auto-cli")
set_version("0.1.0")
set_languages("c++23")

option("build_main")
    set_default(true)
    set_description("Build the example main")
option_end()

if has_config("build_main") then
    add_rules("mode.debug", "mode.release")
    add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

    target("main")
        set_kind("binary")
        add_deps("auto-cli")
        add_files("main.cpp")
end

target("auto-cli")
    set_kind("static")
    set_languages("c++20", {public = true})
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_includedirs("src", {private = true})