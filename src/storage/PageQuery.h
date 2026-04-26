#ifndef PAGE_QUERY_H
#define PAGE_QUERY_H

#include "DbDefine.h"
#include <cstdint>
#include <string>

namespace shortcut {
struct PageQuery
{
    std::string sort_field;
    DbOrderType order_type;
    int32_t page_index;
    int32_t page_size;
};
}  // namespace shortcut

#endif