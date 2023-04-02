#include <fmt/core.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <interpreter/fat16.hpp>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

namespace interpreter {
  Fat16::Fat16(std::string file_name) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file: " + file_name);
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&this->boot_record), sizeof(Fat16::fat16_BS_t));
    file.close();

    this->define_fat_begin()->define_root_dir_begin()->define_data_sector_begin();
  };

  Fat16* Fat16::define_fat_begin() {
    this->fat_begin = this->boot_record.reserved_sectors * this->boot_record.bytes_per_sector;
    return this;
  }

  Fat16* Fat16::define_root_dir_begin() {
    this->root_dir_begin = this->fat_begin
                           + this->boot_record.fat_count * this->boot_record.sectors_per_fat_16
                                 * this->boot_record.bytes_per_sector;
    return this;
  }

  Fat16* Fat16::define_data_sector_begin() {
    unsigned int root_dir_sectors
        = this->boot_record.root_entry_count * 32 / this->boot_record.bytes_per_sector;

    this->data_sector_begin
        = this->root_dir_begin + root_dir_sectors * this->boot_record.bytes_per_sector;
    return this;
  }

  Fat16* Fat16::list_directory() {
    this->unimplemented_logic();
    return this;
  }

  Fat16* Fat16::print_boot_record() {
    std::cout << std::hex << std::showbase
              << "bytes_per_sector: " << this->boot_record.bytes_per_sector << std::endl;
    std::cout << std::hex << std::showbase << "sectors_per_cluster: "
              << static_cast<unsigned int>(this->boot_record.sectors_per_cluster) << std::endl;
    std::cout << std::hex << std::showbase
              << "reserved_sector_count: " << this->boot_record.reserved_sectors << std::endl;
    std::cout << std::hex << std::showbase
              << "table_count: " << static_cast<unsigned int>(this->boot_record.fat_count)
              << std::endl;
    std::cout << std::hex << std::showbase
              << "root_entry_count: " << this->boot_record.root_entry_count << std::endl;
    std::cout << std::hex << std::showbase
              << "total_sectors_16: " << this->boot_record.total_sectors_16 << std::endl;
    std::cout << std::hex << std::showbase
              << "media_type: " << static_cast<unsigned int>(this->boot_record.media_type)
              << std::endl;
    std::cout << std::hex << std::showbase
              << "table_size_16: " << this->boot_record.sectors_per_fat_16 << std::endl;
    std::cout << std::hex << std::showbase
              << "sectors_per_track: " << this->boot_record.sectors_per_track << std::endl;
    std::cout << std::hex << std::showbase
              << "head_side_count: " << this->boot_record.head_side_count << std::endl;
    std::cout << std::hex << std::showbase
              << "hidden_sector_count: " << this->boot_record.hidden_sector_count << std::endl;
    std::cout << std::hex << std::showbase
              << "total_sectors_32: " << this->boot_record.total_sectors_32 << std::endl;
    fmt::print("====================================\n");
    fmt::print("fat_begin: {}\n", this->fat_begin);
    fmt::print("root_dir_begin: {}\n", this->root_dir_begin);
    fmt::print("data_sector_begin: {}\n", this->data_sector_begin);

    return this;
  }
}  // namespace interpreter
