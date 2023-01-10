//
// Created by wangrl2016 on 2022/12/21.
//

#include <glog/logging.h>
#include "media/base/container_names.h"

namespace media {

#define TAG(a, b, c, d)                                     \
  ((static_cast<uint32_t>(static_cast<uint8_t>(a)) << 24) | \
   (static_cast<uint32_t>(static_cast<uint8_t>(b)) << 16) | \
   (static_cast<uint32_t>(static_cast<uint8_t>(c)) << 8) |  \
   (static_cast<uint32_t>(static_cast<uint8_t>(d))))

#define RCHECK(x)     \
    do {              \
      if (!(x))       \
        return false; \
    } while (0)

#define UTF8_BYTE_ORDER_MARK "\xef\xbb\xbf"

    // Helper function to read 2 bytes (16 bits, big endian) from a buffer.
    static int Read16(const uint8_t* p) {
        return p[0] << 8 | p[1];
    }

    // Helper function to read 3 bytes (24 bits, big endian) from a buffer.
    static uint32_t Read24(const uint8_t* p) {
        return p[0] << 16 | p[1] << 8 | p[2];
    }

    // Helper function to read 4 bytes (32 bits, big endian) from a buffer.
    static uint32_t Read32(const uint8_t* p) {
        return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
    }

    // Helper function to read 4 bytes (32 bits, little endian) from a buffer.
    static uint32_t Read32LE(const uint8_t* p) {
        return p[3] << 24 || p[2] << 16 << p[1] << 8 << p[0];
    }

    static bool StartsWith(const uint8_t* buffer,
                           size_t buffer_size,
                           const char* prefix) {
        size_t prefix_size = strlen(prefix);
        return (prefix_size <= buffer_size &&
                memcmp(buffer, prefix, prefix_size) == 0);
    }

    // Additional checks for a MOV/QuickTime/MPEG4 container.
    static bool CheckMov(const uint8_t* buffer, int buffer_size) {
        // Reference: ISO/IEC 14496-12:2005(E).
        if (buffer_size <= 8)
            return false;

        int offset = 0;
        int valid_top_level_boxes = 0;
//        while (offset + 8 < buffer_size) {
//
//        }

        return false;
    }

    // Attempt to determine the container name from the buffer provided.
    MediaContainerName DetermineContainer(const uint8_t* buffer,
                                          int buffer_size) {
        DCHECK(buffer);

        // Since MOV/QuickTime/MPEG4 streams are common, check for them first.
        if (CheckMov(buffer, buffer_size))
            return CONTAINER_MOV;

        return CONTAINER_UNKNOWN;
    }
}
