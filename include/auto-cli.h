#pragma once
#ifndef AUTO_CLI_H
#define AUTO_CLI_H

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "flag.h"
#include "option.h"
#include "positional.h"

namespace auto_cli {

class AutoCli {
 public:
  explicit AutoCli(std::string app_name, std::string description = "");
  ~AutoCli();

  AutoCli(const AutoCli&) = delete;
  AutoCli& operator=(const AutoCli&) = delete;

  AutoCli(AutoCli&&) noexcept;
  AutoCli& operator=(AutoCli&&) noexcept;

  void parse(int argc, char** argv);
  void print_help() const;

  template <typename T>
  [[nodiscard]]
  Positional<T>& positional(const std::string& name,
                            const std::string& description = "",
                            const T& default_value = T()) {
    auto positional =
        std::make_shared<Positional<T>>(name, description, default_value);
    auto ipositional = std::reinterpret_pointer_cast<detail::IPositional>(positional);
    positionals_.emplace_back(ipositional);
    return *positional;
  }

  [[nodiscard]]
  AutoCli& subcommand(const std::string& name,
                      const std::string& description = "");

  template <typename T>
  [[nodiscard]]
  Option<T>& option(const std::string& name,
                    const std::string& description = "",
                    const T& default_value = T()) {
    if (hasFlag(name)) {
      std::cerr << "Error: Flag '" << name << "' is already defined as a flag."
                << std::endl;
      std::terminate();
    }
    if (options_.find(name) == options_.end()) {
      auto option = std::make_shared<Option<T>>(name, description, default_value);
      auto ioption = std::reinterpret_pointer_cast<detail::IOption>(option);
      options_.emplace(name, ioption);
    }
    auto ioption = options_[name];
    return *std::reinterpret_pointer_cast<Option<T>>(ioption);
  }

  [[nodiscard]]
  Flag& flag(const std::string& name, const std::string& description = "",
             bool default_value = false);

  void callback(std::function<void()> func);

 private:
  bool hasOption(const std::string& name) const;
  bool hasFlag(const std::string& name) const;

  std::string app_name_;
  std::string description_;

  std::optional<std::function<void()>> callback_ = std::nullopt;

  std::vector<std::shared_ptr<detail::IPositional>> positionals_;
  std::unordered_map<std::string, std::shared_ptr<detail::IOption>> options_;
  std::unordered_map<std::string, std::unique_ptr<Flag>> flags_;
  std::vector<AutoCli> subcommands_;
};

}  // namespace auto_cli

#endif