#include<iostream>
#include<fstream>
#include<string>
#include<optional>
#include<array>
#include<vector>
#include<algorithm>


int main() {

    try {
    std::ifstream file("input.txt");
    if(!file.good()) {
        std::cout << "Failed to read input\n";
        return -1;
    }


    int answer_p1 = 0;

    std::string line;
    while(std::getline(file, line)) {
        std::vector<int> numbers;
        std::string current;
        line += " ";
        for(char character : line) {
            if(character == ' ') {
                numbers.push_back(std::stoi(current));
                current.clear();
            }
            else {
                current += character;
            }
        }


        bool exit = false;
        for(bool growing : std::array<bool, 2> {false, true}) {
            if(exit) {
                break;
            }
            std::vector<int> mistakes = {0};
            int i = -1;
            do {
                std::vector<int> filtered;
                if(i < mistakes.size() && i >= 0) {
                    std::copy(numbers.begin(), std::next(numbers.begin(), mistakes[i]), std::back_inserter(filtered));
                    std::copy(std::next(numbers.begin(), mistakes[i]+1), numbers.end(), std::back_inserter(filtered));
                }
                else {
                    filtered = numbers;
                }
                std::cout << "Filtered: ";
                for(int n : filtered) {
                    std::cout << n << " ";
                }
                std::cout << "\n";
                int faults = 0;
                for(size_t j = 1 ; j < filtered.size() ; j++) {
                    if(growing && filtered[j-1] > filtered[j]) {
                        if(std::find(mistakes.begin(), mistakes.end(), j) == mistakes.end()) {
                            mistakes.push_back(j);
                        }
                        faults += 1;
                    }
                    else if(!growing && filtered[j-1] < filtered[j]) {
                        if(std::find(mistakes.begin(), mistakes.end(), j) == mistakes.end()) {
                            mistakes.push_back(j);
                        }
                        faults += 1;
                    }
                    else if(filtered[j-1] == filtered[j] || std::abs(filtered[j-1] - filtered[j]) > 3) {
                        if(std::find(mistakes.begin(), mistakes.end(), j) == mistakes.end()) {
                            mistakes.push_back(j);
                        }
                        faults += 1;
                    }
                }
                i += 1;
                if(faults == 0) {
                    answer_p1 += 1;
                    exit = true;
                    std::cout << "Pass\n";
                    break;
                }
            } while(i < mistakes.size());
            std::cout << "Mistakes:\n";
            for(int mistake : mistakes) {
                std::cout << mistake << " ";
            }
            std::cout << "\n";
        }
    }

    std::cout << answer_p1 << "\n";


    file.close();
    }

    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;

}