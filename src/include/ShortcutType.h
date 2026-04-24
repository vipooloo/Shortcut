#ifndef SHORTCUTTYPE_H
#define SHORTCUTTYPE_H

#include <cstdint>

enum class ShortcutType : uint32_t
{
    SHORTCUT,
    EMAIL_DEST,
    FTP_DEST,
    SMB_DEST,
};

#endif  // SHORTCUTTYPE_H