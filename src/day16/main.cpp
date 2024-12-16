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
        int runs;
        Direction direction;
        std::vector<Vec2<int>> path;
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
        starter.runs = 0;
        starter.path = {start};
        std::vector<Navigator> navigators;
        navigators.push_back(starter);

        int answer_1 = maze[end.y][end.x];
        int answer_2 = 999999;
        int same_scores = 0;
        int threshold = 1000;
        int min_score = INT_MAX;
        std::vector<Vec2<int>> best_paths;

        do {
            std::vector<Navigator> new_navigators;
            for(const auto& navigator : navigators) {
                if(navigator.position == end) {
                    if(navigator.score < min_score) {
                        min_score = navigator.score;
                        best_paths.clear();
                        for(const auto& path : navigator.path) {
                            best_paths.push_back(path);
                        }
                    }
                    else if(navigator.score == min_score) {
                        for(const auto& path : navigator.path) {
                            const auto exists = std::find(
                                best_paths.begin(), best_paths.end(),
                                path) != best_paths.end();
                            if(!exists) {
                                best_paths.push_back(path);
                            }
                        }
                    }
                    continue;
                }
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
                    if(new_value - threshold <= value) {
                        if(new_value < value) {
                            value = new_value;
                        }
                        Navigator new_navigator;
                        new_navigator.direction = dir.get_direction();
                        new_navigator.position = new_pos;
                        new_navigator.score = new_value;
                        new_navigator.runs = navigator.runs + 1;
                        new_navigator.path = navigator.path;
                        new_navigator.path.push_back(new_pos);
                        new_navigators.push_back(new_navigator);
                    }
                }
            }
            navigators = new_navigators;
        } while(navigators.size() > 0);

        answer_1 = maze[end.y][end.x];
        answer_2 = best_paths.size();

        std::cout << "Answer 1: " << answer_1 << "\n";
        std::cout << "Answer 2: " << answer_2 << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }


    return 0;

}