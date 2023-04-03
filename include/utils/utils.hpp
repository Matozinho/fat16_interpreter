#pragma once

#include <string>

namespace Utils {
  std::string trim(const std::string& str);
  std::string mount_name(const unsigned char* name, const unsigned char* extension);
}  // namespace Utils
