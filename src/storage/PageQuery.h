#ifndef PAGE_QUERY_H
#define PAGE_QUERY_H

#include "DbDefine.h"
#include <cstdint>
#include <string>

namespace shortcut {

// 独立分页查询入参，线程安全，无状态
struct PageQuery
{
    std::string sort_field{};
    DbOrderType order_type{DbOrderType::DESC};
    int32_t page_index{0};
    int32_t page_size{10};
};

}  // namespace shortcut

#endif