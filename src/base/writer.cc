#include "writer.h"

#include <cassert>
#include <snappy.h>

#include "util/crc32.h"

using strata::util::CRC32;

namespace strata {
namespace base {

Writer::~Writer() {
}

bool Writer::WriteOne(WriteFlags flags, const string& uncompressed_item) {
    // Perform fast compression if desired.
    string compressed_item;
    const string* item;
    if (flags.snappy) {
        snappy::Compress(uncompressed_item.data(), uncompressed_item.size(),
                         &compressed_item);
        item = &compressed_item;
    } else {
        item = &uncompressed_item;
    }

    // Write the flags + item length field.
    //
    // This is a uint16_t if the item is short enough, else uint32_t.
    //
    // The first three bits are reserved for flags:
    // * BIG_LENGTH  Whether we are using 4 bytes for length (else 2).
    // * SNAPPY      Whether the item is compressed with Snappy.
    // * CRC32       Whether we are including a CRC32 checksum.
    if (item->size() < (1U << 13)) {
        uint16_t x = static_cast<uint16_t>(item->size());
        if (flags.snappy) {
            x |= (1U << 14);
        }
        if (flags.crc32) {
            x |= (1U << 13);
        }
        if (!WriteRaw(x)) {
            return false;
        }
    } else if (item->size() < (1U << 29)) {
        uint32_t x = static_cast<uint16_t>(item->size());
        x |= (1U << 31);
        if (flags.snappy) {
            x |= (1U << 30);
        }
        if (flags.crc32) {
            x |= (1U << 29);
        }
        if (!WriteRaw(x)) {
            return false;
        }
    } else {
        assert(false);
    }

    // Add a CRC32 checksum if desired.
    if (flags.crc32) {
        uint32_t crc32 = CRC32(item->data(), item->size());
        if (!WriteRaw(crc32)) {
            return false;
        }
    }

    // Finally, write the item itself.
    return WriteRaw(*item);
}

size_t Writer::Write(WriteFlags flags, const vector<string>& items) {
    size_t i;
    for (i = 0; i < items.size(); ++i) {
        if (!WriteOne(flags, items[i])) {
            break;
        }
    }
    return i;
}

bool Writer::WriteRaw(const string& bytes) {
    return WriteRaw(bytes.data(), bytes.size());
}

bool Writer::WriteRaw(uint16_t bytes) {
    return WriteRaw(reinterpret_cast<const char*>(&bytes), sizeof(bytes));
}

bool Writer::WriteRaw(uint32_t bytes) {
    return WriteRaw(reinterpret_cast<const char*>(&bytes), sizeof(bytes));
}

}  // namespace base
}  // namespace strata
