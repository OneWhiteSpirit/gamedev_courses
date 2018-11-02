#include "io_manager.hpp"

#include <fstream>

bool io_manager::read_file_to_buffer(const char* file_path, std::vector<unsigned char>& buffer)
{
    std::ifstream file(file_path, std::ios_base::binary);
    if (file.fail()) {
        perror(file_path);
        return false;
    }

    file.seekg(0, std::ios_base::end);

    long file_size = file.tellg();

    file.seekg(0, std::ios_base::beg);

    file_size -= file.tellg();

    buffer.resize(static_cast<unsigned long>(file_size));
    file.read(reinterpret_cast<char*>(&(buffer[0])), file_size);
    file.close();

    return true;
}
