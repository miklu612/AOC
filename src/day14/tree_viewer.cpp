#include<iostream>
#include<thread>
#include"../util/file_io.hpp"
#include"../util/vec2.hpp"
#include"../util/string_util.hpp"


class Robot {
    public:
        Vec2<int> position;
        Vec2<int> velocity;
};

int main() {

    auto input = read_file("input.txt");
    auto lines = StringUtil::split_by_character(input, '\n');

    std::vector<Robot> robots;

    const int width = 101;
    const int height = 103;
    
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
        
        Robot robot;
        robot.position = Vec2<int>(position_x, position_y);
        robot.velocity = Vec2<int>(velocity_x, velocity_y);
        robots.push_back(robot);

    }


    const int upper_bound = 2000000;
    int max_max_in_line = 0;
    for(int i = 0 ; i < upper_bound ; i++) {


        for(auto& robot : robots) {
            robot.position.x += robot.velocity.x;
            robot.position.y += robot.velocity.y;

            if(robot.position.x >= width) {
                robot.position.x = robot.position.x - width;
            }
            else if(robot.position.x < 0) {
                robot.position.x = width + robot.position.x;
            }

            if(robot.position.y >= height) {
                robot.position.y = robot.position.y - height;
            }
            else if(robot.position.y < 0) {
                robot.position.y = height + robot.position.y;
            }

        }


        int in_line = 0;
        int max_in_line = 0;
        for(int j = 0 ; j < width ; j++) {
            in_line = 0;
            for(int k = 0 ; k < height ; k++) {
                auto exists = std::find_if(robots.begin(), robots.end(),
                    [&j, &k](const auto& robot){
                        return robot.position == Vec2(j, k);
                    }) != robots.end();
                if(exists) {
                    in_line += 1;
                }
            }
            if(in_line >= height/2) {
                break;
            }
            else {
                max_in_line = std::max(in_line, max_in_line);
                in_line = 0;
            }
        }
        
        const int previous = max_max_in_line;
        max_max_in_line = std::max(max_max_in_line, max_in_line);
        if(in_line == 0 && max_max_in_line == previous) {
            continue;
        }


        std::string output;
        for(int j = 0 ; j < height ; j++) {
            for(int k = 0 ; k < width ; k++) {
                int64_t robot_count = std::count_if(
                    robots.begin(), 
                    robots.end(), 
                    [&k, &j](const auto& robot){
                        return Vec2<int>(k, j) == robot.position;
                    });
                if(robot_count > 0) {
                    output += std::to_string(robot_count);
                }
                else {
                    output += '.';
                }
            }
            output += "\n";
        }

        std::cout << output;
        std::cout << i << "\n";

    }

}