#pragma once
#ifndef AUTO_CLI_OPTION_H
#define AUTO_CLI_OPTION_H

#include <string>
#include <iostream>

#include "convert.h"

namespace auto_cli {

namespace detail {

struct IOption {
  virtual ~IOption() = default;
  virtual std::string get_name() const = 0;
  virtual std::string get_description() const = 0;
  virtual bool parse(const std::string& value) = 0;
  virtual void require() { is_required = true; }
  virtual void print_help() const = 0;
  bool is_required = false;
  bool is_set = false;
};
}  // namespace detail

template <typename T>
struct Option : public detail::IOption {

  Option(const Option&) = delete;
  Option& operator=(const Option&) = delete;

  Option(Option&&) noexcept = default;
  Option& operator=(Option&&) noexcept = default;

  Option(std::string name, std::string description, T default_value)
      : name_(std::move(name)),
        description_(std::move(description)),
        value_(default_value) {}

  std::string get_name() const override { return name_; }
  std::string get_description() const override { return description_; }
  bool parse(const std::string& value) override {
    auto converted = auto_cli::detail::convert<T>(value);
    if (converted) {
      value_ = *converted;
      is_set = true;
      return true;
    }
    return false;
  }
  T get_value() const { return value_; }

  void print_help() const override {
    std::cout << "  --" << get_name() << ": " << get_description() << "\n";
  }

 private:
  std::string name_;
  std::string description_;
  T value_;
};
}  // namespace auto_cli

#endif