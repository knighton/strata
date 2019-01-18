#pragma once

#include "base/reader.h"
#include "base/writer.h"

using strata::base::Reader;
using strata::base::Writer;

namespace strata {
namespace io {

class BufferReader : public Reader {
  public:
    BufferReader(const char** data, const char* data_end);

  private:
    bool ReadRaw(size_t size, char* bytes) override;

    bool IsAtEnd() const override;

    const char** data_{nullptr};
    const char* data_end_{nullptr};
};

class BufferWriter : public Writer {
  public:
    BufferWriter(char** data, const char* data_end);

  private:
    bool WriteRaw(const char* bytes, size_t size) override;

    char** data_{nullptr};
    const char* data_end_{nullptr};
};

}  // namespace io
}  // namespace strata
