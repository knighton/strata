#include "func.h"

#include "io/file/reader.h"
#include "io/file/writer.h"

namespace strata {
namespace io {
namespace file {

bool ReadOneFromStrataFile(FILE* file, string* item, StrataReadOneInfo* info) {
    StrataFileReader reader(file);
    return reader.ReadOne(item, info);
}

size_t ReadFromStrataFile(FILE* file, vector<string>* items,
                          StrataReadManyInfo* info, StrataReadLimit* limit) {
    StrataFileReader reader(file);
    return reader.Read(items, info, limit);
}

bool WriteOneToStrataFile(const StrataWriteFlags& flags, const string& item,
                          FILE* file) {
    StrataFileWriter writer(file);
    return writer.WriteOne(flags, item);
}

size_t WriteToStrataFile(const StrataWriteFlags& flags,
                         const vector<string>& items, FILE* file) {
    StrataFileWriter writer(file);
    return writer.Write(flags, items);
}

}  // namespace file
}  // namespace io
}  // namespace strata
