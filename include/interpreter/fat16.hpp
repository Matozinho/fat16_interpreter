#pragma once

#include <fmt/core.h>

#include <fstream>
#include <string>
#include <vector>

namespace interpreter {
  class Fat16 {
  public:
    struct fat_BS {
      unsigned char bootjmp[3];           // 0  | 3
      unsigned char oem_name[8];          // 3  | 8
      unsigned short bytes_per_sector;    // 11 | 2
      unsigned char sectors_per_cluster;  // 13 | 1
      unsigned short reserved_sectors;    // 14 | 2
      unsigned char fat_count;            // 16 | 1 Number of File Allocation Tables (FAT's)
      unsigned short root_entry_count;    // 17 | 2 Number of root directory entries
      unsigned short total_sectors_16;    // 19 | 2 The total sectors in the logical volume.
      unsigned char media_type;           // 21 | 1
      unsigned short sectors_per_fat_16;  // 22 | 2 	Number of sectors per FAT. (FAT12/16 only)
      unsigned short sectors_per_track;   // 24 | 2
      unsigned short head_side_count;     // 26 | 2 Number of heads or sides on the storage media.
      unsigned int hidden_sector_count;   // 28 | 4
      unsigned int total_sectors_32;      // 32 | 4

      // this will be cast to it's specific type once the driver actually knows what type of FAT
      // this is.
      unsigned char extended_section[54];

    } __attribute__((packed));

    struct fat_dir_entry {
      unsigned char name[8];
      unsigned char extension[3];
      unsigned char attribute;
      unsigned char reserved_windows_nt;
      unsigned char creation_time_in_tenths_of_second;
      unsigned short creation_time;
      unsigned short creation_date;
      unsigned short last_access;
      unsigned short high_first_cluster;
      unsigned short last_modification_time;
      unsigned short last_modification_date;
      unsigned short low_first_cluster;
      unsigned int file_size;

    } __attribute__((packed));

    enum class FileType : unsigned char { DIRECTORY = 0x10, ARCHIVE = 0x20 };

    Fat16(std::string file_name);
    ~Fat16();

    inline Fat16* unimplemented_logic() {
      fmt::print("Unimplemented logic\n");
      return this;
    }
    Fat16* print_boot_record();
    std::vector<fat_dir_entry> get_dir_entries(unsigned short first_cluster);
    std::vector<unsigned char> get_file(fat_dir_entry file);
    std::vector<unsigned short> get_cluster_chain(unsigned short first_cluster);

    // getters
    fat_BS* get_boot_record() { return &this->boot_record; };
    unsigned int get_fat_begin() { return this->fat_begin; };
    unsigned int get_root_dir_begin() { return this->root_dir_begin; };
    unsigned int get_data_sector_begin() { return this->data_sector_begin; };
    unsigned int get_cluster_addr(unsigned short cluster);

  private:
    fat_BS boot_record;
    std::ifstream* file;

    unsigned int fat_begin;
    unsigned int root_dir_begin;
    unsigned int data_sector_begin;

    // all of them store the byte where the section begins
    Fat16* define_fat_begin();
    Fat16* define_root_dir_begin();
    Fat16* define_data_sector_begin();
  };
};  // namespace interpreter
