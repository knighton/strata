#include "reader.h"

#include <cassert>
#include <cstring>

using std::memcpy;

namespace strata {
namespace io {
namespace buf {

StrataBufferReader::StrataBufferReader(const char** data,
                                       const char* data_end) {
    data_ = data;
    data_end_ = data_end;
}

bool StrataBufferReader::ReadRaw(size_t size, char* bytes) {
    assert(*data_ <= data_end_);
    if (data_end_ - *data_ < static_cast<long>(size)) {
        return false;
    }
    memcpy(bytes, *data_, size);
    *data_ += size;
    return true;
}

bool StrataBufferReader::IsAtEnd() const {
    return *data_ == data_end_;
}

}  // namespace buf
}  // namespace io
}  // namespace strata
