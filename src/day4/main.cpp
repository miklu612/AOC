#include<iostream>
#include<optional>
#include<array>
#include<unordered_map>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"

const std::array match_set = {
    'X', 'M', 'A', 'S'
};

int check_character(const std::vector<std::string>& vec, int i, int j) {
    int output = 0;
    for(auto y : {-1, 0, 1}) {
        for(auto x : {-1, 0, 1}) {
            if(y == 0 && x == 0) {
                continue;
            }
            auto py = i;
            auto px = j;
            for(auto character : match_set) {
                if(py < 0 || px < 0) {
                    goto no_match;
                }
                else if(py >= vec.size()) {
                    goto no_match;
                }
                else if(px >= vec[py].length()) {
                    goto no_match;
                }
                else if(vec[py][px] != character) {
                    goto no_match;
                }
                py += y;
                px += x;
            }
            output += 1;
no_match:
            continue;
        }
    }
    return output;
}

int check_x_mas(const std::vector<std::string>& vec, int i, int j) {

    if(vec[i][j] != 'A') return 0;
    if(i-1 < 0) return 0;
    if(j-1 < 0) return 0;
    if(i+1 >= vec.size()) return 0;
    if(j+1 >= vec[i].length()) return 0;

    if(
        (vec[i+1][j+1] != 'M' || vec[i-1][j-1] != 'S') &&
        (vec[i+1][j+1] != 'S' || vec[i-1][j-1] != 'M') 
    ) return 0;

    if(
        (vec[i+1][j-1] != 'M' || vec[i-1][j+1] != 'S') &&
        (vec[i+1][j-1] != 'S' || vec[i-1][j+1] != 'M') 
    ) return 0;

    return 1;
}

int main() {

    std::string input = read_file("./input.txt");
    std::cout << input << "\n";

    const auto lines = StringUtil::split(input.begin(), input.end(), StringUtil::line_splitter);

    int answer_p1 = 0;
    int answer_p2 = 0;
    std::vector<std::tuple<int, int, int>> start_positions;
    for(auto i = 0 ; i < lines.size() ; i++) {
        for(auto j = 0 ; j < lines[i].length() ; j++) {
            answer_p1 += check_character(lines, i, j);
            answer_p2 += check_x_mas(lines, i, j);
        }
    }

    std::cout << "Answer 1: " << answer_p1 << "\n";
    std::cout << "Answer 2: " << answer_p2 << "\n";


    return 0;

}