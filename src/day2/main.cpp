#include<iostream>
#include<fstream>
#include<string>
#include<optional>
#include<array>


int main() {

    std::ifstream file("input.txt");
    if(!file.good()) {
        std::cout << "Failed to read input\n";
        return -1;
    }

    std::string line;
    int valid_lines = 0;
    while(std::getline(file, line)) {
        // Not the best solution to fix the last digit not being check, but it works.
        line += " ";
        for(bool increasing : std::array<bool, 2> {false, true}) {
            int previous = 0;
            std::string current = "";
            int faults = 0;
            for(char character : line) {
                if(character == ' ') {
                    if(previous == 0) {
                        previous = std::stoi(current);
                    }
                    else {
                        int number = std::stoi(current);
                        if(std::abs(previous - number) > 3 || previous == number) {
                            std::cout << "Failed: " << number << " " << previous << "\n";
                            std::cout << "Reason: Difference too large\n";
                            faults += 1;
                        }
                        else if(increasing && previous > number) {
                            std::cout << "Failed: " << number << " " << previous << "\n";
                            std::cout << "Reason: Expected increased value\n";
                            faults += 1;
                        }
                        else if(!increasing && previous < number) {
                            std::cout << "Failed: " << number << " " << previous << "\n";
                            std::cout << "Reason: Expected decreased value\n";
                            faults += 1;
                        }
                        previous = number;
                    }
                    current.clear();
                }
                else {
                    current.push_back(character);
                }
            }
            if(faults == 0) {
                valid_lines += 1;
                break;
            }
        }
    }

    std::cout << "Answer: " << valid_lines << "\n";

    file.close();

    return 0;

}