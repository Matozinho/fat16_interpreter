#include <fmt/core.h>

#include <algorithm>
#include <cli/cli.hpp>
#include <cstdint>
#include <functional>
#include <interpreter/fat16.hpp>
#include <iostream>
#include <ostream>
#include <prepucio/repl.hpp>
#include <utils/utils.hpp>
#include <vector>

void Cli::get_path() {
  std::stack<interpreter::Fat16::fat_dir_entry> dir_stack_copy = dir_stack;
  std::stack<interpreter::Fat16::fat_dir_entry> temp_stack;

  while (!dir_stack_copy.empty()) {
    temp_stack.push(dir_stack_copy.top());
    dir_stack_copy.pop();
  }

  while (!temp_stack.empty()) {
    const interpreter::Fat16::fat_dir_entry& entry = temp_stack.top();
    fmt::print("{}/", Utils::mount_name(entry.name, entry.extension));
    temp_stack.pop();
  }
  std::cout << std::endl;
}

void Cli::change_directory(std::string dir_name) {
  auto dir_it = std::find_if(this->current_dir_entries.begin(), this->current_dir_entries.end(),
                             [&dir_name](const interpreter::Fat16::fat_dir_entry& entry) {
                               return Utils::mount_name(entry.name, entry.extension) == dir_name;
                             });

  if (dir_it == this->current_dir_entries.end()) {
    fmt::print("Directory not found\n");
    return;
  }

  interpreter::Fat16::fat_dir_entry& dir_entry
      = *(reinterpret_cast<interpreter::Fat16::fat_dir_entry*>(&(*dir_it)));

  if (dir_entry.attribute & static_cast<uint8_t>(interpreter::Fat16::FileType::DIRECTORY)) {
    this->current_dir_entry = dir_entry;
    this->current_dir_entries
        = this->fat->get_dir_entries(this->current_dir_entry.low_first_cluster);
    if (Utils::mount_name(dir_entry.name, dir_entry.extension) == "..")
      this->dir_stack.pop();
    else
      this->dir_stack.push(this->current_dir_entry);

    return;
  }

  fmt::print("Is not a directory\n");
}

std::string Cli::get_filetype_icon(uint8_t type) {
  std::string emojiString;

  if (type & static_cast<uint8_t>(interpreter::Fat16::FileType::DIRECTORY)) emojiString += "📁";

  if (type & static_cast<uint8_t>(interpreter::Fat16::FileType::ARCHIVE)) emojiString += "🗄️";

  if (type & static_cast<uint8_t>(interpreter::Fat16::FileType::READ_ONLY)) emojiString += "🔒";

  if (type & static_cast<uint8_t>(interpreter::Fat16::FileType::HIDDEN)) emojiString += "👀";

  if (type & static_cast<uint8_t>(interpreter::Fat16::FileType::SYSTEM)) emojiString += "💻";

  if (type & static_cast<uint8_t>(interpreter::Fat16::FileType::VOLUME_ID)) emojiString += "🔑";

  return emojiString;
}

void Cli::list_directory() {
  fmt::print("{:<20}{:<20}{:<20}\n", "type", "name", "size(bytes)");
  fmt::print("{:-<20}{:-<20}{:-<20}\n", "", "", "");

  for (const auto& entry : this->current_dir_entries) {
    std::string filename = Utils::mount_name(entry.name, entry.extension);
    std::string filetype_icon = this->get_filetype_icon(entry.attribute);

    fmt::print("{:<20}{:<20}{:<20}\n", filetype_icon, filename, entry.file_size);
  }
}

void Cli::print_file(std::string filename) {
  auto file = std::find_if(this->current_dir_entries.begin(), this->current_dir_entries.end(),
                           [&filename](const interpreter::Fat16::fat_dir_entry& entry) {
                             return Utils::mount_name(entry.name, entry.extension) == filename;
                           });

  if (file == this->current_dir_entries.end()) {
    std::cout << "File not found" << std::endl;

    return;
  }

  interpreter::Fat16::fat_dir_entry& entry
      = *(reinterpret_cast<interpreter::Fat16::fat_dir_entry*>(&(*file)));

  fmt::print("File validate: {}\n",
             entry.attribute & static_cast<uint8_t>(interpreter::Fat16::FileType::ARCHIVE));
  if (entry.attribute & static_cast<uint8_t>(interpreter::Fat16::FileType::ARCHIVE)) {
    std::vector<uint8_t> file_content = this->fat->get_file(entry);

    for (auto c : file_content) std::cout << c;

    // in the case that the file haven't a new line at the end
    std::cout << std::endl;

    return;
  }

  std::cout << "Is not a file" << std::endl;
}

Cli* Cli::run() {
  prepucio::REPL::Builder builder;

  std::unique_ptr<prepucio::REPL> repl
      = builder
            .addCommand("ls", "List all the entries of the directory",
                        [this]() { this->list_directory(); })
            .addCommand("cat", "Show the content of the specified file",
                        [this](std::string filename) { this->print_file(filename); })
            .addCommand("cd", "Change to the specified directory",
                        [this](std::string dirname) { this->change_directory(dirname); })
            .addCommand("pwd", "Show the path to the current directory",
                        [this]() { this->get_path(); })
            .addCommand("clear", "Clear the console", []() { std::system("clear"); })
            .build();

  repl->prompt("|> ");
  repl->run();

  return this;
}
