#include<fstream>
#include<iostream>
#include<string>
#include<array>

#include"../util/file_io.hpp"

bool has_mul_instruction(std::string::iterator iterator, std::string::iterator end) {
    bool has_mul = true;
    for(char character : std::array<char, 3> {'m', 'u', 'l'}) {
        if(iterator == end) {
            has_mul = false;
            break;
        }
        else if(*iterator != character) {
            has_mul = false;
        }
        else {
            std::advance(iterator, 1);
        }
    }
    return has_mul;
}

bool has_dont_instruction(std::string::iterator iterator, std::string::iterator end) {
    bool has_dont = true;
    for(char character : std::array<char, 7> {'d', 'o', 'n', '\'', 't', '(', ')'}) {
        if(iterator == end) {
            has_dont = false;
            break;
        }
        else if(*iterator != character) {
            has_dont = false;
        }
        else {
            std::advance(iterator, 1);
        }
    }
    return has_dont;
}

bool has_do_instruction(std::string::iterator iterator, std::string::iterator end) {
    bool has_do = true;
    for(char character : std::array<char, 4> {'d', 'o', '(', ')'}) {
        if(iterator == end) {
            has_do = false;
            break;
        }
        else if(*iterator != character) {
            has_do = false;
        }
        else {
            std::advance(iterator, 1);
        }
    }
    return has_do;
}

int main() {

    // Testing the use of iterators here.
    std::string input = read_file(std::string("input.txt"));
    auto iterator = input.begin();

    std::cout << input << "\n";

    int answer = 0;
    bool is_enabled = true;
    while(iterator != input.end()) {
        // I'm sorry, but assembly has broken my brain.
loop_start:
        // std::cout << *iterator << "\n";

        if(is_enabled && has_mul_instruction(iterator, input.end())) {
            std::advance(iterator, std::string("mul").length());
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

        else if(has_dont_instruction(iterator, input.end())) {
            std::cout << "don't() found\n";
            std::advance(iterator, std::string("don't()").length());
            is_enabled = false;
        }

        else if(has_do_instruction(iterator, input.end())) {
            std::cout << "do() found\n";
            std::advance(iterator, std::string("do()").length());
            is_enabled = true;
        }

        else {
            std::advance(iterator, 1);
        }

    }

end_program:
    std::cout << "Answer: " << answer << "\n";
    return 0;

}