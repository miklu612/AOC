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

        void print() {
            Block* block = this;
            while(block != nullptr) {
                for(int i = 0 ; i < block->size ; i++) {
                    if(block->is_allocated) {
                        std::cout << block->id;
                    }
                    else {
                        std::cout << ".";
                    }
                }
                block = block->next_block.get();
            }
            std::cout << "\n";
        }

        void defragment() {
            bool changed = true;
            std::cout << "Defragmenting\n";
            while(changed) {
                changed = false;
                auto current = get_last();
                while(current != nullptr) {
                    if(current->previous_block.use_count() == 0) {
                        break;
                    }
                    if(current->is_allocated) {
                        auto first = next_block;
                        while(true) {
                            if(first == current) {
                                break;
                            }
                            else if(first == nullptr) {
                                std::cout << first << "\t" << current << "\n";
                                throw std::exception("Couldn't find slot");
                            }
                            else if(first->is_allocated){
                                first = first->next_block;
                                continue;
                            }
                            else if(first->size == current->size) {
                                std::swap(first->id, current->id);
                                std::swap(first->size, current->size);
                                std::swap(first->is_allocated, current->is_allocated);
                                changed = true;
                                break;
                            }
                            else if(current->size < first->size) {

                                while(first->next_block->is_allocated == false) {
                                    if(first->next_block == nullptr) {
                                        break;
                                    }
                                    first->size += first->next_block->size;
                                    first->next_block->next_block->previous_block = first;
                                    first->next_block = first->next_block->next_block;
                                }

                                // Make a new entry
                                std::shared_ptr<Block> new_block = std::make_shared<Block>();
                                new_block->id = current->id;
                                new_block->size = current->size;
                                new_block->is_allocated = current->is_allocated;
                                new_block->previous_block = first->previous_block;
                                new_block->next_block = first;

                                first->previous_block.lock()->next_block = new_block;
                                first->previous_block = new_block;
                                first->size -= current->size;

                                current->is_allocated = false;
                                changed = true;
                                break;

                            }
                            else {
                                first = first->next_block;
                                continue;
                            }
                        }
                    }
                    current = current->previous_block.lock();
                }

                // Do an actual defragmentation of the entries. e.g. remove
                // multiple empty entries
                // auto new_ptr = next_block;
                // while(new_ptr != nullptr && new_ptr->next_block != nullptr) {
                //     if(new_ptr->is_allocated == true) {
                //         new_ptr = new_ptr->next_block;
                //         continue;
                //     }
                //     else if(new_ptr->next_block->is_allocated == true) {
                //         new_ptr = new_ptr->next_block;
                //         continue;
                //     }
                //     // Defragment
                //     new_ptr->size += new_ptr->next_block->size;
                //     new_ptr->next_block = new_ptr->next_block->next_block;
                //     changed = true;
                // }

            }
            std::cout << "Defragment done\n";
        }


        std::shared_ptr<Block> get_last() {
            if(next_block == nullptr) {
                throw std::exception("Expected at least one child\n");
            }
            std::shared_ptr<Block> next = next_block;
            while(next->next_block != nullptr) {
                next = next->next_block;
            }
            return next;
        }

        int get_depth() {
            std::shared_ptr<Block> next = next_block;
            int i = 0;
            for(; ; i++) {
                if(next->next_block == nullptr) {
                    break;
                }
                next = next->next_block;
            }
            return i;
        }

        unsigned int id;
        bool is_allocated;
        unsigned int size;
        std::shared_ptr<Block> next_block;
        std::weak_ptr<Block> previous_block;
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
        std::shared_ptr<Block> previous = nullptr;

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
                next->previous_block = previous;
                if(i+1 < input.length()) {
                    next->next_block = std::make_shared<Block>();
                }
                previous = next;
                next = next->next_block;
                id += 1;
            }
            else {
                next->id = 0;
                next->is_allocated = false;
                next->size = input[i]-'0';
                next->previous_block = previous;
                if(i+1 < input.length()) {
                    next->next_block = std::make_shared<Block>();
                }
                previous = next;
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


        root_block->defragment();
        const auto defragmented_vectorized_blocks = root_block->to_vector();
        unsigned long long checksum_part_2 = 0;
        for(unsigned int i = 0 ; i < defragmented_vectorized_blocks.size() ; i++) {
            if(defragmented_vectorized_blocks[i] == gc_empty_space) {
                continue;
            }
            checksum_part_2 += defragmented_vectorized_blocks[i] * i;
        }


        std::cout << "Answer 1: " << checksum_part_1 << "\n";
        std::cout << "Answer 2: " << checksum_part_2 << "\n";


    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }


    return 0;

}