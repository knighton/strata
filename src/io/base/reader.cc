#include "reader.h"

#include <cassert>
#include <snappy.h>

#include "util/crc32.h"

using strata::util::CRC32;

namespace strata {
namespace io {
namespace base {

StrataReader::~StrataReader() {
}

bool StrataReader::ReadOne(string* payload, StrataReadOneInfo* info) {
    uint32_t bytes_read = 0;

    if (IsAtEnd()) {
        info->InitEnd();
        return false;
    }

    // Parse the flags and paylod size.
    uint16_t first;
    if (!ReadRawU16(&first, &bytes_read)) {
        info->InitTruncatedHead(bytes_read);
        return false;
    }
    bool has_long_length = first & (1 << 15);
    bool used_snappy = first & (1 << 14);
    bool has_crc32 = first & (1 << 13);
    first &= 0x1FFF;
    uint32_t payload_size;
    if (has_long_length) {
        uint16_t second;
        if (!ReadRawU16(&second, &bytes_read)) {
            info->InitTruncatedHead(bytes_read);
            return false;
        }
        payload_size = (static_cast<uint32_t>(first) << 16) | second;
    } else {
        payload_size = first;
    }

    // Get the checksum, if present.
    uint32_t crc32;
    if (has_crc32) {
        if (!ReadRawU32(&crc32, &bytes_read)) {
            info->InitTruncatedCRC32(
                has_long_length, has_crc32, used_snappy, payload_size,
                bytes_read);
            return false;
        }
    } else {
        crc32 = 0;  // Linter not smart enough.
    }

    // Now get the payload.
    string temp;
    if (!ReadRawStr(payload_size, &temp, &bytes_read)) {
        info->InitTruncatedPayload(has_long_length, has_crc32, used_snappy,
                                   payload_size, bytes_read);
        return false;
    }

    // If checksum, apply it.
    if (has_crc32) {
        uint32_t redo_crc32 = CRC32(temp.data(), temp.size());
        if (crc32 != redo_crc32) {
            info->InitErrCRC32(has_long_length, has_crc32, used_snappy,
                               payload_size);
            return false;
        }
    }

    // If compressed, uncompress it.
    uint32_t decoded_payload_size;
    if (used_snappy) {
        string uncompressed_temp;
        if (!snappy::Uncompress(temp.data(), temp.size(), &uncompressed_temp)) {
            info->InitErrSnappy(has_long_length, has_crc32, used_snappy,
                                payload_size);
            return false;
        }
        temp.swap(uncompressed_temp);
        decoded_payload_size = static_cast<uint32_t>(temp.size());
    } else {
        decoded_payload_size = payload_size;
    }

    // Finally, set the payload.
    payload->swap(temp);
    info->InitOK(has_long_length, has_crc32, used_snappy, payload_size,
                 decoded_payload_size);
    return true;
}

bool StrataReader::Read(vector<string>* items, StrataReadManyInfo* info,
                        StrataReadLimit* limit) {
    if (info) {
        info->Init();
    }
    string item;
    StrataReadOneInfo read_one_info;
    StrataReadOneInfo* read_one_info_ptr = info ? &read_one_info : nullptr;
    while (true) {
        if (limit && !limit->ShouldContinue(*info)) {
            return true;
        }
        bool ok = ReadOne(&item, read_one_info_ptr);
        if (items) {
            items->emplace_back(item);
        }
        if (info) {
            info->Add(read_one_info);
        }
        if (!ok) {
            return false;
        }
    }
}

bool StrataReader::ReadRawStr(uint32_t size, string* bytes,
                              uint32_t* bytes_read) {
    bytes->resize(size);
    return ReadRaw(size, &(*bytes)[0], bytes_read);
}

bool StrataReader::ReadRawU16(uint16_t* bytes, uint32_t* bytes_read) {
    return ReadRaw(sizeof(*bytes), reinterpret_cast<char*>(bytes), bytes_read);
}

bool StrataReader::ReadRawU32(uint32_t* bytes, uint32_t* bytes_read) {
    return ReadRaw(sizeof(*bytes), reinterpret_cast<char*>(bytes), bytes_read);
}

}  // namespace base
}  // namespace io
}  // namespace strata
