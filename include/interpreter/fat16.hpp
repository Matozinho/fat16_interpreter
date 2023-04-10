#pragma once

#include <fmt/core.h>

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace interpreter {
  class Fat16 {
  public:
    struct fat_BS {
      uint8_t bootjmp[3];            // 0  | 3
      uint8_t oem_name[8];           // 3  | 8
      uint16_t bytes_per_sector;     // 11 | 2
      uint8_t sectors_per_cluster;   // 13 | 1
      uint16_t reserved_sectors;     // 14 | 2
      uint8_t fat_count;             // 16 | 1 Number of File Allocation Tables (FAT's)
      uint16_t root_entry_count;     // 17 | 2 Number of root directory entries
      uint16_t total_sectors_16;     // 19 | 2 The total sectors in the logical volume.
      uint8_t media_type;            // 21 | 1
      uint16_t sectors_per_fat_16;   // 22 | 2 	Number of sectors per FAT. (FAT12/16 only)
      uint16_t sectors_per_track;    // 24 | 2
      uint16_t head_side_count;      // 26 | 2 Number of heads or sides on the storage media.
      uint32_t hidden_sector_count;  // 28 | 4
      uint32_t total_sectors_32;     // 32 | 4

      // this will be cast to it's specific type once the driver actually knows what type of FAT
      // this is.
      uint8_t extended_section[54];

    } __attribute__((packed));

    struct fat_dir_entry {
      uint8_t name[8];
      uint8_t extension[3];
      uint8_t attribute;
      uint8_t reserved_windows_nt;
      uint8_t creation_time_in_tenths_of_second;
      uint16_t creation_time;
      uint16_t creation_date;
      uint16_t last_access;
      uint16_t high_first_cluster;
      uint16_t last_modification_time;
      uint16_t last_modification_date;
      uint16_t low_first_cluster;
      uint32_t file_size;

    } __attribute__((packed));

    enum class FileType : uint8_t { DIRECTORY = 0x10, ARCHIVE = 0x20 };

    Fat16(std::string file_name);
    ~Fat16();

    inline Fat16* unimplemented_logic() {
      fmt::print("Unimplemented logic\n");
      return this;
    }
    Fat16* print_boot_record();
    std::vector<fat_dir_entry> get_dir_entries(uint16_t first_cluster);
    std::vector<uint8_t> get_file(fat_dir_entry file);
    std::vector<uint16_t> get_cluster_chain(uint16_t first_cluster);

    // getters
    fat_BS* get_boot_record() { return &this->boot_record; };
    uint32_t get_fat_begin() { return this->fat_begin; };
    uint32_t get_root_dir_begin() { return this->root_dir_begin; };
    uint32_t get_data_sector_begin() { return this->data_sector_begin; };
    uint32_t get_cluster_addr(uint16_t cluster);

  private:
    fat_BS boot_record;
    std::ifstream* file;

    uint32_t fat_begin;
    uint32_t root_dir_begin;
    uint32_t data_sector_begin;

    // all of them store the byte where the section begins
    Fat16* define_fat_begin();
    Fat16* define_root_dir_begin();
    Fat16* define_data_sector_begin();
  };
};  // namespace interpreter
