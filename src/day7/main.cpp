#include<iostream>
#include<cassert>
#include<array>
#include"../util/string_util.hpp"
#include"../util/file_io.hpp"


// Challenge: No recursion
bool solve(const std::vector<unsigned long long>& input, unsigned long long goal) {

    std::vector<char> operands(input.size()-1, '+');
    assert(input.size()-1 == operands.size());

    int iteration_max = std::pow(3, operands.size()+1);


    for(int iteration = 0 ; iteration < iteration_max ; iteration++) {
        unsigned long long previous = input[0];
        for(int i = 0 ; i < input.size()-1 ; i++) {
            const auto& operand = operands[i];
            unsigned long long current = (unsigned long long) input[i+1];
            if(operand == '+') {
                previous += current;
            }
            else if(operand == '*') {
                previous *= current;
            }
            else {
                const std::string number_1 = std::to_string(previous);
                const std::string number_2 = std::to_string(current);
                const std::string number_3 = number_1 + number_2;
                previous = std::stoull(number_3);
            }
        }
        if(previous == goal) {
            return true;
        }
        else {
            // Do a binary-esque increment of the operands vector
            //    +  ==  0
            //    *  ==  1
            //    |  ==  3
            for(int i = 0 ; i < operands.size() ; i++) {
                if(operands[i] == '+') {
                    operands[i] = '*';
                    break;
                }
                else if(operands[i] == '*'){
                    operands[i] = '|';
                    break;
                }
                else {
                    operands[i] = '+';
                }
            }
        }
    }
    
    return false;
}

int main() {

    try {
        auto input = read_file("input.txt");
        auto lines = StringUtil::split_by_character(input, '\n');
        unsigned long long answer = 0;
    
        for(const auto& line : lines) {

            auto data_seperator = std::find(line.begin(), line.end(), ':');
            if(data_seperator == line.end()) {
                std::cout << "Failed to find data seperator in input\n";
                std::cout << line << "\n";
                exit(-1);
            }

            // Collect the answer
            unsigned long long expected_answer = 0;
            for(auto iter = line.begin() ; iter < data_seperator ; std::advance(iter, 1 )) {
                expected_answer = expected_answer * 10 + (*iter - '0');
            }

            // Collect the rest of the numbers
            std::vector<unsigned long long> input_numbers;

            // Create another iterator and skip the whitespace in the input
            auto iter = std::next(data_seperator, 2);


            while(iter < line.end()) {
                auto end = std::find(iter, line.end(), ' ');
                int num = 0;
                for(auto c_iter = iter ; c_iter < end ; std::advance(c_iter, 1)) {
                    num = num * 10 + (*c_iter - '0');
                }
                input_numbers.push_back(num);
                iter = std::next(end);
            }

            if(solve(input_numbers, expected_answer)) {
                std::cout << "Correct: " << expected_answer << "\n";
                answer += expected_answer;
            }

        }

        std::cout << "Answer 1: " << answer << "\n";
    }

    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}