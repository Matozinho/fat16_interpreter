#include <fmt/core.h>

#include <cli/cli.hpp>
#include <functional>
#include <iostream>
#include <ostream>
#include <penis/penis.hpp>

Cli* Cli::execute_command(const std::string& command) {
  auto it = this->commands.find(command);
  if (it == this->commands.end()) {
    std::cout << "Unknown command: " << command << std::endl;

    return this;
  }

  std::cout << "Known command: " << std::endl;
  it->second(command);

  return this;
}

Cli* Cli::run() {
  penis::PromptBuilder repl;

  repl.subscribe([this](const std::string& command) { this->execute_command(command); });

  repl.run();

  return this;
}
