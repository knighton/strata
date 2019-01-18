#include "func.h"

#include "io/buf/reader.h"
#include "io/buf/writer.h"

namespace strata {
namespace io {
namespace buf {

ReadOneResult ReadOneFromStrataBuffer(const char** data, const char* data_end,
                                      string* item) {
    StrataBufferReader reader(data, data_end);
    return reader.ReadOne(item);
}

ReadManyResult ReadFromStrataBuffer(const char** data, const char* data_end,
                                    ReadLimit* limit, vector<string>* items,
                                    ReadOneResultStats* stats) {
    StrataBufferReader reader(data, data_end);
    return reader.Read(limit, items, stats);
}

bool WriteOneToStrataBuffer(const WriteFlags& flags, const string& item,
                            char** data, const char* data_end) {
    StrataBufferWriter writer(data, data_end);
    return writer.WriteOne(flags, item);
}

size_t WriteToStrataBuffer(const WriteFlags& flags, const vector<string>& items,
                           char** data, const char* data_end) {
    StrataBufferWriter writer(data, data_end);
    return writer.Write(flags, items);
}

}  // namespace buf
}  // namespace io
}  // namespace strata
