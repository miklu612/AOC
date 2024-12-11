#include<iostream>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"


using IntType = unsigned long long;


int main() {

    try {
        auto input = read_file("input.txt");
        auto digit_strings = StringUtil::split_by_character(input, ' ');

        std::vector<IntType> digits(digit_strings.size());
        std::transform(digit_strings.begin(), digit_strings.end(), digits.begin(), [](const auto& str){
            return std::stoull(str);
        });

        std::cout << digits.size() << "\n";

        unsigned long stones = 0;
        std::vector<IntType> stack;
        for(auto digit : digits) {
            stack.clear();
            stack.push_back(digit);

            for(int i = 0 ; i < 25 ; i++) {
                const auto cur_size = stack.size();
                for(int j = 0 ; j < cur_size ; j++) {
                    const auto& stone = stack[j];
                    if(stone == 0) {
                        stack.push_back(1);
                        continue;
                    }
                    else if(stone >= 10){
                        // Weird math stuff that I figured out by messing with logarithms.
                        const auto power = (IntType) std::floor(std::log10(stone));
                        if(power % 2 == 1) {
                            const IntType modulo_op = std::pow(10, std::ceil(power / 2))*10;
                            const IntType lower = stone % modulo_op;
                            const IntType higher = (stone - lower) / (modulo_op);
                            stack.push_back(higher);
                            stack.push_back(lower);
                            continue;
                        }
                    }
                    stack.push_back(stone * 2024);
                }
                stack.erase(stack.begin(), std::next(stack.begin(), cur_size));
                //for(auto stone : stack) {
                //    std::cout << stone << " ";
                //}
                //std::cout << "\n";
            }
            stones += stack.size();
        }

        std::cout << "Answer 1: " << stones << "\n";
        std::cout << "Answer 2: " << "Not implemented" << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}