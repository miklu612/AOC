#include<iostream>
#include<map>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"
#include"../util/vec2.hpp"


const int gc_wall = -1;
const int gc_empty = 999999;

class Navigator {
    public:
        Vec2<int> position;
        int score;
        Direction direction;
};

int main() {

    try {
        auto input = read_file("input.txt");
        auto lines = StringUtil::split_by_character(input, '\n');
    
        std::vector<std::vector<int>> maze;
        Vec2<int> start;
        Vec2<int> end;
        for(int i = 0 ; i < lines.size() ; i++) {
            maze.push_back({});
            for(int j = 0 ; j < lines[i].size() ; j++) {
                if(lines[i][j] == '#') {
                    maze[i].push_back(gc_wall);
                }
                else if(lines[i][j] == 'S') {
                    start = Vec2<int>(j, i);
                    maze[i].push_back(gc_empty);
                }
                else if(lines[i][j] == 'E') {
                    end = Vec2<int>(j, i);
                    maze[i].push_back(gc_empty);
                }
                else {
                    maze[i].push_back(gc_empty);
                }
            }
        }

        Navigator starter;
        starter.position = start;
        starter.direction = Direction::Right;
        starter.score = 0;
        std::vector<Navigator> navigators;
        navigators.push_back(starter);

        do {
            std::vector<Navigator> new_navigators;
            for(const auto& navigator : navigators) {
                for(const auto& dir : create_direction_vectors<int>()) {
                    const auto new_pos = navigator.position + dir;
                    if(lines[new_pos.y][new_pos.x] == '#') {
                        continue;
                    }
                    auto& value = maze[new_pos.y][new_pos.x];
                    auto new_value = navigator.score + 1;
                    if(navigator.direction != dir.get_direction()) {
                        new_value += 1000;
                    }
                    if(new_value < value) {
                        value = new_value;
                        Navigator new_navigator;
                        new_navigator.direction = dir.get_direction();
                        new_navigator.position = new_pos;
                        new_navigator.score = new_value;
                        new_navigators.push_back(new_navigator);
                    }
                }
            }
            navigators = new_navigators;
        } while(navigators.size() > 0);

        std::cout << maze[end.y][end.x] << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }


    return 0;

}