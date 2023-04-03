#pragma once

#include <string>
#include <vector>

namespace Utils {
  std::string trim(const std::string& str);
  std::string mount_name(const unsigned char* name, const unsigned char* extension);
  std::vector<std::string> split(const std::string& line, char delimiter);
}  // namespace Utils
