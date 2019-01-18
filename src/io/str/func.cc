#include "func.h"

#include "io/str/reader.h"
#include "io/str/writer.h"

namespace strata {
namespace io {
namespace str {

StrataReadOneResult ReadOneFromStrataString(const string& data, size_t* index,
                                            string* item) {
    StrataStringReader reader(&data, index);
    return reader.ReadOne(item);
}

StrataReadManyResult ReadFromStrataString(
        const string& data, size_t* index, StrataReadLimit* limit,
        vector<string>* items, StrataReadOneResultStats* stats) {
    StrataStringReader reader(&data, index);
    return reader.Read(limit, items, stats);
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
