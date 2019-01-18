#include "buffer.h"

#include <cassert>
#include <cstring>

using std::memcpy;

namespace strata {
namespace io {

BufferReader::BufferReader(const char** data, const char* data_end) {
    data_ = data;
    data_end_ = data_end;
}

bool BufferReader::ReadRaw(size_t size, char* bytes) {
    assert(*data_ <= data_end_);
    if (data_end_ - *data_ < static_cast<long>(size)) {
        return false;
    }
    memcpy(bytes, *data_, size);
    *data_ += size;
    return true;
}

bool BufferReader::IsAtEnd() const {
    return *data_ == data_end_;
}

BufferWriter::BufferWriter(char** data, const char* data_end) {
    data_ = data;
    data_end_ = data_end;
}

bool BufferWriter::WriteRaw(const char* bytes, size_t size) {
    assert(*data_ <= data_end_);
    if (data_end_ - *data_ < static_cast<long>(size)) {
        return false;
    }
    memcpy(*data_, bytes, size);
    *data_ += size;
    return true;
}

}  // namespace io
}  // namespace strata
