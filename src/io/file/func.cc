#include "func.h"

#include "io/file/reader.h"
#include "io/file/writer.h"

namespace strata {
namespace io {
namespace file {

ReadOneResult ReadOneFromStrataFile(FILE* file, string* item) {
    StrataFileReader reader(file);
    return reader.ReadOne(item);
}

ReadManyResult ReadFromStrataFile(FILE* file, ReadLimit* limit,
                                  vector<string>* items,
                                  ReadOneResultStats* stats) {
    StrataFileReader reader(file);
    return reader.Read(limit, items, stats);
}

bool WriteOneToStrataFile(const WriteFlags& flags, const string& item,
                          FILE* file) {
    StrataFileWriter writer(file);
    return writer.WriteOne(flags, item);
}

size_t WriteToStrataFile(const WriteFlags& flags, const vector<string>& items,
                         FILE* file) {
    StrataFileWriter writer(file);
    return writer.Write(flags, items);
}

}  // namespace file
}  // namespace io
}  // namespace strata
