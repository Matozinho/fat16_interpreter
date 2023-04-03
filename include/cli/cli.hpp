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
  void print_file(std::string filename);

  std::unordered_map<std::string, std::function<void(std::vector<std::string> params)>> commands = {
      {"ls", [this](std::vector<std::string> params) { this->list_directory(); }},
      {"cat", [this](std::vector<std::string> params) { this->print_file(params[0]); }},
      {"cd", [this](std::vector<std::string> params) { this->fat->unimplemented_logic(); }},
      {"clear", [this](std::vector<std::string> params) { std::system("clear"); }},
  };
};
