#include<iostream>
#include<fstream>
#include<string>
#include<optional>


int main() {

    std::ifstream file("input.txt");
    if(!file.good()) {
        std::cout << "Failed to read input\n";
        return -1;
    }

    std::string line;
    int valid_lines = 0;
    while(std::getline(file, line)) {
        int previous = 0;
        std::string current = "";
        bool is_valid = true;
        std::optional<bool> increasing;
        // Not the best solution to fix the last digit not being check, but it works.
        line += " ";
        for(char character : line) {
            if(character == ' ') {
                if(previous == 0) {
                    previous = std::stoi(current);
                }
                else {
                    int number = std::stoi(current);
                    if(!increasing.has_value()) {
                        increasing = previous < number;
                    }
                    if(std::abs(previous - number) > 3 || previous == number) {
                        std::cout << "Failed: " << number << " " << previous << "\n";
                        std::cout << "Reason: Difference too large\n";
                        is_valid = false;
                        break;
                    }
                    else if(increasing.value() && previous > number) {
                        std::cout << "Failed: " << number << " " << previous << "\n";
                        std::cout << "Reason: Expected increased value\n";
                        is_valid = false;
                        break;
                    }
                    else if(!increasing.value() && previous < number) {
                        std::cout << "Failed: " << number << " " << previous << "\n";
                        std::cout << "Reason: Expected decreased value\n";
                        is_valid = false;
                        break;
                    }
                    previous = number;
                }
                current.clear();
            }
            else {
                current.push_back(character);
            }
        }
        if(is_valid) {
            valid_lines += 1;
        }
    }

    std::cout << "Answer: " << valid_lines << "\n";

    file.close();

    return 0;

}