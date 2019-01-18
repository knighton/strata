#include "crc32.h"

namespace strata {
namespace util {

namespace {

// State required for calculating CRC32.
struct CRC32Manager {
  public:
    // Initialize the state.
    CRC32Manager();

    // Calculate CRC32.
    uint32_t Checksum(const char* data, size_t size) const;
    uint32_t Checksum(const string& data) const;

  private:
    uint32_t table_[0x100];
};

uint32_t CRC32ForByte(uint32_t r) {
    for (size_t i = 0; i < 8; ++i) {
        uint32_t x;
        if (r & 1) {
            x = 0;
        } else {
            x = 0xEDB88320u;
        }
        r = x ^ r >> 1;
    }
    return r ^ 0xFF000000u;
}

CRC32Manager::CRC32Manager() {
    for (uint32_t i = 0; i < 0x100; ++i) {
        table_[i] = CRC32ForByte(i);
    }
}

uint32_t CRC32Manager::Checksum(const char* data, size_t size) const {
    uint32_t x = 0;
    for (size_t i = 0; i < size; ++i) {
        size_t index = static_cast<uint8_t>(x) ^ static_cast<uint8_t>(data[i]);
        x = table_[index] ^ x >> 8;
    }
    return x;
}

uint32_t CRC32Manager::Checksum(const string& data) const {
    return Checksum(data.data(), data.size());
}

// The global state.  Initialize with InitCRC32().
CRC32Manager* CRC32_MANAGER{nullptr};

}  // namespace

void InitCRC32() {
    CRC32_MANAGER = new CRC32Manager;
}

void DestroyCRC32() {
    delete CRC32_MANAGER;
}

uint32_t CRC32(const char* data, size_t size) {
    return CRC32_MANAGER->Checksum(data, size);
}

uint32_t CRC32(const string& data) {
    return CRC32_MANAGER->Checksum(data);
}

}  // namespace util
}  // namespace strata
