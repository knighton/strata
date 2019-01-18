#include "writer.h"

#include <cstdio>

namespace strata {
namespace io {
namespace file {

StrataFileWriter::StrataFileWriter(FILE* file) {
    file_ = file;
}

bool StrataFileWriter::WriteRaw(const char* bytes, size_t size) {
    return fwrite(bytes, 1, size, file_) == size;
}

}  // namespace file
}  // namespace io
}  // namespace strata
