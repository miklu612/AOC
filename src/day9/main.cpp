#include"../util/file_io.hpp"

#include<cassert>
#include<iostream>
#include<map>

const unsigned int gc_empty_space = UINT_MAX;

class Block {

    public:

        Block deep_copy() {
            Block output = *this;
            if(output.next_block != nullptr) {
                output.next_block = std::make_shared<Block>(output.next_block->deep_copy());
            }
            return output;
        }

        std::vector<unsigned int> to_vector() {
            Block* next = this;
            std::vector<unsigned int> output;
            while(next != nullptr) {
                if(next->is_allocated) {
                    output.insert(output.end(), next->size, next->id);
                }
                else {
                    output.insert(output.end(), next->size, gc_empty_space);
                }
                if(next->next_block == nullptr) {
                    break;
                }
                next = next->next_block.get();
            }
            return output;
        }

        int get_depth() {
            std::shared_ptr<Block> next = next_block;
            int i = 0;
            for(; next != nullptr ; i++) {
                next = next->next_block;
            }
            return i;
        }

        unsigned int id;
        bool is_allocated;
        unsigned int size;
        std::shared_ptr<Block> next_block;
};

int main() {

    try {
        auto input = read_file("input.txt");
        assert(input.length() % 2 == 0);

        if(input.back() == '\n') {
            input.pop_back();
        }

        std::shared_ptr<Block> root_block = std::make_shared<Block>();
        std::shared_ptr<Block> next = root_block;

        int id = 0;
        for(int i = 0 ; i < input.length() ; i++) {
            if(input[i] < '0' || input[i] > '9') {
                std::cout << "Invalid data found: '" << input[i] << "'\n";
                std::cout << "Breaking and hoping it wasn't important\n";
                break;
            }
            if(i%2 == 0) {
                next->id = id;
                next->is_allocated = true;
                next->size = input[i]-'0';
                if(i+1 < input.length()) {
                    next->next_block = std::make_shared<Block>();
                }
                next = next->next_block;
                id += 1;
            }
            else {
                next->id = 0;
                next->is_allocated = false;
                next->size = input[i]-'0';
                if(i+1 < input.length()) {
                    next->next_block = std::make_shared<Block>();
                }
                next = next->next_block;
            }
        }


        auto vectorized_blocks = root_block->to_vector();
        int j = 0;
        for(int i = vectorized_blocks.size()-1 ; i >= 0 ; i--) {
            if(vectorized_blocks[i] == gc_empty_space) {
                continue;
            }
            else {
                for(; j < i ; j++) {
                    if(vectorized_blocks[j] == gc_empty_space) {
                        std::swap(vectorized_blocks[j], vectorized_blocks[i]);
                        break;
                    }
                }
            }
        }

        unsigned long long checksum_part_1 = 0;
        for(unsigned int i = 0 ; i < vectorized_blocks.size() ; i++) {
            if(vectorized_blocks[i] == gc_empty_space) {
                break;
            }
            checksum_part_1 += vectorized_blocks[i] * i;
        }


        std::cout << "Answer 1: " << checksum_part_1 << "\n";
        std::cout << "Answer 2: " << "Unimplemented" << "\n";


    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }


    return 0;

}