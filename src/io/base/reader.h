#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace strata {
namespace io {
namespace base {

// Result of parsing one entry.
enum class StrataReadOneResult {
    OK = 0,          // Read the entry successfully.
    END = 1,         // Reached the end.  No data left to parse.
    INCOMPLETE = 2,  // Reached the end.  Incomplete/truncated data present.
    CHECKSUM = 3,    // Read entry, but checksum failure.
    SNAPPY = 4       // Read entry and checksum OK, but snappy fails.
};

// Result of parsing all entries.
struct StrataReadOneResultStats {
    // The counts (see StrataReadOneResult).
    uint32_t num_ok{0};              // Number of successfully parsed entries.
    uint32_t num_error_checksum{0};  // Number of entries with checksum failure.
    uint32_t num_error_snappy{0};    // Number of entries with invalid proto.

    // Reset counts to zeros.
    void Clear();
};

// Result of parsing some entries.
enum class StrataReadManyResult {
    OK = 0,         // Read the desired number of entries (keep going).
    END = 1,        // Reached the end.  No data left to parse.
    INCOMPLETE = 2  // Reached the end.  Incomplete/truncated data present.
};

// Tells when to stop reading items this call.
//
// Examples
// --------
//
// Can't stop won't stop:
//
//     StrataReadLimit limit;
//
// Stop after a maximum of 64 items.
//
//     StrataReadLimit limit(64);
//
// Stop after reading items totaling 16384 bytes (total -- valid or otherwise).
//
//     StrataReadLimit limit(StrataReadLimit::NO_ITEM_LIMIT, 1 << 14);
//
// Stop after either hitting 10 items or 1024 bytes, whichever is first:
//
//     StrataReadLimit limit(10, 1024);
class StrataReadLimit {
  public:
    // No limit on the number of items read.
    static const size_t NO_ITEM_LIMIT{~0UL};

    // No limit on the number of bytes read.
    static const size_t NO_BYTE_LIMIT{~0UL};

    // Accessors.
    size_t max_items() const { return max_items_; }
    size_t max_bytes() const { return max_bytes_; }
    size_t items() const { return items_; }
    size_t bytes() const { return bytes_; }

    // Initialize with the given (default = no) limits.  Calls Init().
    StrataReadLimit(size_t max_items=NO_ITEM_LIMIT,
                    size_t max_bytes=NO_BYTE_LIMIT);

    // Initialize with the given (default = no) limits, resetting the counters.
    void Init(size_t max_items=NO_ITEM_LIMIT, size_t max_bytes=NO_BYTE_LIMIT);

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
    size_t max_items_{NO_ITEM_LIMIT};
    size_t max_bytes_{NO_BYTE_LIMIT};

    // How much we have accumulated this call.
    size_t items_{0};
    size_t bytes_{0};
};

// Strata format reader abstract base class.
class StrataReader {
  public:
    // Initialize with the state we are reading from (string, file, etc).
    virtual ~StrataReader();

    // Read one entry, advancing the state.
    //
    // Sets "item" iff success.
    //
    // Returns a status enum, which is zero on success.
    StrataReadOneResult ReadOne(string* item);

    // Read some entries.
    //
    // Appends read items to "items" (nullptr means don't save).
    //
    // Configure "limit" to return after some number of entries or bytes have
    // been read, otherwise it reads until the end (nullptr means no limit).
    //
    // Tracks statistics on the results of each entry read in "stats" (nullptr
    // means don't track).
    //
    // Returns a status enum, which is zero if there is more data left.
    //
    // Examples
    // --------
    //
    // Read to the end:
    //
    //     Strata...Reader reader(...);
    //     vector<string> items;
    //     if (reader.Read(&items)) {
    //         TellUserFileIsTruncated();
    //     }
    //     HandleItems(items);
    //
    // Read and handle 64 items at a time:
    //
    //     Strata...Reader reader(...);
    //     vector<string> items;
    //     StrataReadLimit limit(64);
    //     while (!reader.Read(&items, &limit)) {
    //         HandleItems(items);
    //         items.clear();
    //     }
    //
    // Items vary a lot in size and we are interested in corruption:
    //
    //     Strata...Reader reader(...);
    //     vector<string> items;
    //     StrataReadLimit limit(StrataReadLimit::NO_ITEM_LIMIT, 1 << 15);
    //     StrataReadOneResultStats stats;
    //     while (!reader.Read(&items, &limit, &stats)) {
    //         HandleItems(items, stats);
    //         items.clear();
    //     }
    //
    // Just validate the thing:
    //
    //     Strata...Reader reader(...);
    //     StrataReadOneResultStats stats;
    //     auto code = reader.Read(nullptr, nullptr, &stats);
    //     HandleReadResults(code, stats);
    //
    // Skip 100 items:
    //
    //     Strata...Reader reader(...);
    //     ...
    //     StrataReadLimit limit(100);
    //     if (reader.Read(nullptr, &limit)) {
    //         HandleError();
    //         return;
    //     }
    //     ...
    StrataReadManyResult Read(vector<string>* items=nullptr,
                              StrataReadLimit* limit=nullptr,
                              StrataReadOneResultStats* stats=nullptr);

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
}  // namespace io
}  // namespace strata
