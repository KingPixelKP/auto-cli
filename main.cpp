#include "auto-cli/auto-cli.h"

int main(int argc, char** argv) {
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