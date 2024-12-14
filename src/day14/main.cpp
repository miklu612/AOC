#include<iostream>
#include"../util/file_io.hpp"
#include"../util/vec2.hpp"
#include"../util/string_util.hpp"



int main() {


    try {
        auto input = read_file("input.txt");
        auto lines = StringUtil::split_by_character(input, '\n');

        std::array quadrants = {
            std::array{0, 0},
            std::array{0, 0}
        };

        std::vector<Vec2<int64_t>> positions;

        const int64_t width = 101;
        const int64_t height = 103;

        for(const auto& line : lines) {

            std::cout << line << "\n";

            auto equals = std::find(line.begin(), line.end(), '=');
            auto comma = std::find(line.begin(), line.end(), ',');
            auto space = std::find(comma, line.end(), ' ');

            std::string position_x_string(std::next(equals, 1), comma);
            std::string position_y_string(std::next(comma, 1), space);

            equals = std::find(space, line.end(), '=');
            comma = std::find(space, line.end(), ',');

            std::string velocity_x_string(std::next(equals, 1), comma);
            std::string velocity_y_string(std::next(comma, 1), line.end());

            int64_t position_x = std::stoi(position_x_string);
            int64_t position_y = std::stoi(position_y_string);
            int64_t velocity_x = std::stoi(velocity_x_string);
            int64_t velocity_y = std::stoi(velocity_y_string);


            for(int64_t i = 0 ; i < 100 ; i++) {
                position_x += velocity_x;
                position_y += velocity_y;

                if(position_x >= width) {
                    position_x = position_x - width;
                }
                else if(position_x < 0) {
                    position_x = width + position_x;
                }

                if(position_y >= height) {
                    position_y = position_y - height;
                }
                else if(position_y < 0) {
                    position_y = height + position_y;
                }

            }

            positions.emplace_back(position_x, position_y);
            if(position_y != std::ceil(height/2) && position_x != std::ceil(width/2)) {
                quadrants[position_y > std::floor(height/2)][position_x > std::floor(width/2)] += 1;
            }

        }

        std::cout
            << quadrants[0][0] << "\t" << quadrants[0][1] << "\n"
            << quadrants[1][0] << "\t" << quadrants[1][1] << "\n";

        for(int64_t i = 0 ; i < height ; i++) {
            for(int64_t j = 0 ; j < width ; j++) {
                int64_t robot_count = std::count(
                    positions.begin(), 
                    positions.end(), 
                    Vec2(j, i));
                if(robot_count > 0) {
                    std::cout << robot_count;
                }
                else {
                    std::cout << '.';
                }
            }
            std::cout << "\n";
        }

        for(auto position : positions) {
            std::cout << position << "\n";
        }

        int64_t answer = 
            quadrants[0][0] * quadrants[0][1] *
            quadrants[1][0] * quadrants[1][1];
    
        std::cout << "Answer 1: " << answer << "\n";
        std::cout << "Answer 2: " << "Unimplemented" << "\n";
    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}