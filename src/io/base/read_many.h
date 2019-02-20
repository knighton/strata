#pragma once

#include <string>
#include <vector>

#include "read_one.h"

using std::string;
using std::vector;

namespace strata {
namespace io {
namespace base {

// Details about a sequence of Strata records that were read.
struct StrataReadManyInfo {
    // Reason why read many exited.
    StrataReadOneExit exit;

    // Records/bytes with checksum failures.
    uint32_t err_crc32_items{0};
    uint32_t err_crc32_record_bytes{0};
    uint32_t err_crc32_encoded_payload_bytes{0};

    // Records/bytes with uncompression failures.
    uint32_t err_snappy_items{0};
    uint32_t err_snappy_record_bytes{0};
    uint32_t err_snappy_encoded_payload_bytes{0};

    // Records/bytes that were read OK.
    uint32_t ok_items{0};
    uint32_t ok_record_bytes{0};
    uint32_t ok_encoded_payload_bytes{0};
    uint32_t ok_decoded_payload_bytes{0};

    // Statistics about how (valid) records were encoded.
    uint32_t ok_big_head{0};     // Portion of OK records with long head field.
    uint32_t ok_had_crc32{0};    // Portion of OK records with checksums.
    uint32_t ok_used_snappy{0};  // Portion of OK records that were compressed.

    // Records/bytes in total.
    uint32_t items{0};
    uint32_t record_bytes{0};
    uint32_t encoded_payload_bytes{0};
    uint32_t decoded_payload_bytes{0};

    // Initialize stats to zeros.
    void Init();

    // Add stats given one read one result.
    void Add(const StrataReadOneInfo& info);
};

// Tells when to stop reading items during a read many call.
class StrataReadLimit {
  public:
    // No limit on the number of items read.
    static const size_t NO_ITEM_LIMIT{~0UL};

    // No limit on the number of bytes read.
    static const size_t NO_BYTE_LIMIT{~0UL};

    // Accessors.
    size_t max_items() const { return max_items_; }
    size_t max_bytes() const { return max_bytes_; }

    // Initialize with the given (default = no) limits.  Calls Init().
    StrataReadLimit(size_t max_items=NO_ITEM_LIMIT,
                    size_t max_bytes=NO_BYTE_LIMIT);

    // Initialize with the given (default = no) limits.
    void Init(size_t max_items=NO_ITEM_LIMIT, size_t max_bytes=NO_BYTE_LIMIT);

    // Returns whether to continue reading items.
    bool ShouldContinue(const StrataReadManyInfo& info) const;

  private:
    // Limits.
    size_t max_items_{NO_ITEM_LIMIT};
    size_t max_bytes_{NO_BYTE_LIMIT};
};

}  // namespace base
}  // namespace io
}  // namespace strata
