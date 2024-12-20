#ifndef string_util_hpp
#define string_util_hpp

#include<string>
#include<vector>
#include<functional>

namespace StringUtil {

    // Lesson learned, this code is broken and rewriting it is faster. Just
    // gonna leave it here, since I used it previously.
    template<typename T>
    std::vector<std::string> split(std::string::iterator start, std::string::iterator end, T check) {
        std::vector<std::string> output;
        std::string::iterator copy_start = start;
        std::string::iterator iterator = start;
        while(iterator < end) {
            std::string::iterator new_pos = check(iterator, end);
            if(new_pos == end) {
                std::advance(iterator, 1);
            }
            else {
#ifndef MAX_SPEED_PLEASE
                if(new_pos < start || new_pos >= end) {
                    throw std::exception("StringUtil::split bound error");
                }
#endif
                output.push_back(std::string (copy_start, iterator));
                copy_start = new_pos;
                iterator = std::next(new_pos, 1);
            }
        }
        return output;
    }

    const auto line_splitter = [](std::string::iterator cur, std::string::iterator end){
        if(std::next(cur, 1) == end) {
            return cur;
        }
        else if(*cur != '\n') {
            return end;
        }
        return std::next(cur, 1);
    };

    std::vector<std::string> split_by_character(std::string text, char character) {
        auto iter = text.begin();
        std::vector<std::string> output;
        do {
            auto last = iter;
            iter = std::find(iter, text.end(), character);
            output.push_back(std::string(last, iter));
            std::advance(iter, 1);
        }
        while(iter < text.end());
        return output;
    }

};

#endif