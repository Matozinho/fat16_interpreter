#pragma once

#include <functional>
#include <interpreter/fat.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>

class Cli {
public:
  Cli() = default;
  Cli(interpreter::Fat* fat) {
    this->fat = fat;
    this->current_directory = 0;
  }
  ~Cli() = default;

  Cli* run();

private:
  unsigned int current_directory;
  interpreter::Fat* fat;

  Cli* execute_command(const std::string& command);

  std::unordered_map<std::string, std::function<void(const std::string&)>> commands = {
      {"ls", [this](const std::string& args) { this->fat->list_directory(); }},
      {"cd", [this](const std::string& args) { this->fat->unimplemented_logic(); }},
      {"cat", [this](const std::string& args) { this->fat->unimplemented_logic(); }},
  };
};
