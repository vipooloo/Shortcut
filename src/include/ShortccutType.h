#ifndef SHORTCCUTTYPE_H
#define SHORTCCUTTYPE_H

#include <cstdint>

enum class ShortccutType : uint32_t
{
  SHORTCUT,
  EMAIL_DEST,
  FTP_DEST,
  SMB_DEST,
};

#endif // SHORTCCUTTYPE_H