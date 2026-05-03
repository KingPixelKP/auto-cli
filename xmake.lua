add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

target("main")
    set_kind("binary")
    add_deps("auto-cli")
    add_files("main.cpp")


includes("auto-cli")