#include "string.h"

#include <cstring>

using std::memcpy;

namespace strata {
namespace io {

StringReader::StringReader(const string* data, size_t* index) {
    data_ = data;
    index_ = index;
}

bool StringReader::ReadRaw(size_t size, char* bytes) {
    if (data_->size() <= *index_ + size) {
        return false;
    }
    memcpy(bytes, &(*data_)[*index_], size);
    *index_ += size;
    return true;
}

bool StringReader::IsAtEnd() const {
    return *index_ == data_->size();
}

StringWriter::StringWriter(string* data) {
    data_ = data;
}

bool StringWriter::WriteRaw(const char* bytes, size_t size) {
    size_t index = data_->size();
    data_->resize(index + size);
    memcpy(&(*data_)[index], bytes, size);
    return true;
}

}  // namespace io
}  // namespace strata
