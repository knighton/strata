#include "func.h"

#include "io/buf/reader.h"
#include "io/buf/writer.h"

namespace strata {
namespace io {
namespace buf {

StrataReadOneResult ReadOneFromStrataBuffer(
        const char** data, const char* data_end, string* item) {
    StrataBufferReader reader(data, data_end);
    return reader.ReadOne(item);
}

StrataReadManyResult ReadFromStrataBuffer(
        const char** data, const char* data_end, vector<string>* items,
        StrataReadLimit* limit, StrataReadOneResultStats* stats) {
    StrataBufferReader reader(data, data_end);
    return reader.Read(items, limit, stats);
}

bool WriteOneToStrataBuffer(
        const StrataWriteFlags& flags, const string& item, char** data,
        const char* data_end) {
    StrataBufferWriter writer(data, data_end);
    return writer.WriteOne(flags, item);
}

size_t WriteToStrataBuffer(
        const StrataWriteFlags& flags, const vector<string>& items, char** data,
        const char* data_end) {
    StrataBufferWriter writer(data, data_end);
    return writer.Write(flags, items);
}

}  // namespace buf
}  // namespace io
}  // namespace strata
