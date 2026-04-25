#ifndef PAGERESULT_H
#define PAGERESULT_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace shortcut {
struct PageResult
{
    std::vector<std::map<std::string, std::string>> rows{};
    int32_t total_count{0};
    int32_t total_page{0};
    int32_t page_index{0};
    int32_t page_size{0};
};
}  // namespace shortcut

#endif