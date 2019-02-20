#include "func.h"

#include "io/buf/reader.h"
#include "io/buf/writer.h"

namespace strata {
namespace io {
namespace buf {

bool ReadOneFromStrataBuffer(const char** data, const char* data_end,
                             string* item, StrataReadOneInfo* info) {
    StrataBufferReader reader(data, data_end);
    return reader.ReadOne(item, info);
}

size_t ReadFromStrataBuffer(const char** data, const char* data_end,
                            vector<string>* items, StrataReadManyInfo* info,
                            StrataReadLimit* limit) {
    StrataBufferReader reader(data, data_end);
    return reader.Read(items, info, limit);
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
