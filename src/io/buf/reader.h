#pragma once

#include "io/base/reader.h"

using strata::io::base::StrataReader;

namespace strata {
namespace io {
namespace buf {

class StrataBufferReader : public StrataReader {
  public:
    StrataBufferReader(const char** data, const char* data_end);

  private:
    bool ReadRaw(uint32_t size, char* bytes, uint32_t* bytes_read) override;

    bool IsAtEnd() const override;

    const char** data_{nullptr};
    const char* data_end_{nullptr};
};

}  // namespace buf
}  // namespace io
}  // namespace strata
