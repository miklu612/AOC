#include<iostream>

#include"../util/file_io.hpp"
#include"../util/string_util.hpp"
#include"../util/vec2.hpp"

class Antenna {
    public:
        Antenna() :
            signal(0),
            position(Vec2(0, 0))
        {
        }
        char signal;
        Vec2<int> position;
};

template<typename T>
Vec2<T> get_antinode_vector(const Vec2<T>& antenna_1, const Vec2<T>& antenna_2) {
    const auto delta_position = antenna_2 - antenna_1;
    return delta_position;
}

bool part_1_check(const std::vector<Vec2<int>>& antinodes, const Vec2<int>& position, int w, int h) {
    if(!position.within_box(w, h)) {
        return false;
    }
    return std::find(antinodes.cbegin(), antinodes.cend(), position) == antinodes.cend();
}


int main() {

    auto input = read_file("./input.txt");
    auto lines = StringUtil::split_by_character(input, '\n');

    std::vector<Antenna> antennas;
    std::vector<char> unique_signals;
    for(int i = 0 ; i < lines.size() ; i++) {
        for(int j = 0 ; j < lines[i].length() ; j++) {
            if(lines[i][j] != '.') {
                Antenna antenna;
                antenna.position = Vec2((int) j, (int) i);
                antenna.signal = lines[i][j];
                antennas.push_back(antenna);
                if(std::find(unique_signals.begin(), unique_signals.end(), lines[i][j]) == unique_signals.end()) {
                    unique_signals.push_back(lines[i][j]);
                }
            }
        }
    }

    std::cout << "Antenna count: " << antennas.size() << "\n";

    const int world_width = lines[0].size();
    const int world_height = lines.size();

    int answer_1 = 0;
    int answer_2 = 0;

    std::vector<Vec2<int>> anti_nodes;
    std::vector<Vec2<int>> part_1_antinodes;

    for(auto unique_signal : unique_signals) {

        auto antenna = std::find_if(antennas.begin(), antennas.end(), 
            [&unique_signal](const auto& antenna){
                return antenna.signal == unique_signal;
            });

        while(antenna < antennas.end()) {

            auto next_antenna = std::find_if(std::next(antenna), antennas.end(), 
                [&unique_signal](const auto& antenna){
                    return antenna.signal == unique_signal;
                });

            while(next_antenna < antennas.end()) {
                if(next_antenna >= antennas.end()) {
                    break;
                }
                std::cout << "Calculating antinode positions\n";

                const auto antinode_vector_1 = get_antinode_vector(antenna->position, next_antenna->position);
                const auto antinode_vector_2 = get_antinode_vector(next_antenna->position, antenna->position);

                if(part_1_check(part_1_antinodes, antinode_vector_1 + next_antenna->position, world_width, world_height)) {
                    part_1_antinodes.push_back(antinode_vector_1 + next_antenna->position);
                    answer_1 += 1;
                }
                if(part_1_check(part_1_antinodes, antinode_vector_2 + antenna->position, world_width, world_height)) {
                    part_1_antinodes.push_back(antinode_vector_2 + antenna->position);
                    answer_1 += 1;
                }

                auto position_1 = antinode_vector_1 + antenna->position;
                while(position_1.within_box(world_width, world_height)) {
                    if(std::find(anti_nodes.begin(), anti_nodes.end(), position_1) == anti_nodes.end()) {
                        answer_2 += 1;
                        anti_nodes.push_back(position_1);
                    }
                    position_1 += antinode_vector_1;
                }

                auto position_2 = antinode_vector_2 + next_antenna->position;
                while(position_2.within_box(world_width, world_height)) {
                    if(std::find(anti_nodes.begin(), anti_nodes.end(), position_2) == anti_nodes.end()) {
                        answer_2 += 1;
                        anti_nodes.push_back(position_2);
                    }
                    position_2 += antinode_vector_2;
                }

                next_antenna = std::find_if(std::next(next_antenna), antennas.end(), 
                    [&unique_signal](const auto& antenna){
                        return antenna.signal == unique_signal;
                    });

            }

            antenna = std::find_if(std::next(antenna), antennas.end(), 
                [&unique_signal](const auto& antenna){
                    return antenna.signal == unique_signal;
                });
        }

    }

    for(int i = 0 ; i < world_height ; i++) {
        for(int j = 0 ; j < world_width ; j++) {
            for(auto anti : anti_nodes) {
                if(anti == Vec2((int)j, (int)i)) {
                    std::cout << '#';
                    goto no_dot_print;
                }
            }
            std::cout << '.';
no_dot_print:
            continue;
        }
        std::cout << "\n";
    }

    std::cout << "Answer 1: " << answer_1 << "\n";
    std::cout << "Answer 2: " << answer_2 << "\n";


    return 0;

}