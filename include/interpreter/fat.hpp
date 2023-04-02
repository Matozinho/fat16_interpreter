#pragma once

#include <fmt/core.h>

#include <stdexcept>

namespace interpreter {
  class Fat {
  public:
    inline Fat* unimplemented_logic() {
      fmt::print("Unimplemented logic\n");
      return this;
    }

    virtual Fat* print_boot_record() = 0;
    virtual Fat* list_directory() = 0;
  };
}  // namespace interpreter
