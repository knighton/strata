#include "reader.h"

#include <cstdio>

namespace strata {
namespace io {
namespace file {

StrataFileReader::StrataFileReader(FILE* file) {
    file_ = file;
}

bool StrataFileReader::ReadRaw(size_t size, char* bytes) {
    return fread(bytes, 1, size, file_) == size;
}

bool StrataFileReader::IsAtEnd() const {
    return feof(file_);
}

}  // namespace file
}  // namespace io
}  // namespace strata
