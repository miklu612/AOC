#ifndef file_io_hpp
#define file_io_hpp

#include<fstream>
#include<filesystem>

std::string read_file(std::filesystem::path path);

#endif