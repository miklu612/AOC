#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>

int main() {

    std::vector<int64_t> left_column;
    std::vector<int64_t> right_column;

    std::ifstream file("input.txt");
    if(!file.good()) {
        std::cout << "Couldn't open file 'input.txt'\n";
        return -1;
    }

    std::string line;
    while(std::getline(file, line)) {

        std::string left;
        std::string right;
        size_t i = 0;

        for(; i < line.length() ; i++) {
            if(line[i] == ' ') {
                break;
            }
            else {
                left.push_back(line[i]);
            }
        }

        i += 2;

        for(; i < line.length() ; i++) {
            right.push_back(line[i]);
        }

        left_column.push_back(std::stoi(left));
        right_column.push_back(std::stoi(right));

    }

    file.close();

    if(left_column.size() != right_column.size()) {
        std::cout << "Size mismatch\n";
        return -1;
    }


#ifndef PART_ONE
    std::cout << "Delta\tLeft\tRight\n";
    int64_t sum = 0;
    while(true) {

        struct {
            int64_t value = INT64_MAX;
            int64_t index = 0;
        } min_left;

        struct {
            int64_t value = INT64_MAX;
            int64_t index = 0;
        } min_right;

        for(size_t i = 0 ; i < left_column.size() ; i++) {
            if(min_left.value > left_column[i]) {
                min_left.value = left_column[i];
                min_left.index = i;
            }
            if(min_right.value > right_column[i]) {
                min_right.value = right_column[i];
                min_right.index = i;
            }
        }

        if(min_left.value == INT64_MAX) {
            break;
        }
        else if(min_right.value == INT64_MAX) {
            break;
        }

        left_column[min_left.index] = INT64_MAX;
        right_column[min_right.index] = INT64_MAX;


        int64_t delta = std::abs(min_right.value - min_left.value);
        sum += delta;

        std::cout << delta << "\t";
        std::cout << min_left.value << "\t";
        std::cout << min_right.value << "\n";

    }

    std::cout << "Answer: " << sum << "\n";


#else
#ifdef PART_TWO

    int64_t similiarity = 0;
    for(int64_t l_number : left_column) {
        int64_t count = std::count_if(right_column.cbegin(), right_column.cend(), [l_number](auto num){
            return num == l_number;
        });
        similiarity += count * l_number;
    }

    std::cout << "Answer: " << similiarity << "\n";

#else
#error "You have to define either PART_ONE or PART_TWO"
#endif
#endif


    return 0;

}