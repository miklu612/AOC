#include<iostream>
#include"../util/file_io.hpp"
#include"../util/vec2.hpp"
#include"../util/string_util.hpp"

// Theory time
//
// The statements are basically just the following math expressions.
//   a.x * a.times + b.x * b.times = prize.x
//   a.y * a.times + b.y * b.times = prize.y
//
// This can be simplified using some algebra
//   a.x * a.times + b.x * b.times = prize.x
//   a.x * a.times = prize.x - b.x * b.times
//   a.times = (prize.x - b.x * b.times) / a.x
//
// This doesn't give out the answer on its own, but we can use the other
// equation to get the value for b.times
//   a.y * a.times + b.y * b.times = prize.y
//   b.y * b.times = prize.y - a.y * a.times
//   b.times = (prize.y - a.y * a.times) / b.y
//
// Replace b.times
//   a.times = (prize.x - b.x * b.times) / a.x
//   a.times = (prize.x - b.x * ((prize.y - a.y * a.times) / b.y)) / a.x
//   a.times = (prize.x - b.x * prize.y - (b.x * (a.y * a.times)) / (b.x * b.y)) / a.x
//   a.times = (prize.x - b.x * prize.y - (b.x * (a.y * a.times)) / (b.x * b.y)) / a.x
//
// Ignore the stuff above. The equation is 
//   x = (-c2 * b1 + c1 * b2) / (a1 * b2 - a2 * b1)
// 
//   a1 = a.x
//   a2 = a.y
//   b1 = b.x
//   b2 = b.y
//   c1 = expect.x
//   c2 = expect.y
//   x = a button press time
//   

using IntType = int32_t;

class GameParameters {
    public:
        Vec2<IntType> a;
        Vec2<IntType> b;
        Vec2<IntType> target;
};

Vec2<IntType> get_values(const std::vector<std::string>::const_iterator iterator) {
    const auto comma_iter = std::find(iterator->begin(), iterator->end(), ',');
    std::string x(
        std::next(iterator->begin(), 12),
        comma_iter
    );
    std::string y(
        std::next(comma_iter, 4),
        iterator->end()
    );
    return Vec2<IntType>(
        (IntType) std::stoi(x),
        (IntType) std::stoi(y)
    );
}

std::vector<GameParameters> parse_input(const std::vector<std::string>& lines) {
    auto iterator = lines.begin();
    std::vector<GameParameters> games;
    while(iterator < lines.end()) {

        auto a = get_values(iterator);
        iterator = std::next(iterator);

        auto b = get_values(iterator);
        iterator = std::next(iterator);

        auto start = std::find(iterator->begin(), iterator->end(), '=');
        auto comma = std::find(iterator->begin(), iterator->end(), ',');
        start = std::next(start);

        auto prize_x_string = std::string(start, comma);
        auto prize_y_string = std::string(std::next(comma, 4), iterator->end());

        Vec2<IntType> prize_vector(
            std::stoi(prize_x_string),
            std::stoi(prize_y_string)
        );
        
        GameParameters game;
        game.a = a;
        game.b = b;
        game.target = prize_vector;
        games.push_back(game);

        iterator = std::next(iterator, 2);
    }
    return games;
}

int main() {

    auto input = read_file("input.txt");
    auto lines = StringUtil::split_by_character(input, '\n');

    auto games = parse_input(lines);

    IntType answer_1 = 0;
    for(const auto& game : games) {

        float a1 = game.a.x;
        float a2 = game.a.y;
        float b1 = game.b.x;
        float b2 = game.b.y;
        float c1 = game.target.x;
        float c2 = game.target.y;

        float x = (-c2 * b1 + c1 * b2) / (a1 * b2 - a2 * b1);
        float y = (c2 - x * a2) / b2;

        if(floor(x) == x && floor(y) == y) {
            answer_1 += x * 3 + y;
            std::cout << x << "\t" << y << "\n";
        }

    }

    std::cout << "Answer 1: " << answer_1 << "\n";
    std::cout << "Answer 2: " << "Not implemented" << "\n";


    return 0;
}