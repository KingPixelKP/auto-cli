package("auto-cli")
    set_homepage("https://github.com/KingPixelKP/auto-cli")
    set_description("A lightweight C++ CLI argument parser")

    add_urls("https://github.com/KingPixelKP/auto-cli/archive/refs/tags/$(version).tar.gz")
    add_versions("0.1.0", "914923a41d047df70520954bc02f958b225780503921ecaaa17b0eeb31960d13")

    on_install(function(package)
        import("package.tools.xmake").install(package)
    end)