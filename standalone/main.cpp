#include <fmt/core.h>

#include <cli/cli.hpp>
#include <cxxopts.hpp>
#include <interpreter/fat16.hpp>

auto main(int argc, char** argv) -> int {
  cxxopts::Options options("fat16-interpreter", "A interpreter for FAT16 filesystem\n");

  // change positional parameters in the help message
  options.positional_help("[fat16-file]");

  // clang-format off
  options.add_options()
    ("h,help", "Print help")
    ("file", "Path to the file to interpret", cxxopts::value<std::string>());
  // clang-format on

  options.parse_positional({"file"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    fmt::print("{}\n", options.help());
    return 0;
  }

  if (result.count("file")) {
    interpreter::Fat16 fat16(result["file"].as<std::string>());
    Cli cli(&fat16);
    cli.run();
  } else {
    fmt::print("{}\n", options.help());
    throw std::runtime_error("No file provided");
  }

  return 0;
}
