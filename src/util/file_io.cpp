#include"file_io.hpp"
#include<filesystem>
#include<string>

std::string read_file(std::filesystem::path path) {

    std::fstream file("input.txt");
    if(!file.good()) {
        const std::string err_message = "Failed to read '" + path.string() + "'";
        throw std::exception(err_message.c_str());
    }

    std::string file_content = "";
    std::string line = "";

    while(std::getline(file, line)) {
        file_content += line + "\n";
    }
    file.close();

    return file_content;
}