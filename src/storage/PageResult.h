#ifndef PAGE_RESULT_H
#define PAGE_RESULT_H

#include <cstdint>

namespace shortcut {
struct PageResult
{
    uint32_t total_count;
    uint32_t total_page;
    uint32_t page_index;
    uint32_t page_size;
};
}  // namespace shortcut

#endif