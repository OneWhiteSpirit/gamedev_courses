#pragma once

#include <vector>

class io_manager {
public:
    io_manager() = default;

    static bool read_file_to_buffer(const char* file_path, std::vector<unsigned char>& buffer);
};
