#ifndef IDALLOCATOR_H
#define IDALLOCATOR_H

#include "ShortccutType.h"
#include <cstdint>
#include <tuple>
#include <vector>

using IdRangeRule = std::tuple<ShortccutType, ///< 快捷方式类型
                               uint32_t,      ///< 最小分配ID
                               uint32_t       ///< 最大分配ID
                               >;
class IdAllocator
{
public:
  IdAllocator();
  ~IdAllocator() = default;
  uint32_t AllocateId(ShortccutType type, const std::vector<int>& used_ids);

private:
  std::vector<IdRangeRule> m_id_range_rules;
};

#endif // IDALLOCATOR_H