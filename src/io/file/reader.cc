#include "reader.h"

#include <cstdio>

namespace strata {
namespace io {
namespace file {

StrataFileReader::StrataFileReader(FILE* file) {
    file_ = file;
}

bool StrataFileReader::ReadRaw(uint32_t size, char* bytes,
                               uint32_t* bytes_read) {
    uint32_t count = static_cast<uint32_t>(fread(bytes, 1, size, file_));
    *bytes_read += count;
    return count == size;
}

bool StrataFileReader::IsAtEnd() const {
    return feof(file_);
}

}  // namespace file
}  // namespace io
}  // namespace strata
