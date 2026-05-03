package("auto-cli")
    set_description("The auto-cli package")

    add_urls("https://github.com/KingPixelKP/auto-cli.git")
    add_versions("0.1.0", "680278edd02466356d88e8d26fc43768a32379d8")

    on_install(function (package)
        local configs = {}
        if package:config("shared") then
            configs.kind = "shared"
        end
        import("package.tools.xmake").install(package, configs)
    end)

    on_test(function (package)
        
    end)
