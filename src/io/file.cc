#include "file.h"

#include <cstdio>

namespace strata {
namespace io {

FileReader::FileReader(FILE* file) {
    file_ = file;
}

bool FileReader::ReadRaw(size_t size, char* bytes) {
    return fread(bytes, 1, size, file_) == size;
}

bool FileReader::IsAtEnd() const {
    return feof(file_);
}

FileWriter::FileWriter(FILE* file) {
    file_ = file;
}

bool FileWriter::WriteRaw(const char* bytes, size_t size) {
    return fwrite(bytes, 1, size, file_) == size;
}

}  // namespace io
}  // namespace strata
