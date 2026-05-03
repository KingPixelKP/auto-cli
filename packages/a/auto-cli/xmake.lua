package("auto-cli")
    set_description("The auto-cli package")

    add_urls("https://github.com/KingPixelKP/auto-cli.git")
    add_versions("0.1.0", "582af9f15c7180a67907e14dd8f369788c56cf6a")

    on_install(function (package)
        local configs = {}
        if package:config("shared") then
            configs.kind = "shared"
        end
        import("package.tools.xmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <auto-cli/auto-cli.h>

            static void test() {
                auto_cli::AutoCli cli("xnew", "tool to aid xmake project creation");
                auto_cli::AutoCli& init = cli.subcommand("init", "Initialize a new project");
                auto& name = init.positional<std::string>("name", "The name of the project");
                auto& type = init.option<std::string>("type", "The type of the project", "slib");

                auto& say_hello = cli.subcommand("say-hello", "Say hello to someone");
                auto& hello_name = say_hello.positional<std::string>("name", "The name of the person to greet");
                auto& hello_greeting = say_hello.positional<std::string>("greeting", "The greeting message", "Hello");
                say_hello.callback([&]() {
                    std::cout << hello_greeting.get_value() << ", " << hello_name.get_value() << "!\n";
                });

                auto& help = cli.subcommand("help", "Show help information");
                help.callback([&]() {
                    cli.print_help();
                });

                cli.parse(argc, argv);
                return 0;
            }
        ]]}, {configs = {languages = "c++20"}, includes = "auto-cli/auto-cli.h"}))
    end)
