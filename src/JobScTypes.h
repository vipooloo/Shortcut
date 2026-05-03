#ifndef JOBSCTYPES_H
#define JOBSCTYPES_H

#include "JobScValue.h"
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

using JobScRow = std::map<std::string, JobScValue>;
using JobScRowList = std::vector<JobScRow>;
using UpgradeCallback = std::function<bool(int64_t, int64_t)>;

#endif  // JOBSCTYPES_H