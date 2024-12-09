#include"../util/file_io.hpp"

#include<cassert>
#include<iostream>
#include<map>
#include<array>

const unsigned int gc_empty_space = UINT_MAX;
const unsigned int gc_only_sub_entries = UINT_MAX-1;

using IDInt = uint64_t;

class SubEntry {
    public:
        uint8_t size;
        IDInt id;
};

class Entry {
    public:

        Entry(uint8_t size, IDInt id) : size(size), id(id) {
        }

        void addSubEntry(uint8_t se_size, IDInt se_id) {
            if(se_size > size) {
                std::cout << (int) se_size << "\t" << (int) size << "\n";
                throw std::exception("Ran out of space\n");
            }
            size -= se_size;
            sub_entries[sub_entry_index].id = se_id;
            sub_entries[sub_entry_index].size = se_size;
            sub_entry_index += 1;
            if(size == 0) {
                id = gc_only_sub_entries;
            }
        }

        uint8_t size;
        IDInt id;

        // When parsing, these entries are done first and then the actual entry.
        std::array<SubEntry, 9> sub_entries;
        uint8_t sub_entry_index = 0;

};

int main() {

    try {
        auto input = read_file("input.txt");
        assert(input.length() % 2 == 0);

        if(input.back() == '\n') {
            input.pop_back();
        }

        std::vector<Entry> entries;
        entries.reserve(input.size());

        for(int i = 0 ; i < input.length() ; i++) {
            if(i%2) {
                entries.emplace_back(input[i]-'0', gc_empty_space);
            }
            else {
                entries.emplace_back(input[i]-'0', i/2);
            }
        }

        auto part_2_entries = entries;

        // Part 1
        auto part_1_entry = entries.begin();
        for(int i = entries.size()-1 ; i >= 0 ; i--) {
            if(entries[i].id == gc_empty_space) {
                continue;
            }
            const int previous_size = entries[i].size;
            const auto end = std::next(entries.begin(), i);
            while(part_1_entry < end) {
                part_1_entry = std::find_if(part_1_entry, end, [](auto const& entry){
                    return 
                        entry.id == gc_empty_space &&
                        entry.size > 0;
                });
                if(part_1_entry >= end) {
                    break;
                }
                if(entries[i].size <= part_1_entry->size) {
                    part_1_entry->addSubEntry(entries[i].size, entries[i].id);
                    entries[i].id = gc_empty_space;
                    break;
                }
                else {
                    entries[i].size -= part_1_entry->size;
                    part_1_entry->addSubEntry(part_1_entry->size, entries[i].id);
                }
            }
        }
        unsigned long long checksum_part_1 = 0;
        IDInt id = 0;
        for(auto entry : entries) {
            for(int i = 0 ; i < entry.sub_entry_index ; i++) {
                for(int j = 0 ; j < entry.sub_entries[i].size ; j++) {
                    checksum_part_1 += entry.sub_entries[i].id * id;
                    id += 1;
                }
            }
            if(entry.id != gc_empty_space) {
                for(int i = 0 ; i < entry.size ; i++) {
                    checksum_part_1 += entry.id * id;
                    id += 1;
                }
            }
        }


        // Part 2
        for(int i = part_2_entries.size()-1 ; i >= 0 ; i--) {
            if(part_2_entries[i].id == gc_empty_space || part_2_entries[i].id == gc_only_sub_entries) {
                continue;
            }
            const int previous_size = part_2_entries[i].size;
            auto entry = part_2_entries.begin();
            const auto end = std::next(part_2_entries.begin(), i);
            while(entry < end) {
                entry = std::find_if(std::next(entry), end, [&part_2_entries, &i](auto const& entry){
                    return 
                        entry.id == gc_empty_space &&
                        entry.size >= part_2_entries[i].size;
                });
                if(entry >= end) {
                    break;
                }
                entry->addSubEntry(part_2_entries[i].size, part_2_entries[i].id);
                part_2_entries[i].id = gc_empty_space;
                break;
            }
        }

        unsigned long long checksum_part_2 = 0;
        id = 0;
        for(auto entry : part_2_entries) {
            for(int i = 0 ; i < entry.sub_entry_index ; i++) {
                for(int j = 0 ; j < entry.sub_entries[i].size ; j++) {
                    checksum_part_2 += entry.sub_entries[i].id * id;
                    id += 1;
                }
            }
            if(entry.id != gc_empty_space && entry.id != gc_only_sub_entries) {
                for(int i = 0 ; i < entry.size ; i++) {
                    checksum_part_2 += entry.id * id;
                    id += 1;
                }
            }
            else {
                for(int i = 0 ; i < entry.size ; i++) {
                    id += 1;
                }
            }
        }

        std::cout << "Part 1: " << checksum_part_1 << "\n";
        std::cout << "Part 2: " << checksum_part_2 << "\n";

    }
    catch(std::exception e) {
        std::cout << e.what() << "\n";
    }


    return 0;

}