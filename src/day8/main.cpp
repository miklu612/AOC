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
Vec2<T> get_antinode(const Vec2<T>& antenna_1, const Vec2<T>& antenna_2) {
    const auto delta_position = antenna_2 - antenna_1;
    const auto double_delta_position = delta_position * 2;
    return antenna_1 + double_delta_position;
}

template<typename T>
std::vector<Antenna>::const_iterator get_existing_signal(const std::vector<Antenna>& signals, const Vec2<T>& pos) {
    return std::find_if(signals.cbegin(), signals.cend(), 
        [&pos](const auto& signal){
            return signal.position.x == pos.x && signal.position.y == pos.y;
        });
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

    std::vector<Vec2<int>> anti_nodes;

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

                const auto antinode_1 = get_antinode(antenna->position, next_antenna->position);
                const auto antinode_2 = get_antinode(next_antenna->position, antenna->position);

                if(antinode_1.within_box(world_width, world_height)) {
                    auto existing_antinode = std::find(anti_nodes.begin(), anti_nodes.end(), antinode_1);
                    if(existing_antinode == anti_nodes.end()) {
                        anti_nodes.push_back(antinode_1);
                        answer_1 += 1;
                    }
                }
                if(antinode_2.within_box(world_width, world_height)) {
                    auto existing_antinode = std::find(anti_nodes.begin(), anti_nodes.end(), antinode_2);
                    if(existing_antinode == anti_nodes.end()) {
                        anti_nodes.push_back(antinode_2);
                        answer_1 += 1;
                    }
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
    std::cout << "Answer 2: " << "Not implemented" << "\n";


    return 0;

}