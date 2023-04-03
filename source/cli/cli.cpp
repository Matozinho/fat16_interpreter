#include <fmt/core.h>

#include <algorithm>
#include <cli/cli.hpp>
#include <functional>
#include <iostream>
#include <ostream>
#include <penis/penis.hpp>
#include <utils/utils.hpp>
#include <vector>

#include "interpreter/fat16.hpp"

Cli* Cli::execute_command(const std::string& command) {
  auto it = this->commands.find(command);
  if (it == this->commands.end()) {
    std::cout << "Unknown command: " << command << std::endl;

    return this;
  }

  it->second();

  return this;
}

void Cli::list_directory() {
  fmt::print("{:<10}{:<11}\n", "Type", "Name");

  for (const auto& entry : this->current_dir_entries) {
    std::string filename = Utils::mount_name(entry.name, entry.extension);
    std::string filetype = static_cast<interpreter::Fat16::FileType>(entry.attribute)
                                   == interpreter::Fat16::FileType::ARCHIVE
                               ? "File"
                               : "Dir";

    fmt::print("{:<10}{:<11}\n", filetype, filename);
  }
}

Cli* Cli::run() {
  penis::PromptBuilder repl;

  repl.subscribe([this](const std::string& command) { this->execute_command(command); });

  repl.run();

  return this;
}
