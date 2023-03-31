#pragma once

#include <string>

namespace interpreter {
  class Fat16 {
  public:
    typedef struct fat16_BS {
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

    } __attribute__((packed)) fat16_BS_t;

    Fat16(std::string fila_name);
    ~Fat16() = default;

    void sample_function();
    void unimplemented_logic();

    Fat16* print_boot_record();

  private:
    fat16_BS_t boot_record;
    unsigned int fat_begin;
    unsigned int root_dir_begin;
    unsigned int data_sector_begin;

    // all of them store the byte where the section begins
    Fat16* define_fat_begin();
    Fat16* define_root_dir_begin();
    Fat16* define_data_sector_begin();
  };
};  // namespace interpreter
