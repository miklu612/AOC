#include<fstream>
#include<iostream>
#include<string>
#include<array>

#include"../util/file_io.hpp"


int main() {

    // Testing the use of iterators here.
    std::string input = read_file(std::string("input.txt"));
    auto iterator = input.begin();

    std::cout << input << "\n";

    int answer = 0;
    while(iterator != input.end()) {
        // I'm sorry, but assembly has broken my brain.
loop_start:
        std::cout << *iterator << "\n";
        // Test for mul
        //   Note: const char*'s add a null terminator into an iterator.
        for(char character : std::array<char, 3> {'m', 'u', 'l'}) {
            if(iterator == input.end()) {
                goto end_program;
            }
            else if(*iterator != character) {
                std::advance(iterator, 1);
                goto loop_start;
            }
            else {
                std::advance(iterator, 1);
            }
        }

        std::cout << "First round of tests passed\n";

        if(iterator == input.end()) continue;
        if(*iterator != '(') continue;
        std::advance(iterator, 1);

        std::cout << "Second round of tests passed\n";

        std::string first_digit_str;
        while(iterator != input.end()) {
            if(*iterator == ',') {
                std::advance(iterator, 1);
                break;
            }
            else if(!std::isdigit(*iterator)) {
                goto loop_start;
            }
            else {
                first_digit_str += *iterator;
                std::advance(iterator, 1);
            }
        }

        std::cout << "Second round of tests passed\n";

        std::string second_digit_str;
        while(iterator != input.end()) {
            if(*iterator == ')') {
                break;
            }
            else if(!std::isdigit(*iterator)) {
                goto loop_start;
            }
            else {
                second_digit_str += *iterator;
                std::advance(iterator, 1);
            }
        }

        int first_digit = std::stoi(first_digit_str);
        int second_digit = std::stoi(second_digit_str);
        answer += first_digit * second_digit;

    }

end_program:
    std::cout << "Answer: " << answer << "\n";
    return 0;

}