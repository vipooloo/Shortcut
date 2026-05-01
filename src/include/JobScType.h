#ifndef JOBSCTYPE_H
#define JOBSCTYPE_H

#include <cstdint>

enum class JobScType : uint32_t
{
    ScanToEmail,
    ScanToFTP,
    ScanToSMB,
    ScanToUSB,
    All,
};

#endif  // JOBSCTYPE_H