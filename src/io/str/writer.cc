#include "writer.h"

#include <cstring>

using std::memcpy;

namespace strata {
namespace io {
namespace str {

StrataStringWriter::StrataStringWriter(string* data) {
    data_ = data;
}

bool StrataStringWriter::WriteRaw(const char* bytes, size_t size) {
    size_t index = data_->size();
    data_->resize(index + size);
    memcpy(&(*data_)[index], bytes, size);
    return true;
}

}  // namespace str
}  // namespace io
}  // namespace strata
