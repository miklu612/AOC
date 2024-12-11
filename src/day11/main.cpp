#include<iostream>
#include<map>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"

using IntType = unsigned long long;
const int cache_depth = 3;


IntType solve(IntType number, int depth=0, int limit=0) {
    if(depth >= limit) {
        return 1;
    }
    else if(number == 0) {
        if(depth+4 < limit) {
            return 
                solve(2, depth+4, limit) +
                solve(0, depth+4, limit) +
                solve(2, depth+4, limit) +
                solve(4, depth+4, limit);
        }
        else {
            return solve(1, depth+1, limit);
        }
    }
    else if(number >= 10){
        // Weird math stuff that I figured out by messing with logarithms.
        const auto power = (IntType) std::floor(std::log10(number));
        if(power % 2 == 1) {
            const IntType modulo_op = std::pow(10, std::ceil(power / 2))*10;
            const IntType lower = number % modulo_op;
            const IntType higher = (number - lower) / (modulo_op);
            IntType answer = 0;
            answer += solve(higher, depth+1, limit);
            answer += solve(lower, depth+1, limit);
            return answer;
        }
    }
    return solve(number*2024, depth+1, limit);
}

int main() {

    try {
        auto input = read_file("input.txt");
        auto digit_strings = StringUtil::split_by_character(input, ' ');

        std::vector<IntType> digits(digit_strings.size());
        std::transform(digit_strings.begin(), digit_strings.end(), digits.begin(), [](const auto& str){
            return std::stoull(str);
        });

        std::cout << digits.size() << "\n";

        unsigned long stones_1 = 0;
        for(auto digit : digits) {
            stones_1 += solve(digit, 0, 25);
        }

        unsigned long stones_2 = 0;
        //for(auto digit : digits) {
        //    std::cout << digit << "\n";
        //    stones_2 += solve(digit, 0, 75);
        //}

        std::cout << "Answer 1: " << stones_1 << "\n";
        std::cout << "Answer 2: " << stones_2 << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}