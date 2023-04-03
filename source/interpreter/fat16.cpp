#include <fmt/core.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <interpreter/fat16.hpp>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace interpreter {
  Fat16::Fat16(std::string file_name) {
    this->file = new std::ifstream(file_name, std::ios::binary);

    if (!this->file->is_open()) {
      throw std::runtime_error("Unable to open file: " + file_name);
    }

    this->file->seekg(0, std::ios::beg);
    this->file->read(reinterpret_cast<char*>(&this->boot_record), sizeof(Fat16::fat_BS));

    this->define_fat_begin()->define_root_dir_begin()->define_data_sector_begin();
  };

  Fat16::~Fat16() {
    this->file->close();
    delete this->file;
  }

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

  std::vector<Fat16::fat_dir_entry> Fat16::get_dir_entries(unsigned short first_cluster) {
    std::vector<unsigned short> cluster_chain = this->get_cluster_chain(first_cluster);
    Fat16::fat_dir_entry dir_entry;
    std::vector<Fat16::fat_dir_entry> dir_entries;

    const unsigned int sizeof_dir_entry = sizeof(Fat16::fat_dir_entry);
    unsigned int bytes_per_cluster
        = this->boot_record.bytes_per_sector * this->boot_record.sectors_per_cluster;

    for (unsigned short cluster : cluster_chain) {
      unsigned int dir_begin = this->get_cluster_addr(cluster);

      this->file->seekg(dir_begin, std::ios::beg);
      this->file->read(reinterpret_cast<char*>(&dir_entry), sizeof_dir_entry);
      unsigned int readed_bytes = sizeof(Fat16::fat_dir_entry);

      while (dir_entry.name[0] != 0x00 && readed_bytes < bytes_per_cluster) {
        if (dir_entry.name[0] != 0xE5 && dir_entry.attribute != 0x0F) {
          dir_entries.push_back(dir_entry);
        }

        this->file->read(reinterpret_cast<char*>(&dir_entry), sizeof_dir_entry);
        readed_bytes += sizeof_dir_entry;
      }
    }

    return dir_entries;
  }

  std::vector<unsigned short> Fat16::get_cluster_chain(unsigned short first_cluster) {
    std::vector<unsigned short> cluster_chain;
    unsigned short next_cluster = first_cluster;

    while (next_cluster < 0xFFF8) {
      cluster_chain.push_back(next_cluster);
      this->file->seekg(this->fat_begin + next_cluster * 2, std::ios::beg);
      this->file->read(reinterpret_cast<char*>(&next_cluster), sizeof(unsigned short));
    }

    return cluster_chain;
  }

  unsigned int Fat16::get_cluster_addr(unsigned short cluster) {
    if (cluster == 0) return this->root_dir_begin;
    return this->data_sector_begin
           + (cluster - 2) * this->boot_record.sectors_per_cluster
                 * this->boot_record.bytes_per_sector;
  }

  std::vector<unsigned char> Fat16::get_file(Fat16::fat_dir_entry file_entry) {
    std::vector<unsigned char> file;
    std::vector<unsigned short> cluster_chain
        = this->get_cluster_chain(file_entry.low_first_cluster);

    unsigned int bytes_per_cluster
        = this->boot_record.sectors_per_cluster * this->boot_record.bytes_per_sector;
    unsigned int remaining_bytes = file_entry.file_size;

    for (unsigned short cluster : cluster_chain) {
      unsigned int cluster_begin = this->get_cluster_addr(cluster);

      // place the pointer in the right addr in the mem
      this->file->seekg(cluster_begin, std::ios::beg);

      // validate the size of the cluster to read
      unsigned int bytes_to_read = bytes_per_cluster;
      if (remaining_bytes < bytes_per_cluster) {
        bytes_to_read = remaining_bytes;
      }
      remaining_bytes -= bytes_to_read;

      std::vector<unsigned char> bytes(bytes_to_read);
      this->file->read(reinterpret_cast<char*>(bytes.data()), bytes_to_read);

      file.insert(file.end(), bytes.begin(), bytes.end());
    }

    return file;
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
