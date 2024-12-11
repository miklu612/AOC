#include<iostream>
#include<map>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"

using IntType = unsigned long long;
using CountType = unsigned long long;




int main() {

    try {
        auto input = read_file("input.txt");
        auto digit_strings = StringUtil::split_by_character(input, ' ');

        std::vector<IntType> digits(digit_strings.size());
        std::transform(digit_strings.begin(), digit_strings.end(), digits.begin(), [](const auto& str){
            return std::stoull(str);
        });

        CountType answer_1 = 0;
        CountType answer_2 = 0;
        for(auto digit : digits) {
            std::map<IntType, CountType> freq_map = {
                {digit, (CountType) 1}
            };
            for(int i = 0 ; i < 75 ; i++) {
                std::map<IntType, CountType> next_freq_map;
                for(auto& value_pair : freq_map) {
                    const auto value = value_pair.first;
                    if(value == 0) {
                        next_freq_map[1] += 1 * value_pair.second;
                        continue;
                    }
                    else if(value >= 10) {
                        // Weird math stuff that I figured out by messing with logarithms.
                        const auto power = (IntType) std::floor(std::log10(value));
                        if(power % 2 == 1) {
                            const IntType modulo_op = std::pow(10, std::ceil(power / 2))*10;
                            const IntType lower = value % modulo_op;
                            const IntType higher = (value - lower) / (modulo_op);
                            next_freq_map[lower] += 1 * value_pair.second;
                            next_freq_map[higher] += 1 * value_pair.second;
                            continue;
                        }
                    }
                    next_freq_map[value*2024] += 1 * value_pair.second;
                }
                freq_map = next_freq_map;
                if(i == 24) {
                    for(auto entry : freq_map) {
                        answer_1 += entry.second;
                    }
                }
            }
            for(auto entry : freq_map) {
                answer_2 += entry.second;
            }
        }

        std::cout << "Answer 1: " << answer_1 << "\n";
        std::cout << "Answer 2: " << answer_2 << "\n";


    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}