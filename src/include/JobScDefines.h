#ifndef JOBSCDEFINES_H
#define JOBSCDEFINES_H

#include <cstdint>

enum class JobScOrderType : int32_t
{
    ASC = 0,
    DESC = 1
};

typedef struct JobScPageResult
{
    uint32_t total_count;
    uint32_t total_page;
    uint32_t page_index;
    uint32_t page_size;
} JOBSCPAGERESULT_S;

enum class JobScType : uint32_t
{
    None = 0,
    ScanToEmail,
    ScanToFTP,
    ScanToSMB,
    ScanToUSB,
    All,
};

static constexpr uint32_t JOBSC_PAGE_DEFAULT_INDEX = 0U;
static constexpr uint32_t JOBSC_PAGE_DEFAULT_SIZE = 10U;

#endif  // JOBSCDEFINES_H