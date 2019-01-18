#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::ReadLimit;
using strata::io::base::ReadManyResult;
using strata::io::base::ReadOneResult;
using strata::io::base::ReadOneResultStats;
using strata::io::base::WriteFlags;

namespace strata {
namespace io {
namespace file {

ReadOneResult ReadOneFromStrataFile(FILE* file, string* item);

ReadManyResult ReadFromStrataFile(FILE* file, ReadLimit* limit,
                                  vector<string>* items,
                                  ReadOneResultStats* stats);

bool WriteOneToStrataFile(const WriteFlags& flags, const string& item,
                          FILE* file);

size_t WriteToStrataFile(const WriteFlags& flags, const vector<string>& items,
                         FILE* file);

}  // namespace file
}  // namespace io
}  // namespace strata
