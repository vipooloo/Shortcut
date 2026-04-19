#include "IdAllocator.h"
#include <algorithm>

IdAllocator::IdAllocator()
  : m_id_range_rules{
    std::make_tuple(ShortccutType::SHORTCUT, 1, 1000),
    std::make_tuple(ShortccutType::EMAIL_DEST, 1001, 2000),
    std::make_tuple(ShortccutType::FTP_DEST, 2001, 3000),
    std::make_tuple(ShortccutType::SMB_DEST, 3001, 4000),
  }
{
}

uint32_t
IdAllocator::AllocateId(ShortccutType type, const std::vector<int>& used_ids)
{
  std::vector<IdRangeRule>::const_iterator it = std::find_if(
    m_id_range_rules.cbegin(),
    m_id_range_rules.cend(),
    [type](const IdRangeRule& rule) { return std::get<0>(rule) == type; });

  uint32_t allocated_id = 0;
  if (it != m_id_range_rules.cend()) {
    uint32_t min_id = std::get<1>(*it);
    uint32_t max_id = std::get<2>(*it);

    for (uint32_t id = min_id; id <= max_id; ++id) {
      if (std::find(used_ids.cbegin(), used_ids.cend(), id) ==
          used_ids.cend()) {
        allocated_id = id;
        break;
      }
    }
  }

  return allocated_id;
}