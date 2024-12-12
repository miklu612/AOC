#include<iostream>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"
#include"../util/vec2.hpp"


using IntType = int64_t;

class Plot {
    public:

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

        std::cout << "Plant\tPrice\tArea\tPerimeter\n";

        IntType answer_1 = 0;
        for(const auto& plot : plots) {
            const auto price = plot.calculate_price();
            const auto perimeter = plot.calculate_perimeter();
            const auto area = plot.plot.size();
            std::cout 
                << plot.plant << "\t" 
                << price << "\t" 
                << area << "\t" 
                << perimeter << "\n";
            answer_1 += price;
        }


        std::cout << "Answer 1: " << answer_1 << "\n";
        std::cout << "Answer 2: " << "Unimplemented" << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}