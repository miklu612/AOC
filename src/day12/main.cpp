#include<iostream>
#include<map>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"
#include"../util/vec2.hpp"


using IntType = int64_t;

struct Sides {
    std::vector<std::vector<std::pair<Vec2<IntType>, Vec2<IntType>>>> vertical;
    std::vector<std::vector<std::pair<Vec2<IntType>, Vec2<IntType>>>> horizontal;
};

class Plot {
    public:

        std::vector<std::pair<Vec2<IntType>, Vec2<IntType>>> get_perimeter() const {
            std::vector<std::pair<Vec2<IntType>, Vec2<IntType>>> perimeter;
            perimeter.reserve(plot.size());
            for(const auto& tile : plot) {
                for(const auto direction : create_direction_vectors<IntType>()) {
                    const auto find_result = std::find(plot.begin(), plot.end(), direction+tile);
                    if(find_result == plot.end()) {
                        perimeter.push_back({
                            tile,
                            direction+tile
                        });
                    }
                }
            }
            perimeter.shrink_to_fit();
            return perimeter;
        }

        IntType calculate_perimeter() const {
            IntType perimeter = 0;
            for(const auto& tile : plot) {
                for(const auto direction : create_direction_vectors<IntType>()) {
                    const auto find_result = std::find(plot.begin(), plot.end(), direction+tile);
                    if(find_result == plot.end()) {
                        perimeter += 1;
                    }
                }
            }
            return perimeter;
        }

        IntType get_sides() const {

            const auto perimeter = get_perimeter();
            Sides sides;

            for(const auto& perimeter_tile : perimeter) {
                // Horizontal
                if((perimeter_tile.first - perimeter_tile.second).abs() == Vec2<IntType>(1, 0)) {
                    auto& vertical = sides.vertical;

                    const auto existing = std::find_if(vertical.begin(), vertical.end(), 
                        [&perimeter_tile](const auto& side){
                            return std::find(side.begin(), side.end(), perimeter_tile) != side.end();
                        }) != vertical.end();

                    if(existing) {
                        continue;
                    }

                    bool handle_up = true;
                    bool handle_down = true;
                    std::vector<std::pair<Vec2<IntType>, Vec2<IntType>>> side{perimeter_tile};
                    for(IntType i = 1 ; handle_up || handle_down ; i++) {
                        const auto up = std::find_if(perimeter.begin(), perimeter.end(), 
                            [&perimeter_tile, &i](const auto& tile){
                                const auto delta_first = perimeter_tile.first + Vec2<IntType>(0, 1) * i;
                                const auto delta_second = perimeter_tile.second + Vec2<IntType>(0, 1) * i;
                                return delta_first == tile.first && delta_second == tile.second;
                            });
                        const auto down = std::find_if(perimeter.begin(), perimeter.end(), 
                            [&perimeter_tile, &i](const auto& tile){
                                const auto delta_first = perimeter_tile.first + Vec2<IntType>(0, -1) * i;
                                const auto delta_second = perimeter_tile.second + Vec2<IntType>(0, -1) * i;
                                return delta_first == tile.first && delta_second == tile.second;
                            });
                        if(up != perimeter.end() && handle_up) {
                            side.push_back(*up);
                        } 
                        else {
                            handle_up = false;
                        }
                        if(down != perimeter.end() && handle_down) {
                            side.push_back(*down);
                        } 
                        else {
                            handle_down = false;
                        }
                    }
                    vertical.push_back(side);
                }

                // Vertical
                if((perimeter_tile.first - perimeter_tile.second).abs() == Vec2<IntType>(0, 1)) {
                    auto& horizontal = sides.horizontal;

                    const auto existing = std::find_if(horizontal.begin(), horizontal.end(), 
                        [&perimeter_tile](const auto& side){
                            return std::find(side.begin(), side.end(), perimeter_tile) != side.end();
                        }) != horizontal.end();

                    if(existing) {
                        continue;
                    }

                    bool handle_right = true;
                    bool handle_left = true;
                    std::vector<std::pair<Vec2<IntType>, Vec2<IntType>>> vector{perimeter_tile};
                    for(IntType i = 1 ; handle_left || handle_right ; i++) {
                        const auto right = std::find_if(perimeter.begin(), perimeter.end(), 
                            [&perimeter_tile, &i](const auto& tile){
                                const auto delta_first = perimeter_tile.first + Vec2<IntType>(1, 0) * i;
                                const auto delta_second = perimeter_tile.second + Vec2<IntType>(1, 0) * i;
                                return delta_first == tile.first && delta_second == tile.second;
                            });
                        const auto left = std::find_if(perimeter.begin(), perimeter.end(), 
                            [&perimeter_tile, &i](const auto& tile){
                                const auto delta_first = perimeter_tile.first + Vec2<IntType>(-1, 0) * i;
                                const auto delta_second = perimeter_tile.second + Vec2<IntType>(-1, 0) * i;
                                return delta_first == tile.first && delta_second == tile.second;
                            });
                        if(right != perimeter.end() && handle_right) {
                            vector.push_back(*right);
                        } 
                        else {
                            handle_right = false;
                        }
                        if(left != perimeter.end() && handle_left) {
                            vector.push_back(*left);
                        } 
                        else {
                            handle_left = false;
                        }
                    }
                    horizontal.push_back(vector);
                    
                }
            }

            return sides.vertical.size() + sides.horizontal.size();

        }

