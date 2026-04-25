#ifndef PAGEQUERY_H
#define PAGEQUERY_H

#include "DbDefine.h"
#include <cstdint>
#include <string>

namespace shortcut {
struct PageQuery
{
    std::string sort_field{};
    DbOrderType order_type{DbOrderType::DESC};
    int32_t page_index{static_cast<int32_t>(DB_PAGE_DEFAULT_INDEX)};
    int32_t page_size{static_cast<int32_t>(DB_PAGE_DEFAULT_SIZE)};
};
}  // namespace shortcut

#endif  // PAGEQUERY_H