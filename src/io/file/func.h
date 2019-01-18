#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataReadLimit;
using strata::io::base::StrataReadManyResult;
using strata::io::base::StrataReadOneResult;
using strata::io::base::StrataReadOneResultStats;
using strata::io::base::StrataWriteFlags;

namespace strata {
namespace io {
namespace file {

StrataReadOneResult ReadOneFromStrataFile(FILE* file, string* item);

StrataReadManyResult ReadFromStrataFile(
    FILE* file, vector<string>* items=nullptr, StrataReadLimit* limit=nullptr,
    StrataReadOneResultStats* stats=nullptr);

bool WriteOneToStrataFile(const StrataWriteFlags& flags, const string& item,
                          FILE* file);

size_t WriteToStrataFile(const StrataWriteFlags& flags,
                         const vector<string>& items, FILE* file);

}  // namespace file
}  // namespace io
}  // namespace strata
