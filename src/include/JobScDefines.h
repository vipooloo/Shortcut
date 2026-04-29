#ifndef JOBSCDEFINES_H
#define JOBSCDEFINES_H

#include <cstdint>

enum class JobScOrderType : int32_t
{
    ASC = 0,
    DESC = 1
};

struct PageResult
{
    uint32_t total_count;
    uint32_t total_page;
    uint32_t page_index;
    uint32_t page_size;
};

static constexpr uint32_t JOBSC_PAGE_DEFAULT_INDEX = 0U;
static constexpr uint32_t JOBSC_PAGE_DEFAULT_SIZE = 10U;

#endif  // JOBSCDEFINES_H