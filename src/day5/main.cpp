#include<iostream>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"


int main() {

    try {
    std::string input = read_file("input.txt");
    auto lines = StringUtil::split_by_character(input, '\n');

    std::cout << lines.back() << "\n";
    if(lines.back() == "") {
        std::cout << "Removed last\n";
        lines.pop_back();
    }

    for(auto line : lines) {
        std::cout << line << "\n";
    }

    auto update_start = std::find(lines.begin(), lines.end(), "");
    if(update_start == lines.end()) {
        std::cout << "Couldn't find the seperator between the data and update\n";
        return -1;
    }


    std::vector<std::pair<int, int>> ordering_rules(std::distance(lines.begin(), update_start));
    std::transform(lines.begin(), update_start, ordering_rules.begin(), [&lines](auto line){
        auto seperator = std::find(line.begin(), line.end(), '|');
        if(seperator == line.end()) {
            std::cout << "Couldn't find seperator in ordering rule\n";
            std::cout << "\"" << line << "\"" << "\n";
            std::exit(-1);
        }
        return std::pair<int, int>{
            std::stoi(std::string(line.begin(), seperator)),
            std::stoi(std::string(std::next(seperator), line.end()))
        };
    });

    std::vector<std::vector<int>> updates(std::distance(std::next(update_start), lines.end()));
    std::transform(std::next(update_start), lines.end(), updates.begin(), [](auto line){
        auto strings = StringUtil::split_by_character(line, ',');
        std::vector<int> numbers(strings.size());
        std::transform(strings.begin(), strings.end(), numbers.begin(), [](auto s_line){
            return std::stoi(s_line);
        });
        return numbers;
    });


    int answer = 0;
    for(auto update : updates) {
        for(auto rule : ordering_rules) {

            auto xpos = std::find(update.begin(), update.end(), rule.first);
            auto ypos = std::find(update.begin(), update.end(), rule.second);

            if(ypos == update.end()) {
                continue;
            }
            else if(xpos == update.end()) {
                continue;
            }
            else if(xpos > ypos) {
                goto rule_break;
            }

        }
        answer += update.at(update.size()/2);
rule_break:
        continue;
    }
    std::cout << answer << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}
