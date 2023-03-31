#include <fmt/core.h>
#include <stdio.h>
#include <stdlib.h>

#include <interpreter/fat16.hpp>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

namespace interpreter {
  Fat16::Fat16(std::string file_name) {
    FILE* fp;

    // convert the filename to a c string
    const char* c_file_name = file_name.c_str();

    fp = std::fopen(c_file_name, "rb");
    fseek(fp, 0, SEEK_SET);
    fread(&this->boot_record, sizeof(Fat16::fat16_BS_t), 1, fp);

    // check if the file is a valid fat16 image:
    // https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#BPB20_OFS_0Ah
    // if (this->boot_record.media_type != 0x04) throw std::runtime_error("Invalid FAT16 image");

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

  void Fat16::sample_function() { fmt::print("Hello, {}!\n", "world"); }

  void unimplemented_logic() { throw std::runtime_error("Unimplemented logic"); }

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

    return this;
  }
}  // namespace interpreter
