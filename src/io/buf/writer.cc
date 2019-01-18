#include "writer.h"

#include <cassert>
#include <cstring>

using std::memcpy;

namespace strata {
namespace io {
namespace buf {

StrataBufferWriter::StrataBufferWriter(char** data, const char* data_end) {
    data_ = data;
    data_end_ = data_end;
}

bool StrataBufferWriter::WriteRaw(const char* bytes, size_t size) {
    assert(*data_ <= data_end_);
    if (data_end_ - *data_ < static_cast<long>(size)) {
        return false;
    }
    memcpy(*data_, bytes, size);
    *data_ += size;
    return true;
}

}  // namespace buf
}  // namespace io
}  // namespace strata
