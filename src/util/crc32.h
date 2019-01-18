#pragma once

#include <string>

using std::string;

namespace strata {
namespace util {

// Allocate global state used for calculating CRC32.
//
// Not thread-safe.
void InitCRC32();

// Delete global state.
//
// Not thread-safe.
void DestroyCRC32();

// Calculate the CRC32.
//
// Assumes the global state is initailized (call InitCRC32).
uint32_t CRC32(const char* data, size_t size);
uint32_t CRC32(const string& data);

}  // namespace util
}  // namespace strata
