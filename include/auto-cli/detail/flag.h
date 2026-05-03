#pragma once
#ifndef AUTO_CLI_FLAG_H
#define AUTO_CLI_FLAG_H

#include <string>
#include <iostream>

namespace auto_cli {
    struct Flag {

        Flag(std::string name, std::string description, bool default_value)
            : name(std::move(name)),
              description(std::move(description)),
              default_value(default_value) {}

        Flag(const Flag&) = delete;
        Flag& operator=(const Flag&) = delete;

        Flag(Flag&&) noexcept = default;
        Flag& operator=(Flag&&) noexcept = default;

        std::string name;
        std::string description;
        bool default_value;
        bool is_required = false;
        bool is_set = false;

        void set() {
            is_set = true;
        }

        void print_help() const {
            std::cout << "  --" << name << ": " << description
                      << " (default: " << (default_value ? "true" : "false") << ")\n";
        }

        void require() {
            is_required = true;
        }
    };
}

#endif