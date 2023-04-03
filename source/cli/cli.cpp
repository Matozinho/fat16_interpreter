#include <fmt/core.h>

#include <algorithm>
#include <cli/cli.hpp>
#include <functional>
#include <interpreter/fat16.hpp>
#include <iostream>
#include <ostream>
#include <penis/penis.hpp>
#include <utils/utils.hpp>
#include <vector>

Cli* Cli::execute_command(const std::string& command) {
  // split command in the space
  auto splited_command = Utils::split(command, ' ');

  auto it = this->commands.find(splited_command[0]);
  if (it == this->commands.end()) {
    std::cout << "Unknown command: " << splited_command[0] << std::endl;

    return this;
  }

  splited_command.erase(splited_command.begin());
  it->second(splited_command);

  return this;
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

  if (static_cast<interpreter::Fat16::FileType>(dir_entry.attribute)
      != interpreter::Fat16::FileType::DIRECTORY) {
    fmt::print("Is not a directory\n");

    return;
  }

  this->current_dir_first_cluster = dir_entry.low_first_cluster;
  this->current_dir_entries = this->fat->get_dir_entries(this->current_dir_first_cluster);
}

void Cli::list_directory() {
  fmt::print("{:<10}{:<12}{:<20}\n", "type", "name", "size(bytes)");
  fmt::print("{:-<10}{:-<12}{:-<20}\n", "", "", "");

  for (const auto& entry : this->current_dir_entries) {
    std::string filename = Utils::mount_name(entry.name, entry.extension);
    std::string filetype = static_cast<interpreter::Fat16::FileType>(entry.attribute)
                                   == interpreter::Fat16::FileType::ARCHIVE
                               ? "arch"
                               : "dir";

    fmt::print("{:<10}{:<12}{:<20}\n", filetype, filename, entry.file_size);
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

  if (static_cast<interpreter::Fat16::FileType>(entry.attribute)
      != interpreter::Fat16::FileType::ARCHIVE) {
    std::cout << "Is not a file" << std::endl;

    return;
  }

  std::vector<unsigned char> file_content = this->fat->get_file(entry);

  for (auto c : file_content) {
    std::cout << c;
  }

  // in the case that the file haven't a new line at the end
  std::cout << std::endl;
}

Cli* Cli::run() {
  penis::PromptBuilder repl;

  repl.subscribe([this](const std::string& command) { this->execute_command(command); });
  repl.prompt("> ");

  repl.run();

  return this;
}
