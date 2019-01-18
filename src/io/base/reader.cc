#include "reader.h"

#include <cassert>
#include <snappy.h>

#include "util/crc32.h"

using strata::util::CRC32;

namespace strata {
namespace io {
namespace base {

void ReadOneResultStats::Clear() {
    num_ok = 0;
    num_error_checksum = 0;
    num_error_snappy = 0;
}

ReadLimit::ReadLimit(size_t max_items, size_t max_bytes) {
    max_items_ = max_items;
    max_bytes_ = max_bytes;
    items_ = 0;
    bytes_ = 0;
}

ReadLimit::ReadLimit(size_t max_items) {
    ReadLimit(max_items, ~0UL);
}

ReadLimit::ReadLimit() {
    ReadLimit(~0UL, ~0UL);
}

void ReadLimit::Start() {
    items_ = 0;
    bytes_ = 0;
}

bool ReadLimit::ShouldContinue(size_t bytes) {
    ++items_;
    ++bytes_ += bytes;
    if (max_items_ <= items_) {
        return false;
    }
    if (max_bytes_ <= bytes_) {
        return false;
    }
    return true;
}

StrataReader::~StrataReader() {
}

ReadOneResult StrataReader::ReadOne(string* item) {
    // Parse the flags and item size.
    uint16_t first;
    if (!ReadRaw(&first)) {
        return ReadOneResult::INCOMPLETE;
    }
    bool has_long_length = first & (1 << 15);
    bool used_snappy = first & (1 << 14);
    bool has_crc32 = first & (1 << 13);
    first &= 0x1FFF;
    uint32_t item_size;
    if (has_long_length) {
        uint16_t second;
        if (!ReadRaw(&second)) {
            return ReadOneResult::INCOMPLETE;
        }
        item_size = (static_cast<uint32_t>(first) << 16) | second;
    } else {
        item_size = first;
    }

    // Get the checksum, if present.
    uint32_t crc32;
    if (has_crc32) {
        if (!ReadRaw(&crc32)) {
            return ReadOneResult::INCOMPLETE;
        }
    } else {
        crc32 = 0;  // Linter not smart enough.
    }

    // Now get the item.
    if (!ReadRaw(item_size, item)) {
        return ReadOneResult::INCOMPLETE;
    }

    // If checksum, apply it.
    if (has_crc32) {
        uint32_t redo_crc32 = CRC32(item->data(), item->size());
        if (crc32 != redo_crc32) {
            return ReadOneResult::CHECKSUM;
        }
    }

    // If compressed, uncompress it.
    if (used_snappy) {
        string out;
        if (!snappy::Uncompress(item->data(), item->size(), &out)) {
            return ReadOneResult::SNAPPY;
        }
        item->swap(out);
    }

    return ReadOneResult::OK;
}

ReadManyResult StrataReader::Read(ReadLimit* limit, vector<string>* items,
                                  ReadOneResultStats* stats) {
    if (limit) {
        limit->Start();
    }
    if (stats) {
        stats->Clear();
    }
    string item;
    while (true) {
        ReadOneResult rr = ReadOne(&item);
        switch (rr) {
            case ReadOneResult::OK:
                items->emplace_back(item);
                if (stats) {
                    ++stats->num_ok;
                }
                break;
            case ReadOneResult::END:
                return ReadManyResult::END;
            case ReadOneResult::INCOMPLETE:
                return ReadManyResult::INCOMPLETE;
            case ReadOneResult::CHECKSUM:
                if (stats) {
                    ++stats->num_error_checksum;
                }
                break;
            case ReadOneResult::SNAPPY:
                if (stats) {
                    ++stats->num_error_snappy;
                }
                break;
            default:
                assert(false);
        }
        if (limit) {
            if (!limit->ShouldContinue(item.size())) {
                break;
            }
        }
    }
    return ReadManyResult::OK;
}

bool StrataReader::ReadRaw(size_t size, string* bytes) {
    bytes->resize(size);
    return ReadRaw(size, &(*bytes)[0]);
}

bool StrataReader::ReadRaw(uint16_t* bytes) {
    return ReadRaw(sizeof(*bytes), reinterpret_cast<char*>(bytes));
}

bool StrataReader::ReadRaw(uint32_t* bytes) {
    return ReadRaw(sizeof(*bytes), reinterpret_cast<char*>(bytes));
}

}  // namespace base
}  // namespace io
}  // namespace strata
