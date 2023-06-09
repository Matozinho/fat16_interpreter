#pragma once

#include <cstdint>
#include <functional>
#include <interpreter/fat16.hpp>
#include <iostream>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

class Cli {
public:
  Cli() = default;
  Cli(interpreter::Fat16* fat) {
    this->fat = fat;
    this->current_dir_entry = {.attribute = 0x10, .low_first_cluster = 0};
    this->current_dir_entries
        = this->fat->get_dir_entries(this->current_dir_entry.low_first_cluster);
    this->dir_stack.push(this->current_dir_entry);
  }
  ~Cli() = default;

  Cli* run();

private:
  interpreter::Fat16* fat;
  std::vector<interpreter::Fat16::fat_dir_entry> current_dir_entries;
  interpreter::Fat16::fat_dir_entry current_dir_entry;
  std::stack<interpreter::Fat16::fat_dir_entry> dir_stack;

  std::string get_filetype_icon(uint8_t type);
  void list_directory();
  void print_file(std::string filename);
  void change_directory(std::string directory);
  void get_path();
};
