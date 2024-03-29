#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace strata {
namespace io {
namespace base {

// Flags bitfield used to control writing one entry.
struct StrataWriteFlags {
    bool snappy : 1;      // Use Snappy compression.
    bool crc32 : 1;       // Add CRC32 checksum.
    uint8_t padding : 6;  // Padding.
};

// writer abstract base class.
class StrataWriter {
  public:
    // Initialize with the state we are reading from (string, file, etc).
    virtual ~StrataWriter();

    // Write one entry to an object, advancing the state.
    //
    // Returns whether entry was successfully written.
    bool WriteOne(const StrataWriteFlags& flags, const string& item);

    // Write a sequence of entries to an object, advancing the state.
    //
    // Returns the number of entries successfully written.
    size_t Write(const StrataWriteFlags& flags, const vector<string>& items);

  protected:
    // Write some raw bytes, advancing the state.
    //
    // Returns whether all the bytes were successfully written.
    virtual bool WriteRaw(const char* bytes, size_t size) = 0;
    bool WriteRaw(const string& bytes);
    bool WriteRaw(uint16_t bytes);
    bool WriteRaw(uint32_t bytes);
};

}  // namespace base
}  // namespace io
}  // namespace strata
