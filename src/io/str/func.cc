#include "func.h"

#include "io/str/reader.h"
#include "io/str/writer.h"

namespace strata {
namespace io {
namespace str {

bool ReadOneFromStrataString(const string& data, size_t* index, string* item,
                             StrataReadOneInfo* info) {
    StrataStringReader reader(&data, index);
    return reader.ReadOne(item, info);
}

size_t ReadFromStrataString(const string& data, size_t* index,
                            vector<string>* items, StrataReadManyInfo* info,
                            StrataReadLimit* limit) {
    StrataStringReader reader(&data, index);
    return reader.Read(items, info, limit);
}

bool WriteOneToStrataString(const StrataWriteFlags& flags, const string& item,
                            string* data) {
    StrataStringWriter writer(data);
    return writer.WriteOne(flags, item);
}

size_t WriteToStrataString(const StrataWriteFlags& flags,
                           const vector<string>& items, string* data) {
    StrataStringWriter writer(data);
    return writer.Write(flags, items);
}

}  // namespace str
}  // namespace io
}  // namespace strata
