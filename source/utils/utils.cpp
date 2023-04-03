#pragma once

#include <string>
#include <utils/utils.hpp>

namespace Utils {
  std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" ");
    if (std::string::npos == first) return std::string();

    size_t last = str.find_last_not_of(" ");
    if (std::string::npos == last) return std::string();

    return str.substr(first, (last - first + 1));
  }
  std::string mount_name(const unsigned char* name, const unsigned char* extension) {
    std::string trimmedName = Utils::trim(std::string(reinterpret_cast<const char*>(name), 8));
    std::string trimmedExtension
        = Utils::trim(std::string(reinterpret_cast<const char*>(extension), 3));
    std::string dot = trimmedExtension.length() > 0 ? "." : "";

    std::string filename = trimmedName + dot + trimmedExtension;
    return filename;
  }
}  // namespace Utils
