#include "auto-cli/auto-cli.h"

#include <iostream>
#include <algorithm>

namespace auto_cli {

AutoCli::AutoCli(std::string app_name, std::string description)
    : app_name_(std::move(app_name)), description_(std::move(description)) {}

AutoCli::~AutoCli() = default;

AutoCli::AutoCli(AutoCli&& other) noexcept
    : app_name_(std::move(other.app_name_)),
      description_(std::move(other.description_)),
      callback_(std::move(other.callback_)),
      positionals_(std::move(other.positionals_)),
      options_(std::move(other.options_)),
      flags_(std::move(other.flags_)),
      subcommands_(std::move(other.subcommands_)) {}

AutoCli& AutoCli::operator=(AutoCli &&other) noexcept {
  if (this != &other) {
    app_name_ = std::move(other.app_name_);
    description_ = std::move(other.description_);
    positionals_ = std::move(other.positionals_);
    options_ = std::move(other.options_);
    flags_ = std::move(other.flags_);
    subcommands_ = std::move(other.subcommands_);
    callback_ = std::move(other.callback_);
  }
  return *this;
}

void AutoCli::print_help() const {
  std::cout << "Command: " << app_name_  << "\n" << description_ << "\n";
  for (const auto& positional : positionals_) {
    positional->print_help();
  }
  for (const auto& [name, option] : options_) {
    option->print_help();
  }
  for (const auto& [name, flag] : flags_) {
    flag->print_help();
  }
  for (const auto& subcommand : subcommands_) {
    subcommand.print_help();
  }
}

void AutoCli::parse(int argc, char** argv) {
  if (argc - 1 < positionals_.size()) {
    std::cerr << "Error: Not enough positional arguments provided."
              << std::endl;
    print_help();
    return;
  }
  // Parse positional arguments
  for (size_t i = 0; i < positionals_.size(); ++i) {
    std::cout << "Parsing positional argument '" << positionals_[i]->get_name() << "': " << argv[i] << std::endl;
    positionals_[i]->parse(argv[i+1]);
    argc--;
  }

  // Parse options and flags
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.starts_with("--")) {
      auto it = options_.find(arg.substr(2));
      if (it != options_.end()) {
        it->second->parse(argv[i]);
      }
      auto jt = flags_.find(arg.substr(2));
      if (jt != flags_.end()) {
        jt->second->set();
      }
    }
  }
  // check if all options and flags have been set
  for (const auto& [name, option] : options_) {
    if (option->is_required && !option->is_set) {
      std::cerr << "Error: Option '--" << name << "' is required." << std::endl;
      print_help();
      return;
    }
  }
  for (const auto& [name, flag] : flags_) {
    if (flag->is_required && !flag->is_set) {
      std::cerr << "Error: Flag '--" << name << "' is required." << std::endl;
      print_help();
      return;
    }
  }

  // Parse subcommands
  for (int i = 1; i < argc; ++i) {
    std::cout << "Parsing subcommand: " << argv[i] << std::endl;
    std::string arg = argv[i];
    auto it = std::find_if(subcommands_.begin(), subcommands_.end(),
                           [&arg](const AutoCli& cmd) { return cmd.app_name_ == arg; });
    if (it != subcommands_.end()) {
      it->parse(argc - i, argv + i);
      return;
    }
  }

  //If is last subcommand then call callback 
  if (callback_)
    (*callback_)();
}

AutoCli& AutoCli::subcommand(const std::string& name,
                             const std::string& description) {
  if (std::find_if(subcommands_.begin(), subcommands_.end(),
                   [&name](const AutoCli& cmd) { return cmd.app_name_ == name; }) == subcommands_.end()) {
    subcommands_.emplace_back(name, description);
  }
  return subcommands_.back();
}

Flag& AutoCli::flag(const std::string& name, const std::string& description,
                    bool default_value) {
  if (hasOption(name)) {
    std::cerr << "Error: Option '" << name
              << "' is already defined as an option." << std::endl;
    std::terminate();
  }
  if (flags_.find(name) == flags_.end()) {
    flags_.try_emplace(name, std::make_unique<Flag>(name, description, default_value));
  }
  return *flags_[name];
}

bool AutoCli::hasOption(const std::string& name) const {
  return options_.find(name) != options_.end();
}

bool AutoCli::hasFlag(const std::string& name) const {
  return flags_.find(name) != flags_.end();
}

void AutoCli::callback(std::function<void()> func) {
  callback_ = std::move(func);
}

}  // namespace auto_cli
