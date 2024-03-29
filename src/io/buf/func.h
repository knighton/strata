#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataReadLimit;
using strata::io::base::StrataReadManyInfo;
using strata::io::base::StrataReadOneInfo;
using strata::io::base::StrataWriteFlags;

namespace strata {
namespace io {
namespace buf {

bool ReadOneFromStrataBuffer(const char** data, const char* data_end,
                             string* item, StrataReadOneInfo* info=nullptr);

size_t ReadFromStrataBuffer(const char** data, const char* data_end,
                            vector<string>* items=nullptr,
                            StrataReadManyInfo* info=nullptr,
                            StrataReadLimit* limit=nullptr);

bool WriteOneToStrataBuffer(const StrataWriteFlags& flags, const string& item,
                            char** data, const char* data_end);

size_t WriteToStrataBuffer(const StrataWriteFlags& flags,
                           const vector<string>& items, char** data,
                           const char* data_end);

}  // namespace buf
}  // namespace io
}  // namespace strata
