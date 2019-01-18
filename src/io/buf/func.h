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
namespace buf {

ReadOneResult ReadOneFromStrataBuffer(const char** data, const char* data_end,
                                      string* item);

ReadManyResult ReadFromStrataBuffer(const char** data, const char* data_end,
                                    ReadLimit* limit, vector<string>* items,
                                    ReadOneResultStats* stats);

bool WriteOneToStrataBuffer(const WriteFlags& flags, const string& item,
                            char** data, const char* data_end);

size_t WriteToStrataBuffer(const WriteFlags& flags, const vector<string>& items,
                           char** data, const char* data_end);

}  // namespace buf
}  // namespace io
}  // namespace strata
