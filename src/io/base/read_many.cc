#include "reader.h"

#include <cassert>
#include <snappy.h>

#include "util/crc32.h"

using strata::util::CRC32;

namespace strata {
namespace io {
namespace base {

void StrataReadManyInfo::Init() {
     err_crc32_items = 0;
     err_crc32_record_bytes = 0;
     err_crc32_encoded_payload_bytes = 0;

     err_snappy_items = 0;
     err_snappy_record_bytes = 0;
     err_snappy_encoded_payload_bytes = 0;

     ok_items = 0;
     ok_record_bytes = 0;
     ok_encoded_payload_bytes = 0;
     ok_decoded_payload_bytes = 0;

     ok_big_head = 0;
     ok_had_crc32 = 0;
     ok_used_snappy = 0;

     items = 0;
     record_bytes = 0;
     encoded_payload_bytes = 0;
     decoded_payload_bytes = 0;
}

void StrataReadManyInfo::Add(const StrataReadOneInfo& info) {
    switch (info.exit()) {
        case StrataReadOneExit::END:
            return;
        case StrataReadOneExit::TRUNCATED_HEAD:
            return;
        case StrataReadOneExit::TRUNCATED_CRC32:
            return;
        case StrataReadOneExit::TRUNCATED_PAYLOAD:
            return;
        case StrataReadOneExit::ERR_CRC32:
            ++err_crc32_items;
            err_crc32_record_bytes += info.record_size();
            err_crc32_encoded_payload_bytes += info.encoded_payload_size();
            break;
        case StrataReadOneExit::ERR_SNAPPY:
            ++err_snappy_items;
            err_snappy_record_bytes += info.record_size();
            err_snappy_encoded_payload_bytes += info.encoded_payload_size();
            break;
        case StrataReadOneExit::OK:
            ++ok_items;
            ok_record_bytes += info.record_size();
            ok_encoded_payload_bytes += info.encoded_payload_size();
            ok_decoded_payload_bytes += info.decoded_payload_size();
            if (info.head_size() == 4) {
                ++ok_big_head;
            }
            if (info.had_crc32()) {
                ++ok_had_crc32;
            }
            if (info.used_snappy()) {
                ++ok_used_snappy;
            }
            break;
    }
    ++items;
    record_bytes += info.record_size();
    encoded_payload_bytes += info.encoded_payload_size();
    if (info.ok()) {
        decoded_payload_bytes += info.decoded_payload_size();
    }
}

StrataReadLimit::StrataReadLimit(size_t max_items, size_t max_bytes) {
    Init(max_items, max_bytes);
}

void StrataReadLimit::Init(size_t max_items, size_t max_bytes) {
    max_items_ = max_items;
    max_bytes_ = max_bytes;
}

bool StrataReadLimit::ShouldContinue(const StrataReadManyInfo& info) const {
    if (max_items_ <= info.items) {
        return false;
    }
    if (max_bytes_ <= info.record_bytes) {
        return false;
    }
    return true;
}

}  // namespace base
}  // namespace io
}  // namespace strata
