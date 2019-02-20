#include "reader.h"

#include <cassert>
#include <snappy.h>

#include "util/crc32.h"

using strata::util::CRC32;

namespace strata {
namespace io {
namespace base {

bool StrataReadOneInfo::was_head_valid() const {
    return exit_ != StrataReadOneExit::END &&
        exit_ != StrataReadOneExit::TRUNCATED_HEAD;
}

StrataReadOneExit StrataReadOneInfo::exit() const {
    return exit_;
}

bool StrataReadOneInfo::ok() const {
    return exit_ == StrataReadOneExit::OK;
}

uint32_t StrataReadOneInfo::head_size() const {
    assert(was_head_valid());
    return big_head_ ? 4 : 2;
}

bool StrataReadOneInfo::had_crc32() const {
    assert(was_head_valid());
    return had_crc32_;
}

bool StrataReadOneInfo::used_snappy() const {
    assert(was_head_valid());
    return used_snappy_;
}

uint32_t StrataReadOneInfo::header_size() const {
    uint32_t size = head_size();
    if (had_crc32_) {
        size += 4;
    }
    return size;
}

uint32_t StrataReadOneInfo::record_size_in_header() const {
    switch (exit_) {
        case StrataReadOneExit::END:
            assert(false);
        case StrataReadOneExit::TRUNCATED_HEAD:
            assert(false);
        case StrataReadOneExit::TRUNCATED_CRC32:
            return header_size() + xa_;
        case StrataReadOneExit::TRUNCATED_PAYLOAD:
            return header_size() + xa_;
        case StrataReadOneExit::ERR_CRC32:
            return xa_;
        case StrataReadOneExit::ERR_SNAPPY:
            return xa_;
        case StrataReadOneExit::OK:
            return header_size() + xa_;
    }
}

uint32_t StrataReadOneInfo::payload_size_in_header() const {
    switch (exit_) {
        case StrataReadOneExit::END:
            assert(false);
        case StrataReadOneExit::TRUNCATED_HEAD:
            assert(false);
        case StrataReadOneExit::TRUNCATED_CRC32:
            return xa_;
        case StrataReadOneExit::TRUNCATED_PAYLOAD:
            return xa_;
        case StrataReadOneExit::ERR_CRC32:
            return xb_;
        case StrataReadOneExit::ERR_SNAPPY:
            return xa_;
        case StrataReadOneExit::OK:
            return xa_;
    }
}

uint32_t StrataReadOneInfo::record_size() const {
    switch (exit_) {
        case StrataReadOneExit::END:
            assert(false);
        case StrataReadOneExit::TRUNCATED_HEAD:
            return xa_;
        case StrataReadOneExit::TRUNCATED_CRC32:
            return xb_;
        case StrataReadOneExit::TRUNCATED_PAYLOAD:
            return header_size() + xb_;
        case StrataReadOneExit::ERR_CRC32:
            return header_size() + xa_;
        case StrataReadOneExit::ERR_SNAPPY:
            return header_size() + xa_;
        case StrataReadOneExit::OK:
            return header_size() + xa_;
    }
}

uint32_t StrataReadOneInfo::encoded_payload_size() const {
    switch (exit_) {
        case StrataReadOneExit::END:
            assert(false);
        case StrataReadOneExit::TRUNCATED_HEAD:
            assert(false);
        case StrataReadOneExit::TRUNCATED_CRC32:
            assert(false);
        case StrataReadOneExit::TRUNCATED_PAYLOAD:
            return xb_;
        case StrataReadOneExit::ERR_CRC32:
            return xa_;
        case StrataReadOneExit::ERR_SNAPPY:
            return xa_;
        case StrataReadOneExit::OK:
            return xa_;
    }
}

uint32_t StrataReadOneInfo::decoded_payload_size() const {
    switch (exit_) {
        case StrataReadOneExit::END:
            assert(false);
        case StrataReadOneExit::TRUNCATED_HEAD:
            assert(false);
        case StrataReadOneExit::TRUNCATED_CRC32:
            assert(false);
        case StrataReadOneExit::TRUNCATED_PAYLOAD:
            assert(false);
        case StrataReadOneExit::ERR_CRC32:
            assert(false);
        case StrataReadOneExit::ERR_SNAPPY:
            assert(false);
        case StrataReadOneExit::OK:
            return xb_;
    }
}

void StrataReadOneInfo::InitEnd() {
    exit_ = StrataReadOneExit::END;
    big_head_ = false;
    had_crc32_ = false;
    used_snappy_ = false;
    xa_ = 0;
    xb_ = 0;
}

void StrataReadOneInfo::InitTruncatedHead(uint32_t truncated_record_size) {
    exit_ = StrataReadOneExit::TRUNCATED_HEAD;
    big_head_ = false;
    had_crc32_ = false;
    used_snappy_ = false;
    xa_ = truncated_record_size;
    xb_ = 0;
}

void StrataReadOneInfo::InitTruncatedCRC32(
        bool big_head, bool had_crc32, bool used_snappy,
        uint32_t payload_size_in_header, uint32_t truncated_record_size) {
    exit_ = StrataReadOneExit::TRUNCATED_CRC32;
    big_head_ = big_head;
    had_crc32_ = had_crc32;
    used_snappy_ = used_snappy;
    xa_ = payload_size_in_header;
    xb_ = truncated_record_size;
}

void StrataReadOneInfo::InitTruncatedPayload(
        bool big_head, bool had_crc32, bool used_snappy,
        uint32_t payload_size_in_header, uint32_t truncated_record_size) {
    exit_ = StrataReadOneExit::TRUNCATED_PAYLOAD;
    big_head_ = big_head;
    had_crc32_ = had_crc32;
    used_snappy_ = used_snappy;
    xa_ = payload_size_in_header;
    xb_ = truncated_record_size - header_size();
}

void StrataReadOneInfo::InitErrCRC32(
        bool big_head, bool had_crc32, bool used_snappy,
        uint32_t encoded_payload_size) {
    exit_ = StrataReadOneExit::ERR_CRC32;
    big_head_ = big_head;
    had_crc32_ = had_crc32;
    used_snappy_ = used_snappy;
    xa_ = encoded_payload_size;
    xb_ = 0;
}

void StrataReadOneInfo::InitErrSnappy(
        bool big_head, bool had_crc32, bool used_snappy,
        uint32_t encoded_payload_size) {
    exit_ = StrataReadOneExit::ERR_SNAPPY;
    big_head_ = big_head;
    had_crc32_ = had_crc32;
    used_snappy_ = used_snappy;
    xa_ = encoded_payload_size;
    xb_ = 0;
}

void StrataReadOneInfo::InitOK(
        bool big_head, bool had_crc32, bool used_snappy,
        uint32_t encoded_payload_size, uint32_t decoded_payload_size) {
    exit_ = StrataReadOneExit::OK;
    big_head_ = big_head;
    had_crc32_ = had_crc32;
    used_snappy_ = used_snappy;
    xa_ = encoded_payload_size;
    xb_ = decoded_payload_size;
}

}  // namespace base
}  // namespace io
}  // namesapce strata
