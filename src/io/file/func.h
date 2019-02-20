#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataReadLimit;
using strata::io::base::StrataReadManyInfo;
using strata::io::base::StrataReadOneInfo;
using strata::io::base::StrataWriteFlags;

namespace strata {
namespace io {
namespace file {

bool ReadOneFromStrataFile(FILE* file, string* item,
                           StrataReadOneInfo* info=nullptr);

size_t ReadFromStrataFile(FILE* file, vector<string>* items=nullptr,
                          StrataReadManyInfo* info=nullptr,
                          StrataReadLimit* limit=nullptr);

bool WriteOneToStrataFile(const StrataWriteFlags& flags, const string& item,
                          FILE* file);

size_t WriteToStrataFile(const StrataWriteFlags& flags,
                           const vector<string>& items, FILE* file);

}  // namespace file
}  // namespace io
}  // namespace strata
