#include "reader.h"

#include <cstring>

using std::memcpy;

namespace strata {
namespace io {
namespace str {

StrataStringReader::StrataStringReader(const string* data, size_t* index) {
    data_ = data;
    index_ = index;
}

bool StrataStringReader::ReadRaw(uint32_t size, char* bytes,
                                 uint32_t* bytes_read) {
    if (data_->size() < *index_ + size) {
        return false;
    }
    memcpy(bytes, &(*data_)[*index_], size);
    *index_ += size;
    *bytes_read += size;
    return true;
}

bool StrataStringReader::IsAtEnd() const {
    return *index_ == data_->size();
}

}  // namespace str
}  // namespace io
}  // namespace strata
