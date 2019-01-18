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
namespace str {

ReadOneResult ReadOneFromStrataString(const string& data, size_t* index,
                                      string* item);

ReadManyResult ReadFromStrataString(const string& data, size_t* index,
                                    ReadLimit* limit, vector<string>* items,
                                    ReadOneResultStats* stats);

bool WriteOneToStrataString(const WriteFlags& flags, const string& item,
                            string* data);

size_t WriteToStrataString(const WriteFlags& flags, const vector<string>& items,
                           string* data);

}  // namespace str
}  // namespace io
}  // namespace strata
