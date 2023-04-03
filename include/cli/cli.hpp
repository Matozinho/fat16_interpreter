#pragma once

#include <functional>
#include <interpreter/fat16.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

class Cli {
public:
  Cli() = default;
  Cli(interpreter::Fat16* fat) {
    this->fat = fat;
    this->current_directory = fat->get_root_dir_begin();
    this->current_dir_entries = this->fat->get_dir_entries(this->current_directory);
  }
  ~Cli() = default;

  Cli* run();

private:
  unsigned int current_directory;
  interpreter::Fat16* fat;
  std::vector<interpreter::Fat16::fat_dir_entry> current_dir_entries;

  Cli* execute_command(const std::string& command);
  void list_directory();

  std::unordered_map<std::string, std::function<void()>> commands = {
      {"ls", [this]() { this->list_directory(); }},
      {"cat", [this]() { this->fat->unimplemented_logic(); }},
      {"cd", [this]() { this->fat->unimplemented_logic(); }},
  };
};
