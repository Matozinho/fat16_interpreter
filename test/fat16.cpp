#include <fmt/core.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <interpreter/fat.hpp>
#include <interpreter/fat16.hpp>

bool compare_fat_bs(const interpreter::Fat16::fat16_BS_t& a,
                    const interpreter::Fat16::fat16_BS_t& b) {
  return (a.bytes_per_sector == b.bytes_per_sector && a.sectors_per_cluster == b.sectors_per_cluster
          && a.reserved_sectors == b.reserved_sectors && a.fat_count == b.fat_count
          && a.root_entry_count == b.root_entry_count && a.total_sectors_16 == b.total_sectors_16
          && a.media_type == b.media_type && a.sectors_per_fat_16 == b.sectors_per_fat_16
          && a.sectors_per_track == b.sectors_per_track && a.head_side_count == b.head_side_count
          && a.hidden_sector_count == b.hidden_sector_count);
}

TEST(Fat16, ReadBootRecord_4SectorsPerCluster) {
  interpreter::Fat16 fat16_4sectors("assets/fat16_4sectorpercluster.img");
  interpreter::Fat16::fat16_BS_t boot_record_4sectors = {
      {0x90, 0x3c, 0xeb},                                // bootjmp
      {0x6d, 0x6b, 0x66, 0x73, 0x2e, 0x66, 0x61, 0x74},  // oem_name
      static_cast<unsigned short>(0x0200),               // bytes_per_sector
      0x04,                                              // sectors_per_cluster
      0x0004,                                            // reserved_sectors
      0x02,                                              // fat_count
      static_cast<unsigned short>(0x0200),               // root_entry_count
      static_cast<unsigned short>(0xA000),               // total_sectors_16
      0xf8,                                              // media_type
      0x0028,                                            // sectors_per_fat_16
      0x0020,                                            // sectors_per_track
      0x0040,                                            // head_side_count
      0x00000000,                                        // hidden_sector_count
      0x00000000,                                        // total_sectors_32
  };

  EXPECT_TRUE(compare_fat_bs(boot_record_4sectors, fat16_4sectors.get_boot_record()));
}

TEST(Fat16, ReadBootRecord_1SectorsPerCluster) {
  interpreter::Fat16 fat16_4sectors("assets/fat16_1sectorpercluster.img");
  interpreter::Fat16::fat16_BS_t boot_record_4sectors = {
      {0xeb, 0x3c, 0x90},                                // bootjmp
      {0x6d, 0x6b, 0x64, 0x6f, 0x73, 0x66, 0x73, 0x00},  // oem_name
      static_cast<unsigned short>(0x0200),               // bytes_per_sector
      0x01,                                              // sectors_per_cluster
      0x0001,                                            // reserved_sectors
      0x02,                                              // fat_count
      static_cast<unsigned short>(0x0200),               // root_entry_count
      static_cast<unsigned short>(0x9c40),
      0xf8,        // media_type
      0x009b,      // sectors_per_fat_16
      0x0020,      // sectors_per_track
      0x0040,      // head_side_count
      0x00000000,  // hidden_sector_count
      0x00000000,  // total_sectors_32
  };

  EXPECT_TRUE(compare_fat_bs(boot_record_4sectors, fat16_4sectors.get_boot_record()));
}

// TEST(SampleTest, ShouldThrow) { EXPECT_THROW(sample::unimplemented_logic(), std::runtime_error);
// }
