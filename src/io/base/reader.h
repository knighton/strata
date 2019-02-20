#pragma once

#include <string>
#include <vector>

#include "read_many.h"
#include "read_one.h"

using std::string;
using std::vector;

namespace strata {
namespace io {
namespace base {

// Strata format reader abstract base class.
class StrataReader {
  public:
    // Initialize with the state we are reading from (string, file, etc).
    virtual ~StrataReader();

    // Read one entry, advancing the state.
    //
    // Saves detailed info about the entry in "info" (nullptr to not save).
    //
    // Sets "item" and returns true iff success.
    bool ReadOne(string* item, StrataReadOneInfo* info=nullptr);

    // Read some entries, advancing the state.
    //
    // Appends read items to "items" (nullptr means don't save).
    //
    // Tracks statistics on the results of each entry read in "stats" (nullptr
    // means don't track).
    //
    // Configure "limit" to return after some number of entries or bytes have
    // been read, otherwise it reads until the end (nullptr means no limit).
    //
    // Returns the number of items read.
    size_t Read(vector<string>* items=nullptr, StrataReadManyInfo* info=nullptr,
                StrataReadLimit* limit=nullptr);

  protected:
    // Read some raw bytes, advancing the state.
    //
    // Returns whether all the bytes were successfully read.
    virtual bool ReadRaw(uint32_t size, char* bytes, uint32_t* bytes_read) = 0;
    bool ReadRawStr(uint32_t size, string* bytes, uint32_t* bytes_read);
    bool ReadRawU16(uint16_t* bytes, uint32_t* bytes_read);
    bool ReadRawU32(uint32_t* bytes, uint32_t* bytes_read);

    // Returns whether we are at the end of the string/file/etc.
    virtual bool IsAtEnd() const = 0;
};

}  // namespace base
}  // namespace io
}  // namespace strata
