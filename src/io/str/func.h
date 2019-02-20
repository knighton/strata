#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataReadLimit;
using strata::io::base::StrataReadManyInfo;
using strata::io::base::StrataReadOneInfo;
using strata::io::base::StrataWriteFlags;

namespace strata {
namespace io {
namespace str {

bool ReadOneFromStrataString(const string& data, size_t* index, string* item,
                             StrataReadOneInfo* info=nullptr);

bool ReadFromStrataString(const string& data, size_t* index,
                          vector<string>* items=nullptr,
                          StrataReadManyInfo* info=nullptr,
                          StrataReadLimit* limit=nullptr);

bool WriteOneToStrataString(const StrataWriteFlags& flags, const string& item,
                            string* data);

size_t WriteToStrataString(const StrataWriteFlags& flags,
                           const vector<string>& items, string* data);

}  // namespace str
}  // namespace io
}  // namespace strata
