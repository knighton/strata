#include "func.h"

#include "io/file/reader.h"
#include "io/file/writer.h"

namespace strata {
namespace io {
namespace file {

StrataReadOneResult ReadOneFromStrataFile(FILE* file, string* item) {
    StrataFileReader reader(file);
    return reader.ReadOne(item);
}

StrataReadManyResult ReadFromStrataFile(
        FILE* file, vector<string>* items, StrataReadLimit* limit,
        StrataReadOneResultStats* stats) {
    StrataFileReader reader(file);
    return reader.Read(items, limit, stats);
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