        IntType calculate_price() const {
            return calculate_perimeter() * plot.size();
        }

        std::vector<Vec2<IntType>> plot;
        char plant;
};


Plot collect_plot(std::vector<std::string>& map, Vec2<IntType> starting_at) {

    const auto bounds = Vec2<IntType>(
        map[0].length(),
        map.size()
    );

    Plot output;
    output.plot.reserve(1024);
    output.plant = map.at(starting_at.y).at(starting_at.x);

    std::vector<Vec2<IntType>> land;
    land.reserve(1024);
    land.push_back(starting_at);

    while(land.size() > 0) {
        const auto size = land.size();
        for(IntType i = 0 ; i < size ; i++) {

            const auto& coordinates = land[i];
            map[coordinates.y][coordinates.x] = '.';
            output.plot.push_back(coordinates);

            for(auto direction_vector : create_direction_vectors<IntType>()) {
                auto const new_coord = coordinates + direction_vector;
                if(new_coord.within_box(bounds)) {
                    if(map[new_coord.y][new_coord.x] == output.plant) {
                        map[new_coord.y][new_coord.x] = '.';
                        land.push_back(new_coord);
                    }
                }
            }
        }
        land.erase(land.begin(), std::next(land.begin(), size));
    }

    output.plot.shrink_to_fit();
    return output;
}

int main() {

    try {
        auto input = read_file("input.txt");
        auto lines = StringUtil::split_by_character(input, '\n');

        for(auto line : lines) {
            std::cout << line << "\n";
        }

        std::vector<Plot> plots;
        plots.reserve(1024);

        for(int i = 0 ; i < lines.size() ; i++) {
            for(int j = 0 ; j < lines[i].length() ; j++) {
                if(lines[i][j] == '.') {
                    continue;
                }
                plots.push_back(collect_plot(lines, Vec2((IntType)j, (IntType)i)));
            }
        }

        std::cout << "Plant\tPrice 1\tPrice 2\tArea\tSides\tPerimeter\n";

        IntType answer_1 = 0;
        IntType answer_2 = 0;
        for(const auto& plot : plots) {
            const auto price = plot.calculate_price();
            const auto perimeter = plot.calculate_perimeter();
            const auto area = plot.plot.size();
            const auto sides = plot.get_sides();
            const auto price_2 = sides * area;
            std::cout 
                << plot.plant << "\t" 
                << price << "\t" 
                << price_2 << "\t" 
                << area << "\t" 
                << sides << "\t" 
                << perimeter << "\n";
            answer_1 += price;
            answer_2 += price_2;
        }


        std::cout << "Answer 1: " << answer_1 << "\n";
        std::cout << "Answer 2: " << answer_2 << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}