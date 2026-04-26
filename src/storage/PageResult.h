#ifndef PAGE_RESULT_H
#define PAGE_RESULT_H

#include "DbValue.h"
#include <cstdint>

namespace shortcut {
struct PageResult
{
    int32_t total_count;
    int32_t total_page;
    int32_t page_index;
    int32_t page_size;
};
}  // namespace shortcut

#endif