#pragma once
#ifndef AUTO_CLI_POSITIONAL_H
#define AUTO_CLI_POSITIONAL_H

#include <iostream>
#include <string>

#include "convert.h"

namespace auto_cli {

namespace detail {

struct IPositional {
  virtual ~IPositional() = default;
  virtual std::string get_name() const = 0;
  virtual std::string get_description() const = 0;
  virtual bool parse(const std::string& value) = 0;
  virtual void print_help() const = 0;
};
}  // namespace detail
template <typename T>
struct Positional : public detail::IPositional {

  Positional(const Positional&) = delete;
  Positional& operator=(const Positional&) = delete;

  Positional(Positional&&) noexcept = default;
  Positional& operator=(Positional&&) noexcept = default;

  Positional(std::string name, std::string description, T default_value)
      : name_(std::move(name)),
        description_(std::move(description)),
        value_(default_value) {}

  std::string get_name() const override { return name_; }
  std::string get_description() const override { return description_; }
  T get_value() const { return value_; }

  bool parse(const std::string& value) override {
    auto converted = auto_cli::detail::convert<T>(value);
    if (converted) {
      value_ = *converted;
      return true;
    }
    return false;
  }

  void print_help() const override {
    std::cout << "  " << get_name() << ": " << get_description() << "\n";
  }

 private:
  std::string name_;
  std::string description_;
  T value_;
};

}  // namespace auto_cli

#endif