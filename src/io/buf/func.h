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
namespace buf {

StrataReadOneResult ReadOneFromStrataBuffer(
    const char** data, const char* data_end, string* item);

StrataReadManyResult ReadFromStrataBuffer(
    const char** data, const char* data_end, vector<string>* items=nullptr,
    StrataReadLimit* limit=nullptr, StrataReadOneResultStats* stats=nullptr);

bool WriteOneToStrataBuffer(const StrataWriteFlags& flags, const string& item,
                            char** data, const char* data_end);

size_t WriteToStrataBuffer(const StrataWriteFlags& flags,
                           const vector<string>& items, char** data,
                           const char* data_end);

}  // namespace buf
}  // namespace io
}  // namespace strata
