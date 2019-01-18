#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace strata {
namespace base {

// Result of parsing one entry.
enum class ReadOneResult {
    OK = 0,          // Read the entry successfully.
    END = 1,         // Reached the end.  No data left to parse.
    INCOMPLETE = 2,  // Reached the end.  Incomplete/truncated data present.
    CHECKSUM = 3,    // Read entry, but checksum failure.
    SNAPPY = 4       // Read entry and checksum OK, but snappy fails.
};

// Result of parsing all entries.
struct ReadOneResultStats {
    // The counts (see ReadOneResult).
    uint32_t num_ok{0};              // Number of successfully parsed entries.
    uint32_t num_error_checksum{0};  // Number of entries with checksum failure.
    uint32_t num_error_snappy{0};    // Number of entries with invalid proto.

    // Reset counts to zeros.
    void Clear();
};

// Result of parsing some entries.
enum class ReadManyResult {
    OK = 0,         // Read the desired number of entries (keep going).
    END = 1,        // Reached the end.  No data left to parse.
    INCOMPLETE = 2  // Reached the end.  Incomplete/truncated data present.
};

// Tells when to stop reading items this call.
class ReadLimit {
  public:
    // Accessors.
    size_t max_items() const { return max_items_; }
    size_t max_bytes() const { return max_bytes_; }
    size_t items() const { return items_; }
    size_t bytes() const { return bytes_; }

    // Initialize with limits.
    ReadLimit(size_t max_items, size_t max_bytes);
    ReadLimit(size_t max_items);
    ReadLimit();

    // Note that we are at the beginning of a read many call.
    //
    // Clears the read bytes/item count.
    void Start();

    // Update the state with the results of a new read.
    //
    // Returns whether to continue reading items.
    bool ShouldContinue(size_t bytes);

  private:
    // Limits.
    size_t max_items_{~0UL};
    size_t max_bytes_{~0UL};

    // How much we have accumulated this call.
    size_t items_{0};
    size_t bytes_{0};
};

// Strata format reader abstract base class.
class Reader {
  public:
    // Initialize with the state we are reading from (string, file, etc).
    virtual ~Reader();

    // Read one entry, advancing the state.
    //
    // Sets "item" on success.
    //
    // Returns a status enum, which is zero on success.
    ReadOneResult ReadOne(string* item);

    // Read some entries.
    //
    // Configure "limit" to return after some number of entries or bytes have
    // been read, otherwise it reads until the end (nullptr means no limit).
    //
    // Appends parsed items to "items".
    //
    // Tracks statistics on the results of each entry read in "stats" (nullptr
    // means don't track).
    //
    // Returns a status enum, which is zero if there is more data left.
    ReadManyResult Read(ReadLimit* limit, vector<string>* items,
                        ReadOneResultStats* stats);

  protected:
    // Read some raw bytes, advancing the state.
    //
    // Returns whether all the bytes were successfully read.
    virtual bool ReadRaw(size_t size, char* bytes) = 0;
    bool ReadRaw(size_t size, string* bytes);
    bool ReadRaw(uint16_t* bytes);
    bool ReadRaw(uint32_t* bytes);

    // Returns whether we are at the end of the string/file/etc.
    virtual bool IsAtEnd() const = 0;
};

}  // namespace base
}  // namespace strata
