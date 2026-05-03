#pragma once
#ifndef AUTO_CLI_CONVERT_H
#define AUTO_CLI_CONVERT_H

#include <string>
#include <optional>

namespace auto_cli::detail {
    template <typename T>
    std::optional<T> convert(const std::string& value);
}

#endif