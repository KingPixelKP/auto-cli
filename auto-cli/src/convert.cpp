#include "auto-cli/convert.h"

#include <charconv>

namespace auto_cli::detail {
template <>
std::optional<int> convert(const std::string& value) {
  int result;
  auto [ptr, ec] =
      std::from_chars(value.data(), value.data() + value.size(), result);
  if (ec == std::errc()) {
    return result;
  }
  return std::nullopt;
}

template <>
std::optional<float> convert(const std::string& value) {
  float result;
  auto [ptr, ec] =
      std::from_chars(value.data(), value.data() + value.size(), result);
  if (ec == std::errc()) {
    return result;
  }
  return std::nullopt;
}

template <>
std::optional<double> convert(const std::string& value) {
  double result;
  auto [ptr, ec] =
      std::from_chars(value.data(), value.data() + value.size(), result);
  if (ec == std::errc()) {
    return result;
  }
  return std::nullopt;
}

template <>
std::optional<std::string> convert(const std::string& value) {
  return value;
}

template <>
std::optional<bool> convert(const std::string& value) {
  if (value == "true") {
    return true;
  } else if (value == "false") {
    return false;
  }
  return std::nullopt;
}
}  // namespace auto_cli::detail
