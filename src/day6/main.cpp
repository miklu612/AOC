#include<iostream>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"

// With this order of directions, rotation to the right should be as easy as
// `(direction + 1) % 4`
enum class Direction {
    Up = 0,
    Right = 1,
    Down = 2, 
    Left = 3
};

template<typename T>
class Vec2 {
    public:
        Vec2(T x, T y) : x(x), y(y) {
        }
        T x = 0;
        T y = 0;

        friend Vec2<T> operator+(const Vec2<T>& l, const Vec2<T>& r) {
            return Vec2<T>(l.x + r.x, l.y + r.y);
        }

        friend bool operator==(const Vec2<T>& l, const Vec2<T>& r) {
            return 
                l.x == r.x &&
                l.y == r.y;
        }

        friend bool operator!=(const Vec2<T>& l, const Vec2<T>& r) {
            return !(l == r);
        }

};

class Guard {
    public:
        Vec2<int> position = Vec2<int>(0, 0);
        Direction direction = Direction::Up;

        void rotate_right() {
            direction = (Direction) (((int)direction + 1) % 4);
        }

        Vec2<int> get_next_position() {
            switch(direction) {
                case Direction::Up:
                    return position + Vec2(0, -1);
                case Direction::Right:
                    return position + Vec2(1, 0);
                case Direction::Left:
                    return position + Vec2(-1, 0);
                case Direction::Down:
                    return position + Vec2(0, 1);
            }
            std::cout << "Unknown direction\n";
            std::exit(-1);
        }

};

class Block {

};

void visualize_board(const Guard& guard, const std::vector<Vec2<int>>& blocks, int w, int h) {
    std::string output;
    // +1 since there is also a newline
    output.reserve(h * (w+1));
    for(int i = 0 ; i < h ; i++) {
        for(int j = 0 ; j < w ; j++) {
            const Vec2<int> pos(j, i);
            if(std::find(blocks.begin(), blocks.end(), pos) != blocks.end()) {
                output += '#';
            }
            else if(pos == guard.position) {
                output += "?";
            }
            else {
                output += '.';
            }
        }
        output += "\n";
    }
    std::cout << output;
}

int main() {

    std::string input = read_file("./input.txt");

    auto lines = StringUtil::split_by_character(input, '\n');

    std::vector<std::vector<char>> grid(lines.size());
    std::transform(lines.begin(), lines.end(), grid.begin(), [](auto line){
        return std::vector<char>(line.begin(), line.end());
    });


    for(auto line : grid) {
        for(char character : line) {
            std::cout << character;
        }
        std::cout << "\n";
    }

    // Setup up the game... I mean puzzle state
    Guard guard;
    std::vector<Vec2<int>> blocks;

    // Find the guard and collect block coordinates
    bool guard_found = false;
    for(size_t i = 0 ; i < grid.size() ; i++) {
        for(size_t j = 0 ; j < grid.size() ; j++) {
            if(grid[i][j] == '^' && !guard_found) {
                guard.position = Vec2((int) j, (int) i);
                guard_found = true;
            }
            else if(grid[i][j] == '#') {
                blocks.push_back(Vec2((int)j, (int)i));
            }
        }
    }
    if(!guard_found) {
        std::cout << "Guard was not found\n";
        return -1;
    }


    // Run the simulation
    int answer = 0;
    const int loop_limit = 10'000;
    std::vector<Vec2<int>> unique_blocks {
        guard.position
    };
    for(int i = 0 ; i < loop_limit ; i++) {
        std::cout << i << "\n";
        const auto next_pos = guard.get_next_position();
        const auto iter = std::find(blocks.begin(), blocks.end(), next_pos);
        //std::cout << "\n\nIteration Start Board State\n";
        //visualize_board(guard, blocks, grid[0].size(), grid.size());
        if(iter != blocks.end()) {
            guard.rotate_right();
        }
        else {
            guard.position = next_pos;
            const auto existing = std::find(unique_blocks.begin(), unique_blocks.end(), next_pos);
            if(existing == unique_blocks.end()) {
                if(next_pos.y >= 0 && next_pos.y < grid.size()) {
                    if(next_pos.x >= 0 && next_pos.x < grid[next_pos.y].size()) {
                        unique_blocks.push_back(next_pos);
                    }
                }
            }
        }
        if(guard.position.y < 0 || guard.position.y >= grid.size()) {
            break;
        }
        else if(guard.position.x < 0 || guard.position.x >= grid[guard.position.y].size()) {
            break;
        }
    }

    for(auto unique_block : unique_blocks) {
        std::cout << unique_block.x << '\t' << unique_block.y << '\n';
    }

    std::cout << "Answer 1: " << unique_blocks.size() << "\n";

    return 0;

}