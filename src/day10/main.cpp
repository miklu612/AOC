#include<iostream>
#include<array>
#include<unordered_map>
#include"../util/file_io.hpp"
#include"../util/string_util.hpp"
#include"../util/vec2.hpp"

// I skipped a day, so I have to do a bad for the computer, but somewhat easier
// on the mind to process answer. This could be done with a whole lot less
// complexity, but that, as stated just now, requires time and energy.


class PathMatrix;

class PathNode {
    public:

        PathNode(char value) : value(value) {

        }

        int get_answer() {
            if(value == '0') {
                int answer = 0;
                std::vector<std::weak_ptr<PathNode>> visited;
                get_incrementing(answer, visited);
                return answer;
            }
            else {
                return 0;
            }
        }

        int get_answer_part_2() {
            if(value == '0') {
                int answer = 0;
                get_incrementing_part_2(answer);
                return answer;
            }
            else {
                return 0;
            }
        }

        void get_incrementing(int& answer, std::vector<std::weak_ptr<PathNode>>& visited) {
            for(auto ptr : {up, down, left, right}) {
                if(ptr.use_count() == 0) {
                    continue;
                }
                int ptr_value = ptr.lock()->value;
                if(ptr_value == value+1) {
                    if(ptr_value == '9') {
                        auto existing = std::find_if(visited.begin(), visited.end(), [&ptr](const auto wptr){
                            return ptr.lock() == wptr.lock();
                        });
                        if(existing == visited.end()) {
                            visited.push_back(ptr);
                            answer += 1;
                        }
                        continue;
                    }
                    ptr.lock()->get_incrementing(answer, visited);
                }
            }
        }

        void get_incrementing_part_2(int& answer) {
            for(auto ptr : {up, down, left, right}) {
                if(ptr.use_count() == 0) {
                    continue;
                }
                int ptr_value = ptr.lock()->value;
                if(ptr_value == value+1) {
                    if(ptr_value == '9') {
                        answer += 1;
                        continue;
                    }
                    ptr.lock()->get_incrementing_part_2(answer);
                }
            }
        }

        int value;

        std::weak_ptr<PathNode> up;
        std::weak_ptr<PathNode> down;
        std::weak_ptr<PathNode> left;
        std::weak_ptr<PathNode> right;
};

class PathMatrix {
    public:

        void add(Vec2<int> pos, std::shared_ptr<PathNode> node) {
            const auto existing = std::find_if(matrix.begin(), matrix.end(), 
                [&pos](const auto& value){
                    return value.first == pos;
                });
            if(existing == matrix.end()) {
                matrix.push_back({pos, node});
            }
            else {
                throw std::exception("Tried to push an existing element");
            }
        }

        std::shared_ptr<PathNode> get(Vec2<int> pos) {
            const auto& iter = std::find_if(matrix.begin(), matrix.end(), 
                [&pos](const auto& current){
                    return current.first == pos;
                });
            if(iter == matrix.end()) {
                throw std::exception("Failed to find element");
            }
            return iter->second;
        }

        void solve() {
            int answer_part_1 = 0;
            int answer_part_2 = 0;
            for(auto entry : matrix) {
                answer_part_1 += entry.second->get_answer();
                answer_part_2 += entry.second->get_answer_part_2();
            }
            std::cout << "Answer 1: " << answer_part_1 << "\n";
            std::cout << "Answer 2: " << answer_part_2 << "\n";
        }

        std::vector<std::pair<Vec2<int>, std::shared_ptr<PathNode>>> matrix;
};

int main() {

    try {
        auto input = read_file("input.txt");
        auto lines = StringUtil::split_by_character(input, '\n');

        std::shared_ptr<PathNode> a = nullptr;

        PathMatrix path_matrix;

        for(int i = 0 ; i < lines.size() ; i++) {
            for(int j = 0 ; j < lines[i].length() ; j++) {
                std::shared_ptr<PathNode> node(std::make_shared<PathNode>(PathNode(lines[i][j])));
                Vec2<int> pos(j, i);
                path_matrix.add(pos, node);
            }
        }

        // Time to make connections
        const auto bounds = Vec2<int>(lines[0].size(), lines.size());
        for(int i = 0 ; i < lines.size() ; i++) {
            for(int j = 0 ; j < lines[i].length() ; j++) {
                auto node = path_matrix.get(Vec2<int>(j, i));
                if((Vec2<int>(j, i+1)).within_box(bounds)) {
                    node->up = path_matrix.get(Vec2<int>(j, i+1));
                }
                if((Vec2<int>(j, i-1)).within_box(bounds)) {
                    node->down = path_matrix.get(Vec2<int>(j, i-1));
                }
                if((Vec2<int>(j+1, i)).within_box(bounds)) {
                    node->right = path_matrix.get(Vec2<int>(j+1, i));
                }
                if((Vec2<int>(j-1, i)).within_box(bounds)) {
                    node->left = path_matrix.get(Vec2<int>(j-1, i));
                }
            }
        }

        path_matrix.solve();

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }
    
    return 0;

}