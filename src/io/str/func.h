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
namespace str {

StrataReadOneResult ReadOneFromStrataString(const string& data, size_t* index,
                                            string* item);

StrataReadManyResult ReadFromStrataString(
    const string& data, size_t* index, StrataReadLimit* limit,
    vector<string>* items, StrataReadOneResultStats* stats);

bool WriteOneToStrataString(const StrataWriteFlags& flags, const string& item,
                            string* data);

size_t WriteToStrataString(const StrataWriteFlags& flags,
                           const vector<string>& items, string* data);

}  // namespace str
}  // namespace io
}  // namespace strata
