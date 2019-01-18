#pragma once

#include "base/reader.h"
#include "base/writer.h"

using strata::base::Reader;
using strata::base::Writer;

namespace strata {
namespace io {

class StringReader : public Reader {
  public:
    StringReader(const string* data, size_t* index);

  private:
    bool IsAtEnd() const override;

    bool ReadRaw(size_t size, char* bytes) override;

    const string* data_{nullptr};
    size_t* index_{nullptr};
};

class StringWriter : public Writer {
  public:
    StringWriter(string* data);

  private:
    bool WriteRaw(const char* bytes, size_t size) override;

    string* data_{nullptr};
};

}  // namespace io
}  // namespace strata
